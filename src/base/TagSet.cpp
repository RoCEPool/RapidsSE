/*============================================================================*/
/* TagSet.cpp -- name-value tag set                                           */
/*============================================================================*/

#include "BASE.h"
#include "Bytes.hpp"

static const Stz* C_BEGIN_TAGSET    = "Begin_TagSet"_Stz;
static const Stz* C_END_TAGSET      = "End_TagSet"_Stz;
static const Stz* C_BEGIN_TAGARRAY  = "Begin_TagArray"_Stz;
static const Stz* C_END_TAGARRAY    = "End_TagArray"_Stz;
static const Stz* C_BEGIN_STRBUF    = "Begin_StrBuf"_Stz;
static const Stz* C_END_STRBUF      = "End_StrBuf"_Stz;
static const Stz* C_BEGIN_STRSEG    = "Begin_StrSeg"_Stz;
static const Stz* C_END_STRSEG      = "End_StrSeg"_Stz;

static const i8 C_SIZE_BEGIN_TAGSET  = C_BEGIN_TAGSET->nLength() + 1;
static const i8 C_SIZE_END_TAGSET    = C_END_TAGSET->nLength() + 1;
static const i8 C_SIZE_BEGIN_TAGARRAY= C_BEGIN_TAGARRAY->nLength() + 1;
static const i8 C_SIZE_END_TAGARRAY  = C_END_TAGARRAY->nLength() + 1;
static const i8 C_SIZE_BEGIN_STRBUF  = C_BEGIN_STRBUF->nLength() + 1;
static const i8 C_SIZE_END_STRBUF    = C_END_STRBUF->nLength() + 1;
static const i8 C_SIZE_BEGIN_STRSEG  = C_BEGIN_STRSEG->nLength() + 1;
static const i8 C_SIZE_END_STRSEG    = C_END_STRSEG->nLength() + 1;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
TagSet* BASE::_pTagSet(void* aAddr)
{
  TagSet* pTagSet = reinterpret_cast<TagSet*>(aAddr);
  
  RASSERT_TYPE(pTagSet,CN_TAGSET);
  
  return pTagSet;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* TagSet::operator new(size_t)
{
  void* pObject = Heaps::pAllocate(CN_TAGSET);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void TagSet::operator delete(void* aAddr)
{
  Object* pObject = reinterpret_cast<Object*>(aAddr);
  
  Heaps::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
TagSet::TagSet(void)
: Object(CN_TAGSET)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
  
  _checkInitialised_(CN_TAGSET);
  
  _postcondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* Copy constructor                                                           */
/*----------------------------------------------------------------------------*/
TagSet::TagSet(const TagSet& oOther)
: Object(CN_TAGSET)
, m_oTagArray(oOther.m_oTagArray)
, m_xyStrings(oOther.m_xyStrings)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  _checkInitialised_(CN_TAGSET);

  _postcondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TagSet::~TagSet(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* repair a tagset                                                            */
/*----------------------------------------------------------------------------*/
void TagSet::vRepair(Object* pObject)
{
  _static_

  TagSet* pTagSet = _pTagSet(pObject);

  delete pTagSet;
}

/*----------------------------------------------------------------------------*/
/* serialise a tagset to a string                                             */
/*----------------------------------------------------------------------------*/
bool TagSet::bSerialise(TagSet* pTagSet, Bytes* pBuf)
{
  _static_

  ASSERT(pBuf);
  ASSERT(pBuf->bIsEmpty());
  ASSERT(pBuf->i_Latter() == 0);

  // (1) The overall size of the serialised data will be filled in at the end
  i8* piSerialisedSize = reinterpret_cast<i8*>(pBuf->szLatter());
  pBuf->vTake(sizeof(i8));

  // (2)
  pBuf->iAppend(C_BEGIN_TAGSET);

  // (3) Write the array of tags
  pBuf->iAppend(C_BEGIN_TAGARRAY);

    // (4) First write the number of Tag entries in the array
    pBuf->iAppend(pTagSet->m_oTagArray.nItems());

    // (5) Now write each tag sequentially. Need to accommodate that the array is segmented.
    TagArrayIter xIter = pTagSet->m_oTagArray.xIterator();
    Tag* pTag;
    while ((pTag = xIter.pNext()) != nullptr) {
      pBuf->iAppend(reinterpret_cast<byte*>(pTag), sizeof(Tag));
    }

  // (6)
  pBuf->iAppend(C_END_TAGARRAY);

  // (7) Now write the associated strings
  pBuf->iAppend(C_BEGIN_STRBUF);

    // (8) First write the number of segments
    pBuf->iAppend(1);

      // (9) For each segment...
      pBuf->iAppend(C_BEGIN_STRSEG);

      // (10) Now write the size of the segment. This is important because the tag contains
      // offsets from the beginning of the first segment and there can be plenty of
      // unused space at the end of segments.
      pBuf->iAppend(pTagSet->m_xyStrings.nArea());

      // (11) Now write the size of the data in this segment
      i8 iStrSize = pTagSet->m_xyStrings.i_Latter();
      pBuf->iAppend(iStrSize);

      // (12) Write the data in this segment
      pBuf->iAppend(reinterpret_cast<byte*>(pTagSet->m_xyStrings.szArea()), iStrSize);

      // (13)
      pBuf->iAppend(C_END_STRSEG);

  // (14)
  pBuf->iAppend(C_END_STRBUF);

  // (15)
  pBuf->iAppend(C_END_TAGSET);

  // Now we can write the overall serialised size
  *piSerialisedSize = pBuf->i_Latter();

  RASSERT(pBuf->iRemaining() >= 0);

  return true;
}

/*----------------------------------------------------------------------------*/
/* deserialise a tagset from a string                                         */
/*----------------------------------------------------------------------------*/
TagSet* TagSet::pDeserialise(Bytes* pBuf)
{
  _static_

  ASSERT(pBuf);
  ASSERT(pBuf->i_Latter() > 0);

  TagSet* pTagSet = new TagSet();
  i8 iPos = 0;

  // (1) Read the overall size of the serialised data
  i8 iSerialisedSize = pBuf->iGet(iPos);
  UNUSED(iSerialisedSize);
  iPos += sizeof(i8);
  ASSERT(iSerialisedSize > 0);

  // (2) Read C_BEGIN_TAGSET
  ASSERT(pBuf->szGet(iPos)->bEqual(C_BEGIN_TAGSET));
  iPos += C_SIZE_BEGIN_TAGSET;

    // (3) Read C_BEGIN_TAGARRAY
    ASSERT(pBuf->szGet(iPos)->bEqual(C_BEGIN_TAGARRAY));
    iPos += C_SIZE_BEGIN_TAGARRAY;

      // (4) Read the number of tags
      i8 nTags = pBuf->iGet(iPos);
      iPos += sizeof(i8);
      ASSERT(nTags >= 0);

      // (5) Read each tag and add it to the array
      for (i8 i=0; i<nTags; i++) {
        Tag* pTag = reinterpret_cast<Tag*>(pBuf->szGet(iPos));
        pTagSet->m_oTagArray.vAdd(*pTag);
        iPos += sizeof(Tag);
      }

    // (6) Read C_END_TAGARRAY
    ASSERT(pBuf->szGet(iPos)->bEqual(C_END_TAGARRAY));
    iPos += C_SIZE_END_TAGARRAY;

    // (7) Read C_BEGIN_STRBUF
    ASSERT(pBuf->szGet(iPos)->bEqual(C_BEGIN_STRBUF));
    iPos += C_SIZE_BEGIN_STRBUF;

      // (8) Read the number of segments
      i8 nStrSegments = pBuf->iGet(iPos);
      iPos += sizeof(i8);
      ASSERT(nStrSegments > 0);

        // (9) Read C_BEGIN_STRSEG
        ASSERT(pBuf->szGet(iPos)->bEqual(C_BEGIN_STRSEG));
        iPos += C_SIZE_BEGIN_STRSEG;

        for (i8 i=0; i<nStrSegments; i++) {
          // (10) Read the size of this segment
          i8 iSegSize = pBuf->iGet(iPos);
          iPos += sizeof(i8);
          ASSERT(iSegSize > 0);

          // (11) Read the size of the data in this segment
          i8 iStrSize = pBuf->iGet(iPos);
          iPos += sizeof(i8);
          ASSERT(iStrSize >= 0);

          // (12) Read the data of this segment and add it to the string buffer
          Stz* szStr = pBuf->szGet(iPos);
          pTagSet->m_xyStrings.iAppend(reinterpret_cast<byte*>(szStr), iStrSize);
          iPos += iStrSize;
        }

        // (13) Read C_END_STRSEG
        ASSERT(pBuf->szGet(iPos)->bEqual(C_END_STRSEG));
        iPos += C_SIZE_END_STRSEG;

    // (14) Read C_END_STRBUF
    ASSERT(pBuf->szGet(iPos)->bEqual(C_END_STRBUF));
    iPos += C_SIZE_END_STRBUF;

  // (15) Read C_END_TAGSET
  ASSERT(pBuf->szGet(iPos)->bEqual(C_END_TAGSET));
  iPos += C_SIZE_END_TAGSET;

  return pTagSet;
}



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append name only                                                           */
/*----------------------------------------------------------------------------*/
void TagSet::vAppend(Stz* szName)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  Tag xTag{m_xyStrings.iAppend(szName), C_UNKNOWN, TN_STRING};
  m_oTagArray.vAdd(xTag);

  ASSERT(bHasName(szName));
}

/*----------------------------------------------------------------------------*/
/* append name = int                                                          */
/*----------------------------------------------------------------------------*/
void TagSet::vAppend(Stz* szName, i8 iValue)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  Tag xTag{m_xyStrings.iAppend(szName), iValue, TN_INTEGER};
  m_oTagArray.vAdd(xTag);

  ASSERT(bHasName(szName));
}

/*----------------------------------------------------------------------------*/
/* append name = int                                                          */
/*----------------------------------------------------------------------------*/
void TagSet::vAppend(Stz* szName, Object* pObject)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  Tag xTag{m_xyStrings.iAppend(szName), (i8) pObject, TN_OBJECT};
  m_oTagArray.vAdd(xTag);

  ASSERT(bHasName(szName));
}

/*----------------------------------------------------------------------------*/
/* append name = str                                                          */
/*----------------------------------------------------------------------------*/
void TagSet::vAppend(Stz* szName, Stz* szValue)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  Tag xTag{m_xyStrings.iAppend(szName), m_xyStrings.iAppend(szValue), TN_STRING};
  m_oTagArray.vAdd(xTag);

  ASSERT(bHasName(szName));
}

/*----------------------------------------------------------------------------*/
/* concat tagSet to cmd string                                              */
/*----------------------------------------------------------------------------*/
void TagSet::vConcat(String* pCmd)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  TagArrayIter xIter = m_oTagArray.xIterator();
  Tag* pTag;

  while ((pTag = xIter.pNext()) != nullptr) {

    pCmd->vConcat(" "_Stz);
    
    if (TN_INTEGER == pTag->m_eToken) {

      pCmd->vCatSprint("%lld "_Stz,
                       pTag->m_i_Value
             );
      continue;
    }
    pCmd->vConcat("\""_Stz);
    pCmd->vConcat(m_xyStrings.szGet(pTag->m_i_Value));
    pCmd->vConcat("\""_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* append tagSet to cmd string                                                */
/*----------------------------------------------------------------------------*/
void TagSet::vConcat(String* pCmd, i8 i_Tag)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  ASSERT(i_Tag >= 0);
  ASSERT(i_Tag < m_oTagArray.nItems());

  Tag* pTag = m_oTagArray.pGet(i_Tag);
  
  pCmd->vConcat(" "_Stz);
  
  if (TN_INTEGER == pTag->m_eToken) {
    
    pCmd->vCatSprint("%lld "_Stz,
                     pTag->m_i_Value
                      );
  }
  else {
    pCmd->vConcat("\""_Stz);
    pCmd->vConcat(m_xyStrings.szGet(pTag->m_i_Value));
    pCmd->vConcat("\""_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* content dump                                                               */
/*----------------------------------------------------------------------------*/
void TagSet::vContentDump(void)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  if (m_oTagArray.nItems()) {
    vTrace("\n"_Stz);
    vTrace("Tag Set\n"_Stz);
    vTrace("-------\n"_Stz);
    
    vDumpSet();
  }
}

/*----------------------------------------------------------------------------*/
/* copy source ename-set into this ename-set                                  */
/*----------------------------------------------------------------------------*/
void TagSet::vCopy(TagSet* pScrTagSet)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
  
  for (i8 i_Tag = 0; i_Tag < pScrTagSet->nItems(); i_Tag++) {
    
    Stz* szName = pScrTagSet->szName(i_Tag);
    
    if (bHasName(szName)) {
      if (TN_STRING == pScrTagSet->eToken(i_Tag)) {
        vReplace(szName,pScrTagSet->szValue(i_Tag));
      }
      else
      if (TN_OBJECT == pScrTagSet->eToken(i_Tag)) {
        vReplace(szName,pScrTagSet->pValue(i_Tag));
      }
      else {
        vReplace(szName,pScrTagSet->iValue(i_Tag));
      }
    }
    else {
      if (TN_STRING == pScrTagSet->eToken(i_Tag)) {
        vInsert(szName,pScrTagSet->szValue(i_Tag));
      }
      else
      if (TN_OBJECT == pScrTagSet->eToken(i_Tag)) {
        vInsert(szName,pScrTagSet->pValue(i_Tag));
      }
      else {
        vInsert(szName,pScrTagSet->iValue(i_Tag));
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/* copy source ename-set into this ename-set and replace an ename pair        */
/*----------------------------------------------------------------------------*/
void TagSet::vDelete(Stz* szName)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
  
  TagSet xTagSet;
    
  xTagSet.vCopy(this);
  vReset();

  for (i8 i_Tag = 0; i_Tag < xTagSet.nItems(); i_Tag++) {
    
    if (xTagSet.szName(i_Tag)->bSame(szName))
      continue;
    
    if (TN_STRING == xTagSet.eToken(i_Tag))
      vInsert(xTagSet.szName(i_Tag),xTagSet.szValue(i_Tag));
    else
    if (TN_OBJECT == xTagSet.eToken(i_Tag))
      vInsert(xTagSet.szName(i_Tag),xTagSet.pValue(i_Tag));
    else
      vInsert(xTagSet.szName(i_Tag),xTagSet.iValue(i_Tag));
  }
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void TagSet::vDumpSet(bool bCarriageReturns)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
  
  vTrace("{ "_Stz);
  if (bCarriageReturns)
    vTrace("\n"_Stz);

  TagArrayIter xIter = m_oTagArray.xIterator();
  Tag* pTag;

  while ((pTag = xIter.pNext()) != nullptr) {

    if ((! bCarriageReturns) && xIter.iArrayIndex())
      vTrace(", "_Stz);
    
    if (bCarriageReturns && xIter.iArrayIndex())
      vTrace("\n"_Stz);
    
    if (bCarriageReturns)
      vTrace("  [%02lld] %7s : "_Stz
             ,xIter.iArrayIndex()
             ,aTN[pTag->m_eToken]);

    if (TN_INTEGER == pTag->m_eToken) {
      vTrace(bCarriageReturns
               ? "%16s = %lld"_Stz
               : "%s=%lld"_Stz
             
             ,m_xyStrings.szGet(pTag->m_i_Name)
             ,pTag->m_i_Value
              );
    }
    else {
      vTrace(bCarriageReturns
               ? "%16s = %s"_Stz
               : "%s=%s"_Stz

             ,m_xyStrings.szGet(pTag->m_i_Name)
             ,m_xyStrings.szGet(pTag->m_i_Value)
              );
    }
  }
  
  
  if (bCarriageReturns)
    vTrace("\n"_Stz);
  
  vTrace("BYTES=%lld/%lld"_Stz,m_xyStrings.i_Latter(),m_xyStrings.nSize());

  if (bCarriageReturns)
    vTrace("\n"_Stz);

  vTrace("}\n"_Stz,m_xyStrings.i_Latter(),m_xyStrings.nSize());
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void TagSet::vDumpSetDetailed(void)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  TagArrayIter xIter = m_oTagArray.xIterator();
  Tag* pTag;

  while ((pTag = xIter.pNext()) != nullptr) {
    vTrace("  [%lld] %s _%lld, %s _%lld (%s)\n"_Stz,
           
           xIter.iArrayIndex(),
           
           m_xyStrings.szGet(pTag->m_i_Name),
           pTag->m_i_Name,
           
           m_xyStrings.szGet(pTag->m_i_Value),
           pTag->m_i_Value,
           
           aTN[pTag->m_eToken],
           
           C_VARGS_END
           );
  }
}

/*----------------------------------------------------------------------------*/
/* duplicate the TagSet                                                       */
/*----------------------------------------------------------------------------*/
TagSet* TagSet::pDuplicate(void)
{
  TagSet* pNewTagSet = new TagSet(*this);
  return pNewTagSet;
}

/*----------------------------------------------------------------------------*/
/* find tag by name                                                           */
/*----------------------------------------------------------------------------*/
i8 TagSet::i_Find(Stz *szName)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  TagArrayIter xIter = m_oTagArray.xIterator();
  Tag* pTag;

  while ((pTag = xIter.pNext()) != nullptr) {
    if (m_xyStrings.szGet(pTag->m_i_Name)->bSame(szName))
      return xIter.iArrayIndex();
  }
  
  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* find tag by name                                                           */
/*----------------------------------------------------------------------------*/
Tag* TagSet::pFind(Stz *szName)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  TagArrayIter xIter = m_oTagArray.xIterator();
  Tag* pTag;

  while ((pTag = xIter.pNext()) != nullptr) {
    if (m_xyStrings.szGet(pTag->m_i_Name)->bSame(szName))
      return pTag;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* test if ename-set has a pair with given name                               */
/*----------------------------------------------------------------------------*/
bool TagSet::bHasName(Stz *szName)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
  
  i8 i_Tag = i_Find(szName);
  
  if (C_UNKNOWN == i_Tag)
    return false;
  else
    return true;
}

/*----------------------------------------------------------------------------*/
/* Insert the tag in alphabetical order according to the tag name             */
/*----------------------------------------------------------------------------*/
void TagSet::vInsert(Stz* szName)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  i8 i_Tag = i_InsertPoint(szName);

  Tag xTag{m_xyStrings.iAppend(szName), C_UNKNOWN, TN_STRING};
  m_oTagArray.vInsert(xTag, i_Tag);

  ASSERT(bHasName(szName));
}


/*----------------------------------------------------------------------------*/
/* Insert the tag in alphabetical order according to the tag name             */
/*----------------------------------------------------------------------------*/
void TagSet::vInsert(Stz* szName, i8 iValue)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  i8 i_Tag = i_InsertPoint(szName);

  Tag xTag{m_xyStrings.iAppend(szName), iValue, TN_INTEGER};
  m_oTagArray.vInsert(xTag, i_Tag);
  
  ASSERT(bHasName(szName));
}

/*----------------------------------------------------------------------------*/
/* Insert the tag in alphabetical order according to the tag name             */
/*----------------------------------------------------------------------------*/
void TagSet::vInsert(Stz* szName, Object* pValue)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  i8 i_Tag = i_InsertPoint(szName);

  Tag xTag{m_xyStrings.iAppend(szName), (i8) pValue, TN_OBJECT};
  m_oTagArray.vInsert(xTag, i_Tag);

  ASSERT(bHasName(szName));
}

/*----------------------------------------------------------------------------*/
/* Insert the tag in alphabetical order according to the tag name             */
/*----------------------------------------------------------------------------*/
void TagSet::vInsert(Stz* szName, Stz* szValue)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  i8 i_Tag = i_InsertPoint(szName);

  Tag xTag{m_xyStrings.iAppend(szName), m_xyStrings.iAppend(szValue), TN_STRING};
  m_oTagArray.vInsert(xTag, i_Tag);

  ASSERT(bHasName(szName));
}

/*----------------------------------------------------------------------------*/
/* is token a decodable enum?                                                 */
/*----------------------------------------------------------------------------*/
bool TagSet::bIsDecodable(i8 i_Tag)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
  
  Stz* szName = this->szName(i_Tag);
  
  if (szName->bSame("<crossTopo>"_Stz))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* is token a decodable enum?                                                 */
/*----------------------------------------------------------------------------*/
bool TagSet::bIsDecodable(Tag* pTag)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  Stz* szName = this->szName(pTag);

  if (szName->bSame("<crossTopo>"_Stz))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is token kind an integer?                                                  */
/*----------------------------------------------------------------------------*/
bool TagSet::bIsInteger(i8 i_Tag)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
  
  eTN eToken = m_oTagArray.pGet(i_Tag)->m_eToken;
  
  if (TN_INTEGER == eToken)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is token kind an integer?                                                  */
/*----------------------------------------------------------------------------*/
bool TagSet::bIsInteger(Tag* pTag)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  eTN eToken = pTag->m_eToken;

  if (TN_INTEGER == eToken)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is token kind a pointer to object?                                         */
/*----------------------------------------------------------------------------*/
bool TagSet::bIsObject(i8 i_Tag)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  eTN eToken = m_oTagArray.pGet(i_Tag)->m_eToken;

  if (TN_OBJECT == eToken)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is token kind a pointer to object?                                         */
/*----------------------------------------------------------------------------*/
bool TagSet::bIsObject(Tag* pTag)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  eTN eToken = pTag->m_eToken;

  if (TN_OBJECT == eToken)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is token kind a string?                                                    */
/*----------------------------------------------------------------------------*/
bool TagSet::bIsString(i8 i_Tag)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
  
  eTN eToken = m_oTagArray.pGet(i_Tag)->m_eToken;
  
  if (TN_STRING == eToken)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is token kind a string?                                                    */
/*----------------------------------------------------------------------------*/
bool TagSet::bIsString(Tag* pTag)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  eTN eToken = pTag->m_eToken;

  if (TN_STRING == eToken)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* get nItems                                                                 */
/*----------------------------------------------------------------------------*/
i8 TagSet::nItems(void)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
  
  return m_oTagArray.nItems();
}

/*----------------------------------------------------------------------------*/
/* get name                                                                   */
/*----------------------------------------------------------------------------*/
Stz* TagSet::szName(i8 i_Tag)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  Tag* pTag = m_oTagArray.pGet(i_Tag);
  Stz* szName = m_xyStrings.szGet(pTag->m_i_Name);
  
  return szName;
}

/*----------------------------------------------------------------------------*/
/* get name                                                                   */
/*----------------------------------------------------------------------------*/
Stz* TagSet::szName(Tag* pTag)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  Stz* szName = m_xyStrings.szGet(pTag->m_i_Name);

  return szName;
}

/*----------------------------------------------------------------------------*/
/* produce names string                                                       */
/*----------------------------------------------------------------------------*/
void TagSet::vNamesString(String* pString)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  char* azCrs = pString->azStr();
  const char* azDstLastChar = pString->azStr() + pString->nAreaBytes() - 1;

  TagArrayIter xIter = m_oTagArray.xIterator();
  Tag* pTag;

  while ((pTag = xIter.pNext()) != nullptr) {
    if (xIter.iArrayIndex())
      StrLib::vAppendChar(&azCrs, azDstLastChar, ' ');
    char* azName = m_xyStrings.szGet(pTag->m_i_Name)->azChar();
    StrLib::vAppendString(&azCrs, azDstLastChar, azName);
  }
}

/*----------------------------------------------------------------------------*/
/* copy source ename-set into this ename-set and replace an ename pair        */
/*----------------------------------------------------------------------------*/
void TagSet::vReplace(Stz* szName, i8 iValue)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  // TODO: This can be simplified

  TagSet xOriginalTagSet;

  bool bWasCopied = false;

  xOriginalTagSet.vCopy(this);
  vReset();

  for (i8 i_Tag = 0; i_Tag < xOriginalTagSet.nItems(); i_Tag++) {

    if (xOriginalTagSet.szName(i_Tag)->bSame(szName)) {
      vInsert(szName,iValue);
      bWasCopied = true;
      continue;
    }

    if (TN_STRING == xOriginalTagSet.eToken(i_Tag))
      vInsert(xOriginalTagSet.szName(i_Tag),xOriginalTagSet.szValue(i_Tag));
    else
    if (TN_OBJECT == xOriginalTagSet.eToken(i_Tag))
      vInsert(xOriginalTagSet.szName(i_Tag),xOriginalTagSet.pValue(i_Tag));
    else
      vInsert(xOriginalTagSet.szName(i_Tag),xOriginalTagSet.iValue(i_Tag));
  }

  if (! bWasCopied)
    vInsert(szName,iValue);
}

/*----------------------------------------------------------------------------*/
/* copy source ename-set into this ename-set and replace an ename pair        */
/*----------------------------------------------------------------------------*/
void TagSet::vReplace(Stz* szName, Object* pValue)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  // TODO: This can be simplified

  TagSet xOriginalTagSet;

  bool bWasCopied = false;

  xOriginalTagSet.vCopy(this);
  vReset();

  for (i8 i_Tag = 0; i_Tag < xOriginalTagSet.nItems(); i_Tag++) {

    if (xOriginalTagSet.szName(i_Tag)->bSame(szName)) {
      vInsert(szName,pValue);
      bWasCopied = true;
      continue;
    }

    if (TN_STRING == xOriginalTagSet.eToken(i_Tag))
      vInsert(xOriginalTagSet.szName(i_Tag),xOriginalTagSet.szValue(i_Tag));
    else
    if (TN_OBJECT == xOriginalTagSet.eToken(i_Tag))
      vInsert(xOriginalTagSet.szName(i_Tag),xOriginalTagSet.pValue(i_Tag));
    else
      vInsert(xOriginalTagSet.szName(i_Tag),xOriginalTagSet.iValue(i_Tag));
  }

  if (! bWasCopied)
    vInsert(szName,pValue);
}

/*----------------------------------------------------------------------------*/
/* copy source ename-set into this ename-set and replace an ename pair        */
/*----------------------------------------------------------------------------*/
void TagSet::vReplace(Stz* szName, Stz* szValue)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  // TODO: This can be simplified

  TagSet xOriginalTagSet;

  bool bWasCopied = false;

  xOriginalTagSet.vCopy(this);
  vReset();

  for (i8 i_Tag = 0; i_Tag < xOriginalTagSet.nItems(); i_Tag++) {

    if (xOriginalTagSet.szName(i_Tag)->bSame(szName)) {
      vInsert(szName,szValue);
      bWasCopied = true;
      continue;
    }

    if (TN_STRING == xOriginalTagSet.eToken(i_Tag))
      vInsert(xOriginalTagSet.szName(i_Tag),xOriginalTagSet.szValue(i_Tag));
    else
    if (TN_OBJECT == xOriginalTagSet.eToken(i_Tag))
      vInsert(xOriginalTagSet.szName(i_Tag),xOriginalTagSet.pValue(i_Tag));
    else
      vInsert(xOriginalTagSet.szName(i_Tag),xOriginalTagSet.iValue(i_Tag));
  }

  if (! bWasCopied)
    vInsert(szName,szValue);
}

/*----------------------------------------------------------------------------*/
/* get token kind                                                             */
/*----------------------------------------------------------------------------*/
void TagSet::vReset(void)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  m_oTagArray.vReset();
  m_xyStrings.vReset();
}

/*----------------------------------------------------------------------------*/
/* sprint                                                                     */
/*----------------------------------------------------------------------------*/
void TagSet::vSprint(String* psArgs, bool bParenth, bool bNames, bool bDecode)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
  UNUSED(bDecode);
  
  if (bParenth)
    psArgs->vCopy("("_Stz);

  TagArrayIter xIter = m_oTagArray.xIterator();
  Tag* pTag;

  while ((pTag = xIter.pNext()) != nullptr) {

    if (xIter.iArrayIndex())
      psArgs->vConcat(", "_Stz);
    
    if (bNames) {
      psArgs->vConcat(szName(pTag));
      psArgs->vConcat("="_Stz);
    }
    
    if (pTag->m_eToken == TN_STRING) {
      psArgs->vConcat(m_xyStrings.szGet(pTag->m_i_Value));
      continue;
    }
    
    ASSERT(bIsInteger(pTag));

    s32 sValue;
    
    if (bIsDecodable(xIter.iArrayIndex()))
      sValue = szValueDecoded(xIter.iArrayIndex());
    else
      sValue.vSprint("%lld"_Stz, pTag->m_i_Value);
    psArgs->vConcat(sValue);
  }
  
  if (bParenth)
    psArgs->vConcat(")"_Stz);
}

/*----------------------------------------------------------------------------*/
/* get tage                                                                   */
/*----------------------------------------------------------------------------*/
Tag* TagSet::pTag(i8 i_Tag)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
    
  return m_oTagArray.pGet(i_Tag);
}

/*----------------------------------------------------------------------------*/
/* get token kind                                                             */
/*----------------------------------------------------------------------------*/
eTN TagSet::eToken(i8 i_Tag)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
  
  eTN eToken = m_oTagArray.pGet(i_Tag)->m_eToken;
  
  return eToken;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool TagSet::bSame(TagSet* pTagSet_arg)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
  
  i8 nTags = this->nItems();
  i8 nTags_arg = pTagSet_arg->nItems();

  if (nTags != nTags_arg)
    return false;
  
  for (i8 i_Tag = 0; i_Tag < nTags; i_Tag++) {
    
    Stz* szName = this->szName(i_Tag);
    i8 i_Tag_arg = pTagSet_arg->i_Find(szName);
    if (C_UNKNOWN == i_Tag_arg)
      return false;
    
    eTN eToken = this->eToken(i_Tag);
    eTN eToken_arg = pTagSet_arg->eToken(i_Tag_arg);
    if (eToken != eToken_arg)
      return false;
    
    if (TN_STRING == eToken) {
      if (! this->szValue(i_Tag)->bSame(pTagSet_arg->szValue(i_Tag_arg)))
        return false;
    }
    else
    if (TN_OBJECT == eToken) {
      if (this->pValue(i_Tag) != pTagSet_arg->pValue(i_Tag_arg))
        return false;
    }
    else {
      RASSERT(TN_INTEGER == eToken);
      if (this->iValue(i_Tag) != pTagSet_arg->iValue(i_Tag_arg))
        return false;
    }
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* to string                                                                  */
/*----------------------------------------------------------------------------*/
void TagSet::vToString(String* psStr, bool bValueOnly)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
  
  if (! nItems()) {
    psStr->vConcat("(empty)"_Stz);
    return;
  }

  TagArrayIter xIter = m_oTagArray.xIterator();
  Tag* pTag;

  while ((pTag = xIter.pNext()) != nullptr) {

    if (bValueOnly) {
      if (xIter.iArrayIndex())
        psStr->vConcat(" "_Stz);
    }
    else {
      if (xIter.iArrayIndex())
        psStr->vConcat(", "_Stz);

      psStr->vConcat(szName(pTag));
      psStr->vConcat("="_Stz);
    }

    if (bIsString(pTag)) {
      psStr->vConcat(m_xyStrings.szGet(pTag->m_i_Value));
      continue;
    }
    
    //ASSERT(bIsInteger(i_Tag));

    s32 sValue;

    if (bIsObject(pTag)) {
      sValue.vSprint( "%p"_Stz, pValue(pTag));
      psStr->vConcat(sValue);
      continue;
    }

    if (bIsDecodable(pTag))
      sValue = szValueDecoded(pTag);

    else
      sValue.vSprint("%lld"_Stz,iValue(pTag));
    psStr->vConcat(sValue);
  }
}

/*----------------------------------------------------------------------------*/
/* get value                                                                  */
/*----------------------------------------------------------------------------*/
i8 TagSet::iValue(i8 i_Tag)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  i8 iValue = C_UNKNOWN;

  Tag* pTag = m_oTagArray.pGet(i_Tag);
  ASSERT(pTag);

  if (TN_INTEGER == pTag->m_eToken) {
    iValue = pTag->m_i_Value;
  }
  else {
    Stz* szValue = m_xyStrings.szGet(pTag->m_i_Value);
    iValue = szValue->iToI8();
  }
  return iValue;
}

/*----------------------------------------------------------------------------*/
/* get value                                                                  */
/*----------------------------------------------------------------------------*/
Stz* TagSet::szValue(i8 i_Tag)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
  
  Tag* pTag = m_oTagArray.pGet(i_Tag);
  ASSERT(pTag);
  ASSERT(TN_INTEGER != pTag->m_eToken);
  Stz* szValue = m_xyStrings.szGet(pTag->m_i_Value);

  return szValue;
}

/*----------------------------------------------------------------------------*/
/* get decoded value                                                          */
/*----------------------------------------------------------------------------*/
Stz* TagSet::szValueDecoded(i8 i_Tag)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
  
  ASSERT(TN_INTEGER == m_oTagArray.pGet(i_Tag)->m_eToken);
  Stz* szValue = "cantDecode"_Stz;

  Stz* szName = this->szName(i_Tag);

  if (szName->bSame("<crossTopo>"_Stz)) {
    i8 iValue = this->iValue(i_Tag);
    szValue = aYX[iValue];
  }
                  
  return szValue;
}

/*----------------------------------------------------------------------------*/
/* get decoded value                                                          */
/*----------------------------------------------------------------------------*/
Stz* TagSet::szValueDecoded(Tag* pTag)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  ASSERT(TN_INTEGER == pTag->m_eToken);
  Stz* szValue = "cantDecode"_Stz;

  Stz* szName = this->szName(pTag);

  if (szName->bSame("<crossTopo>"_Stz)) {
    i8 iValue = this->iValue(pTag);
    szValue = aYX[iValue];
  }

  return szValue;
}

/*----------------------------------------------------------------------------*/
/* get value                                                                  */
/*----------------------------------------------------------------------------*/
i8 TagSet::iValue(Stz *szName)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  Tag* pTag = pFind(szName);
  if (!pTag) {
    return 0;
  }

  ASSERT(TN_INTEGER == pTag->m_eToken);
  i8 iValue = pTag->m_i_Value;

  return iValue;
}

/*----------------------------------------------------------------------------*/
/* get value                                                                  */
/*----------------------------------------------------------------------------*/
i8 TagSet::iValue(Tag* pTag)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  ASSERT(TN_INTEGER == pTag->m_eToken);
  return pTag->m_i_Value;
}

/*----------------------------------------------------------------------------*/
/* get value                                                                  */
/*----------------------------------------------------------------------------*/
Object* TagSet::pValue(i8 i_Tag)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
  
  ASSERT(TN_OBJECT == m_oTagArray.pGet(i_Tag)->m_eToken);
  Object* pObject = (Object*)m_oTagArray.pGet(i_Tag)->m_i_Value;

  return pObject;
}

/*----------------------------------------------------------------------------*/
/* get value                                                                  */
/*----------------------------------------------------------------------------*/
Object* TagSet::pValue(Tag* pTag)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  ASSERT(TN_OBJECT == pTag->m_eToken);
  Object* pObject = (Object*)pTag->m_i_Value;

  return pObject;
}

/*----------------------------------------------------------------------------*/
/* get value                                                                  */
/*----------------------------------------------------------------------------*/
Object* TagSet::pValue(Stz *szName)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  Tag* pTag = pFind(szName);
  if (!pTag) {
    return 0;
  }

  ASSERT(TN_OBJECT == pTag->m_eToken);
  Object* pObject = (Object*)pTag->m_i_Value;

  return pObject;
}

/*----------------------------------------------------------------------------*/
/* get value                                                                  */
/*----------------------------------------------------------------------------*/
Stz* TagSet::szValue(Stz *szName)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  Tag* pTag = pFind(szName);
  if (!pTag) {
    return ""_Stz;
  }

  ASSERT(TN_INTEGER != pTag->m_eToken);
  Stz* szValue = m_xyStrings.szGet(pTag->m_i_Value);
  
  return szValue;
}

/*----------------------------------------------------------------------------*/
/* get value                                                                  */
/*----------------------------------------------------------------------------*/
Stz* TagSet::szValue(Tag* pTag)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  ASSERT(TN_INTEGER != pTag->m_eToken);
  Stz* szValue = m_xyStrings.szGet(pTag->m_i_Value);

  return szValue;
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* find where to add the given name                                           */
/*----------------------------------------------------------------------------*/
i8 TagSet::i_InsertPoint(Stz* szNewName)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
  
  if (! nItems())
    return 0;

  TagArrayIter xIter = m_oTagArray.xIterator();
  Tag* pTag;

  while ((pTag = xIter.pNext()) != nullptr) {
    if (szName(pTag)->iCompare(szNewName) >= 0)
      return xIter.iArrayIndex();
  }
  
  return nItems();
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(TagSet)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
