/*============================================================================*/
/* TagSet.cpp -- name-value tag set                                           */
/*============================================================================*/

#include "BASE.h"
#include "Bytes.hpp"
#include "PackedStringBuf.hpp"

static const Stz* C_BEGIN_TAGSET    = "Begin_TagSet"_Stz;
static const Stz* C_END_TAGSET      = "End_TagSet"_Stz;
static const Stz* C_BEGIN_TAGARRAY  = "Begin_TagArray"_Stz;
static const Stz* C_END_TAGARRAY    = "End_TagArray"_Stz;


static const i8 C_SIZE_BEGIN_TAGSET  = C_BEGIN_TAGSET->nLength() + 1;
static const i8 C_SIZE_END_TAGSET    = C_END_TAGSET->nLength() + 1;
static const i8 C_SIZE_BEGIN_TAGARRAY= C_BEGIN_TAGARRAY->nLength() + 1;
static const i8 C_SIZE_END_TAGARRAY  = C_END_TAGARRAY->nLength() + 1;


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
, m_pStrings(PackedStringBuf::pCreate(PackedStringBuf::TINY))
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
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  // m_oTagArray already copied above, but we need to correct
  // all string pointers used in it to use the new packed string buffer

  m_pStrings = PackedStringBuf::pCreate(PackedStringBuf::TINY);

  TagArrayIter xIter = m_oTagArray.xIterator();
  Tag* pTag;

  while ((pTag = xIter.pNext()) != nullptr) {
    // Copy the name into the new packed string buffer and set the new pointer
    // back into the tag.
    ASSERT(pTag->m_szName);
    pTag->m_szName = m_pStrings->pAdd(pTag->m_szName);

    // Copy the value, if necessary
    if (pTag->m_eToken == TN_STRING) {
      ASSERT(pTag->m_szValue);
      pTag->m_szValue = m_pStrings->pAdd(pTag->m_szValue);
    }
  }

  _checkInitialised_(CN_TAGSET);

  _postcondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
TagSet::~TagSet(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  RASSERT(!m_oLock.bIsLocked());

  PackedStringBuf::vDestroy(m_pStrings);
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
/* serialise a tagset to a byte buffer                                        */
/*----------------------------------------------------------------------------*/
bool TagSet::bSerialise(TagSet* pTagSet, Bytes* pBuf)
{
  _static_

  ASSERT(pBuf);
  ASSERT(pBuf->bIsEmpty());
  ASSERT(pBuf->i_Latter() == 0);

  ReadLocker oReadLocker(&pTagSet->m_oLock);

  // (1) The overall size of the serialised data will be filled in at the end
  i8* piSerialisedSize = reinterpret_cast<i8*>(pBuf->szLatter());
  pBuf->vTake(sizeof(i8));

  // (2)
  pBuf->iAppend(C_BEGIN_TAGSET);

  // (3) Write the array of tags
  pBuf->iAppend(C_BEGIN_TAGARRAY);

    // (4) First write the number of Tag entries in the array
    pBuf->iAppend(pTagSet->m_oTagArray.nItems());

    // (5) Now write each tag sequentially.

    TagArrayIter xIter = pTagSet->m_oTagArray.xIterator();
    Tag* pTag;
    while ((pTag = xIter.pNext()) != nullptr) {

      // Important note:
      //   The tag contains pointers to names and string values, but these are
      //   obviously not portable. So we will transform them to offsets in
      //   the string buffer.
      Tag xTagCopy = *pTag;
      i8 iOffset = pTagSet->m_pStrings->iOffsetOf(pTag->m_szName);
      xTagCopy.m_szName = (Stz*) iOffset;

      if (pTag->m_eToken == TN_STRING && pTag->m_szValue != nullptr) {
        iOffset = pTagSet->m_pStrings->iOffsetOf(pTag->m_szValue);
        xTagCopy.m_szValue = (Stz*) iOffset;
      }

      // Copy the updated tag
      pBuf->iAppend(reinterpret_cast<byte*>(&xTagCopy), sizeof(Tag));
    }

  // (6) Add a human-readable footer indicating the end of the array of tags
  pBuf->iAppend(C_END_TAGARRAY);

  // (7) Now serialise the associated strings
  PackedStringBuf::bSerialise(pTagSet->m_pStrings, pBuf);

  // (8)
  pBuf->iAppend(C_END_TAGSET);

  // Now we can write the overall serialised size
  *piSerialisedSize = pBuf->i_Latter();

  RASSERT(pBuf->iRemaining() >= 0);

  return true;
}

/*----------------------------------------------------------------------------*/
/* deserialise a tagset from a byte buffer                                    */
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

    // (7) Deserialise the string buffer
    // TagSet has already created a default one, but it may not be the right size.
    // Let the deserialisation create one.
    PackedStringBuf::vDestroy(pTagSet->m_pStrings);
    pTagSet->m_pStrings = PackedStringBuf::pDeserialise(pBuf, iPos);


  // (8) Read C_END_TAGSET
  ASSERT(pBuf->szGet(iPos)->bEqual(C_END_TAGSET));
  iPos += C_SIZE_END_TAGSET;


  // Finally, we need to correct the tag names and string values.
  // When they were serialised the string pointers were converted to offsets
  // from the beginning. We now need to reverse this process and convert the
  // offsets back to string pointers.

  TagArrayIter xIter = pTagSet->m_oTagArray.xIterator();
  Tag* pTag;

  while ((pTag = xIter.pNext()) != nullptr) {
    Stz* szName = pTagSet->m_pStrings->szAt((i8) pTag->m_szName);
    pTag->m_szName = szName;

    if (pTag->m_eToken == TN_STRING && pTag->m_szValue != nullptr) {
      Stz* szValue = pTagSet->m_pStrings->szAt((i8) pTag->m_szValue);
      pTag->m_szValue = szValue;
    }
  }

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

  {
    WriteLocker oWriteLocker(&m_oLock);
    Tag xTag{m_pStrings->pAdd(szName), (Stz *) nullptr, TN_STRING};
    m_oTagArray.vAdd(xTag);
  }

  ASSERT(bHasName(szName));
}

/*----------------------------------------------------------------------------*/
/* append name = int                                                          */
/*----------------------------------------------------------------------------*/
void TagSet::vAppend(Stz* szName, i8 iValue)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  {
    WriteLocker oWriteLocker(&m_oLock);
    Tag xTag{m_pStrings->pAdd(szName), iValue, TN_INTEGER};
    m_oTagArray.vAdd(xTag);
  }

  ASSERT(bHasName(szName));
}

/*----------------------------------------------------------------------------*/
/* append name = int                                                          */
/*----------------------------------------------------------------------------*/
void TagSet::vAppend(Stz* szName, Object* pObject)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  {
    WriteLocker oWriteLocker(&m_oLock);
    Tag xTag{m_pStrings->pAdd(szName), (i8) pObject, TN_OBJECT};
    m_oTagArray.vAdd(xTag);
  }

  ASSERT(bHasName(szName));
}

/*----------------------------------------------------------------------------*/
/* append name = str                                                          */
/*----------------------------------------------------------------------------*/
void TagSet::vAppend(Stz* szName, Stz* szValue)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  {
    WriteLocker oWriteLocker(&m_oLock);
    Tag xTag{m_pStrings->pAdd(szName), m_pStrings->pAdd(szValue), TN_STRING};
    m_oTagArray.vAdd(xTag);
  }

  ASSERT(bHasName(szName));
}

/*----------------------------------------------------------------------------*/
/* concat tagSet to cmd string                                              */
/*----------------------------------------------------------------------------*/
void TagSet::vConcat(String* pCmd)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  ReadLocker oReadLocker(&m_oLock);

  TagArrayIter xIter = m_oTagArray.xIterator();
  Tag* pTag;

  while ((pTag = xIter.pNext()) != nullptr) {

    pCmd->vConcat(" "_Stz);
    
    if (TN_INTEGER == pTag->m_eToken) {

      pCmd->vCatSprint("%lld "_Stz,
                       pTag->m_iValue
             );
      continue;
    }
    ASSERT(TN_STRING == pTag->m_eToken);
    pCmd->vConcat("\""_Stz);
    pCmd->vConcat(pTag->m_szValue);
    pCmd->vConcat("\""_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* append tagSet to cmd string                                                */
/*----------------------------------------------------------------------------*/
void TagSet::vConcat(String* pCmd, i8 i_Tag)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  ReadLocker oReadLocker(&m_oLock);

  ASSERT(i_Tag >= 0);
  ASSERT(i_Tag < m_oTagArray.nItems());

  Tag* pTag = m_oTagArray.pGet(i_Tag);
  
  pCmd->vConcat(" "_Stz);
  
  if (TN_INTEGER == pTag->m_eToken) {
    
    pCmd->vCatSprint("%lld "_Stz,
                     pTag->m_iValue
                      );
  }
  else {
    ASSERT(TN_STRING == pTag->m_eToken);
    pCmd->vConcat("\""_Stz);
    pCmd->vConcat(pTag->m_szValue);
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

  WriteLocker oWriteLockerThis(&m_oLock);
  ReadLocker  oReadLockerArg(&pScrTagSet->m_oLock);

  for (i8 i_Tag = 0; i_Tag < pScrTagSet->nItemsNL(); i_Tag++) {
    
    Stz* szName = pScrTagSet->szName(i_Tag);
    
    if (bHasNameNL(szName)) {
      if (TN_STRING == pScrTagSet->eToken(i_Tag)) {
        vReplaceNL(szName,pScrTagSet->szValue(i_Tag));
      }
      else
      if (TN_OBJECT == pScrTagSet->eToken(i_Tag)) {
        vReplaceNL(szName,pScrTagSet->pValue(i_Tag));
      }
      else {
        vReplaceNL(szName,pScrTagSet->iValue(i_Tag));
      }
    }
    else {
      if (TN_STRING == pScrTagSet->eToken(i_Tag)) {
        vInsertNL(szName,pScrTagSet->szValue(i_Tag));
      }
      else
      if (TN_OBJECT == pScrTagSet->eToken(i_Tag)) {
        vInsertNL(szName,pScrTagSet->pValue(i_Tag));
      }
      else {
        vInsertNL(szName,pScrTagSet->iValue(i_Tag));
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/* delete a tag matching a given name                                         */
/*----------------------------------------------------------------------------*/
void TagSet::vDelete(Stz* szName)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  WriteLocker oWriteLocker(&m_oLock);

  TagArrayIter xIter = m_oTagArray.xIterator();
  Tag* pTag;

  while ((pTag = xIter.pNext()) != nullptr) {
    if (pTag->m_szName->bSame(szName)) {
      // Remove the strings
      m_pStrings->vRemove(pTag->m_szName);
      if (pTag->m_eToken == TN_STRING && pTag->m_szValue != nullptr) {
        m_pStrings->vRemove(pTag->m_szValue);
      }

      // Remove the token
      i8 iIdx = xIter.iArrayIndex();
      xIter.pPrevious();
      m_oTagArray.vRemove(iIdx);
    }
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

  ReadLocker oReadLocker(&m_oLock);

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
             
             ,pTag->m_szName
             ,pTag->m_iValue
              );
    }
    else {
      ASSERT(TN_STRING == pTag->m_eToken);
      vTrace(bCarriageReturns
               ? "%16s = %s"_Stz
               : "%s=%s"_Stz

             ,pTag->m_szName
             ,pTag->m_szValue
              );
    }
  }
  
  
  if (bCarriageReturns)
    vTrace("\n"_Stz);
  
  vTrace("BYTES=%lld"_Stz,m_pStrings->iTotalArea());

  if (bCarriageReturns)
    vTrace("\n"_Stz);

  vTrace("}\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void TagSet::vDumpSetDetailed(void)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  ReadLocker oReadLocker(&m_oLock);

  TagArrayIter xIter = m_oTagArray.xIterator();
  Tag* pTag;
  i8 i = -1;

  while ((pTag = xIter.pNext()) != nullptr) {
    vTrace("  [%lld] %s _%lld, %s (%s)\n"_Stz,

           ++i,

           pTag->m_szName,
           xIter.iArrayIndex(),

           pTag->m_szValue,

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
  ReadLocker oReadLocker(&m_oLock);

  TagSet* pNewTagSet = new TagSet(*this);
  return pNewTagSet;
}

/*----------------------------------------------------------------------------*/
/* find tag by name                                                           */
/*----------------------------------------------------------------------------*/
i8 TagSet::i_Find(Stz *szName)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  ReadLocker oReadLocker(&m_oLock);
  return i_FindNL(szName);
}

/*----------------------------------------------------------------------------*/
/* find tag by name                                                           */
/*----------------------------------------------------------------------------*/
i8 TagSet::i_FindNL(Stz *szName)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  ASSERT(m_oLock.bIsLocked());

  TagArrayIter xIter = m_oTagArray.xIterator();
  Tag* pTag;

  while ((pTag = xIter.pNext()) != nullptr) {
    if (pTag->m_szName->bSame(szName))
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

  ReadLocker oReadLocker(&m_oLock);
  return pFindNL(szName);
}

/*----------------------------------------------------------------------------*/
/* find tag by name                                                           */
/*----------------------------------------------------------------------------*/
Tag* TagSet::pFindNL(Stz *szName)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  ASSERT(m_oLock.bIsLocked());

  TagArrayIter xIter = m_oTagArray.xIterator();
  Tag* pTag;

  while ((pTag = xIter.pNext()) != nullptr) {
    if (pTag->m_szName->bSame(szName))
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

  ReadLocker oReadLocker(&m_oLock);
  return bHasNameNL(szName);
}

/*----------------------------------------------------------------------------*/
/* test if ename-set has a pair with given name                               */
/*----------------------------------------------------------------------------*/
bool TagSet::bHasNameNL(Stz *szName)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  ASSERT(m_oLock.bIsLocked());

  i8 i_Tag = i_FindNL(szName);

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

  WriteLocker oWriteLocker(&m_oLock);
  vInsertNL(szName);

  ASSERT(bHasNameNL(szName));
}

/*----------------------------------------------------------------------------*/
/* Insert the tag in alphabetical order according to the tag name             */
/*----------------------------------------------------------------------------*/
void TagSet::vInsertNL(Stz* szName)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  ASSERT(m_oLock.bIsWriteLocked());

  i8 i_Tag = i_InsertPoint(szName);

  Tag xTag{m_pStrings->pAdd(szName), (Stz *) nullptr, TN_STRING};
  m_oTagArray.vInsert(xTag, i_Tag);
}

/*----------------------------------------------------------------------------*/
/* Insert the tag in alphabetical order according to the tag name             */
/*----------------------------------------------------------------------------*/
void TagSet::vInsert(Stz* szName, i8 iValue)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  WriteLocker oWriteLocker(&m_oLock);
  vInsertNL(szName, iValue);

  ASSERT(bHasNameNL(szName));
}
/*----------------------------------------------------------------------------*/
/* Insert the tag in alphabetical order according to the tag name             */
/*----------------------------------------------------------------------------*/
void TagSet::vInsertNL(Stz* szName, i8 iValue)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
  ASSERT(m_oLock.bIsWriteLocked());

  i8 i_Tag = i_InsertPoint(szName);

  Tag xTag{m_pStrings->pAdd(szName), iValue, TN_INTEGER};
  m_oTagArray.vInsert(xTag, i_Tag);
}

/*----------------------------------------------------------------------------*/
/* Insert the tag in alphabetical order according to the tag name             */
/*----------------------------------------------------------------------------*/
void TagSet::vInsert(Stz* szName, Object* pValue)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  WriteLocker oWriteLocker(&m_oLock);
  vInsertNL(szName, pValue);

  ASSERT(bHasNameNL(szName));
}

/*----------------------------------------------------------------------------*/
/* Insert the tag in alphabetical order according to the tag name             */
/*----------------------------------------------------------------------------*/
void TagSet::vInsertNL(Stz* szName, Object* pValue)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
  ASSERT(m_oLock.bIsWriteLocked());

  i8 i_Tag = i_InsertPoint(szName);

  Tag xTag{m_pStrings->pAdd(szName), (i8) pValue, TN_OBJECT};
  m_oTagArray.vInsert(xTag, i_Tag);
}

/*----------------------------------------------------------------------------*/
/* Insert the tag in alphabetical order according to the tag name             */
/*----------------------------------------------------------------------------*/
void TagSet::vInsert(Stz* szName, Stz* szValue)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  WriteLocker oWriteLocker(&m_oLock);
  vInsertNL(szName, szValue);

  ASSERT(bHasNameNL(szName));
}

/*----------------------------------------------------------------------------*/
/* Insert the tag in alphabetical order according to the tag name             */
/*----------------------------------------------------------------------------*/
void TagSet::vInsertNL(Stz* szName, Stz* szValue) {
  _precondition_(_MAGIC_, CN_TAGSET, _FOOTER_);
  ASSERT(m_oLock.bIsWriteLocked());

  i8 i_Tag = i_InsertPoint(szName);

  Stz *szAddedName = m_pStrings->pAdd(szName);
  Stz *szAddedValue = m_pStrings->pAdd(szValue);
  Tag xTag{szAddedName, szAddedValue, TN_STRING};
  m_oTagArray.vInsert(xTag, i_Tag);
}
/*----------------------------------------------------------------------------*/
/* is token a decodable enum?                                                 */
/*----------------------------------------------------------------------------*/
bool TagSet::bIsDecodable(i8 i_Tag)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  ReadLocker oReadLocker(&m_oLock);

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

  ReadLocker oReadLocker(&m_oLock);

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

  ReadLocker oReadLocker(&m_oLock);

  ASSERT(i_Tag >= 0);
  ASSERT(i_Tag < m_oTagArray.nItems());
  
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
  ASSERT(pTag);

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

  ReadLocker oReadLocker(&m_oLock);

  ASSERT(i_Tag >= 0);
  ASSERT(i_Tag < m_oTagArray.nItems());

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
  ASSERT(pTag);

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

  ReadLocker oReadLocker(&m_oLock);

  ASSERT(i_Tag >= 0);
  ASSERT(i_Tag < m_oTagArray.nItems());
  
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
  ASSERT(pTag);

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

  ReadLocker oReadLocker(&m_oLock);

  return m_oTagArray.nItems();
}

/*----------------------------------------------------------------------------*/
/* get nItems                                                                 */
/*----------------------------------------------------------------------------*/
i8 TagSet::nItemsNL(void)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  ASSERT(m_oLock.bIsLocked());

  return m_oTagArray.nItems();
}

/*----------------------------------------------------------------------------*/
/* get name                                                                   */
/*----------------------------------------------------------------------------*/
Stz* TagSet::szName(i8 i_Tag)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  ReadLocker oReadLocker(&m_oLock);

  ASSERT(i_Tag >= 0);
  ASSERT(i_Tag < m_oTagArray.nItems());

  Tag* pTag = m_oTagArray.pGet(i_Tag);
  return pTag->m_szName;
}

/*----------------------------------------------------------------------------*/
/* get name                                                                   */
/*----------------------------------------------------------------------------*/
Stz* TagSet::szName(Tag* pTag)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
  ASSERT(pTag);

  return pTag->m_szName;
}

/*----------------------------------------------------------------------------*/
/* produce names string                                                       */
/*----------------------------------------------------------------------------*/
void TagSet::vNamesString(String* pString)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  ReadLocker oReadLocker(&m_oLock);

  char* azCrs = pString->azStr();
  const char* azDstLastChar = pString->azStr() + pString->nAreaBytes() - 1;

  TagArrayIter xIter = m_oTagArray.xIterator();
  Tag* pTag;

  while ((pTag = xIter.pNext()) != nullptr) {
    if (xIter.iArrayIndex())
      StrLib::vAppendChar(&azCrs, azDstLastChar, ' ');
    char* azName = pTag->m_szName->azChar();
    StrLib::vAppendString(&azCrs, azDstLastChar, azName);
  }
}

/*----------------------------------------------------------------------------*/
/* copy source ename-set into this ename-set and replace an ename pair        */
/*----------------------------------------------------------------------------*/
void TagSet::vReplace(Stz* szName, i8 iValue) {
  _precondition_(_MAGIC_, CN_TAGSET, _FOOTER_);

  WriteLocker oWriteLocker(&m_oLock);
  vReplaceNL(szName, iValue);
}

/*----------------------------------------------------------------------------*/
/* copy source ename-set into this ename-set and replace an ename pair        */
/*----------------------------------------------------------------------------*/
void TagSet::vReplaceNL(Stz* szName, i8 iValue)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  ASSERT(m_oLock.bIsWriteLocked());

  bool bWasCopied = false;

  TagArrayIter xIter = m_oTagArray.xIterator();
  Tag* pTag;

  while ((pTag = xIter.pNext()) != nullptr) {

    if (pTag->m_szName->bSame(szName)) {
      if (pTag->m_eToken == TN_STRING && pTag->m_szValue != nullptr) {
        m_pStrings->vRemove(pTag->m_szValue);
      }
      // What if the prior tag value is an object?

      // Update the value on this item
      pTag->m_iValue = iValue;
      pTag->m_eToken = TN_INTEGER;
      bWasCopied = true;
      continue;
    }
  }

  if (! bWasCopied) {
    vInsertNL(szName, iValue);
  }
}

/*----------------------------------------------------------------------------*/
/* copy source ename-set into this ename-set and replace an ename pair        */
/*----------------------------------------------------------------------------*/
void TagSet::vReplace(Stz* szName, Object* pValue)
{
  _precondition_(_MAGIC_, CN_TAGSET, _FOOTER_);

  WriteLocker oWriteLocker(&m_oLock);
  vReplaceNL(szName, pValue);
}

/*----------------------------------------------------------------------------*/
/* copy source ename-set into this ename-set and replace an ename pair        */
/*----------------------------------------------------------------------------*/
void TagSet::vReplaceNL(Stz* szName, Object* pValue)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  ASSERT(m_oLock.bIsWriteLocked());

  bool bWasCopied = false;

  TagArrayIter xIter = m_oTagArray.xIterator();
  Tag* pTag;

  while ((pTag = xIter.pNext()) != nullptr) {

    if (pTag->m_szName->bSame(szName)) {
      if (pTag->m_eToken == TN_STRING && pTag->m_szValue != nullptr) {
        m_pStrings->vRemove(pTag->m_szValue);
      }
      // What if the prior tag value is an object?

      // Update the value on this item
      pTag->m_pValue = pValue;
      pTag->m_eToken = TN_OBJECT;
      bWasCopied = true;
      continue;
    }
  }

  if (! bWasCopied) {
    vInsertNL(szName, pValue);
  }
}

/*----------------------------------------------------------------------------*/
/* copy source ename-set into this ename-set and replace an ename pair        */
/*----------------------------------------------------------------------------*/
void TagSet::vReplace(Stz* szName, Stz* szValue)
{
  _precondition_(_MAGIC_, CN_TAGSET, _FOOTER_);

  WriteLocker oWriteLocker(&m_oLock);
  vReplaceNL(szName, szValue);
}

/*----------------------------------------------------------------------------*/
/* copy source ename-set into this ename-set and replace an ename pair        */
/*----------------------------------------------------------------------------*/
void TagSet::vReplaceNL(Stz* szName, Stz* szValue)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  ASSERT(m_oLock.bIsWriteLocked());

  bool bWasCopied = false;

  TagArrayIter xIter = m_oTagArray.xIterator();
  Tag* pTag;

  while ((pTag = xIter.pNext()) != nullptr) {

    if (pTag->m_szName->bSame(szName)) {
      if (pTag->m_eToken == TN_STRING && pTag->m_szValue != nullptr) {
        m_pStrings->vRemove(pTag->m_szValue);
      }
      // What if the prior tag value is an object?

      // Update the value on this item
      pTag->m_szValue = m_pStrings->pAdd(szValue);
      pTag->m_eToken = TN_STRING;
      bWasCopied = true;
      continue;
    }
  }

  if (! bWasCopied) {
    vInsertNL(szName, szValue);
  }
}

/*----------------------------------------------------------------------------*/
/* get token kind                                                             */
/*----------------------------------------------------------------------------*/
void TagSet::vReset(void)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  WriteLocker oWriteLocker(&m_oLock);

  m_oTagArray.vReset();
  m_pStrings->vReset();
}

/*----------------------------------------------------------------------------*/
/* sprint                                                                     */
/*----------------------------------------------------------------------------*/
void TagSet::vSprint(String* psArgs, bool bParenth, bool bNames, bool bDecode)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
  UNUSED(bDecode);

  ReadLocker oReadLocker(&m_oLock);

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
      psArgs->vConcat(pTag->m_szValue);
      continue;
    }
    
    ASSERT(bIsInteger(pTag));

    s32 sValue;
    
    if (bIsDecodable(xIter.iArrayIndex()))
      sValue = szValueDecoded(xIter.iArrayIndex());
    else
      sValue.vSprint("%lld"_Stz, pTag->m_iValue);
    psArgs->vConcat(sValue);
  }
  
  if (bParenth)
    psArgs->vConcat(")"_Stz);
}

/*----------------------------------------------------------------------------*/
/* get token kind                                                             */
/*----------------------------------------------------------------------------*/
eTN TagSet::eToken(i8 i_Tag)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
  ReadLocker oReadLocker(&m_oLock);

  eTN eToken = m_oTagArray.pGet(i_Tag)->m_eToken;
  
  return eToken;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool TagSet::bSame(TagSet* pTagSet_arg)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  ReadLocker oReadLockerThis(&m_oLock);
  ReadLocker oReadLockerArg(&pTagSet_arg->m_oLock);

  i8 nTags = this->nItemsNL();
  i8 nTags_arg = pTagSet_arg->nItemsNL();

  if (nTags != nTags_arg)
    return false;
  
  for (i8 i_Tag = 0; i_Tag < nTags; i_Tag++) {
    
    Stz* szName = this->szName(i_Tag);
    i8 i_Tag_arg = pTagSet_arg->i_FindNL(szName);
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

  ReadLocker oReadLocker(&m_oLock);

  if (! nItemsNL()) {
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

      psStr->vConcat(pTag->m_szName);
      psStr->vConcat("="_Stz);
    }

    if (bIsString(pTag)) {
      psStr->vConcat(pTag->m_szValue);
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

  ReadLocker oReadLocker(&m_oLock);

  i8 iValue = C_UNKNOWN;

  Tag* pTag = m_oTagArray.pGet(i_Tag);
  ASSERT(pTag);

  if (TN_INTEGER == pTag->m_eToken) {
    iValue = pTag->m_iValue;
  }
  else {
    Stz* szValue = pTag->m_szValue;
    if (szValue) {
      iValue = szValue->iToI8();
    } else {
      iValue = 0;
    }
  }
  return iValue;
}

/*----------------------------------------------------------------------------*/
/* get value                                                                  */
/*----------------------------------------------------------------------------*/
Stz* TagSet::szValue(i8 i_Tag)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  ReadLocker oReadLocker(&m_oLock);

  Tag* pTag = m_oTagArray.pGet(i_Tag);
  ASSERT(pTag);
  ASSERT(TN_INTEGER != pTag->m_eToken);
  return pTag->m_szValue;
}

/*----------------------------------------------------------------------------*/
/* get decoded value                                                          */
/*----------------------------------------------------------------------------*/
Stz* TagSet::szValueDecoded(i8 i_Tag)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  ReadLocker oReadLocker(&m_oLock);

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

  ReadLocker oReadLocker(&m_oLock);

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

  ReadLocker oReadLocker(&m_oLock);

  Tag* pTag = pFindNL(szName);
  if (!pTag) {
    return 0;
  }

  ASSERT(TN_INTEGER == pTag->m_eToken);
  i8 iValue = pTag->m_iValue;

  return iValue;
}

/*----------------------------------------------------------------------------*/
/* get value                                                                  */
/*----------------------------------------------------------------------------*/
i8 TagSet::iValue(Tag* pTag)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  ReadLocker oReadLocker(&m_oLock);

  ASSERT(TN_INTEGER == pTag->m_eToken);
  return pTag->m_iValue;
}

/*----------------------------------------------------------------------------*/
/* get value                                                                  */
/*----------------------------------------------------------------------------*/
Object* TagSet::pValue(i8 i_Tag)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  ReadLocker oReadLocker(&m_oLock);

  ASSERT(TN_OBJECT == m_oTagArray.pGet(i_Tag)->m_eToken);
  Object* pObject = m_oTagArray.pGet(i_Tag)->m_pValue;

  return pObject;
}

/*----------------------------------------------------------------------------*/
/* get value                                                                  */
/*----------------------------------------------------------------------------*/
Object* TagSet::pValue(Tag* pTag)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  ReadLocker oReadLocker(&m_oLock);

  ASSERT(TN_OBJECT == pTag->m_eToken);
  Object* pObject = pTag->m_pValue;

  return pObject;
}

/*----------------------------------------------------------------------------*/
/* get value                                                                  */
/*----------------------------------------------------------------------------*/
Object* TagSet::pValue(Stz *szName)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  ReadLocker oReadLocker(&m_oLock);

  Tag* pTag = pFindNL(szName);
  if (!pTag) {
    return nullptr;
  }

  ASSERT(TN_OBJECT == pTag->m_eToken);
  return pTag->m_pValue;
}

/*----------------------------------------------------------------------------*/
/* get value                                                                  */
/*----------------------------------------------------------------------------*/
Stz* TagSet::szValue(Stz *szName)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  ReadLocker oReadLocker(&m_oLock);

  Tag* pTag = pFindNL(szName);
  if (!pTag) {
    return ""_Stz;
  }

  ASSERT(TN_INTEGER != pTag->m_eToken);
  return pTag->m_szValue;
}

/*----------------------------------------------------------------------------*/
/* get value                                                                  */
/*----------------------------------------------------------------------------*/
Stz* TagSet::szValue(Tag* pTag)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  ReadLocker oReadLocker(&m_oLock);

  ASSERT(TN_INTEGER != pTag->m_eToken);
  return pTag->m_szValue;
}

/*----------------------------------------------------------------------------*/
/* Validate the tag set                                                       */
/*----------------------------------------------------------------------------*/
void TagSet::vValidate()
{
  TagArrayIter xIter = m_oTagArray.xIterator();
  Tag* pTag;

  while ((pTag = xIter.pNext()) != nullptr) {
    RASSERT(pTag->m_szName);
    RASSERT(m_pStrings->bContainsAddr(pTag->m_szName));

    if (pTag->m_eToken == TN_STRING) {
      RASSERT(pTag->m_szValue);
      RASSERT(m_pStrings->bContainsAddr(pTag->m_szValue));

    } else if (pTag->m_eToken == TN_OBJECT) {
      RASSERT(pTag->m_pValue);
      Object* pObject = pTag->m_pValue;
      pObject->vAssertHeader();
    }
  }
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

  ASSERT(m_oLock.bIsWriteLocked());

  i8 iNumItems = nItemsNL();
  if (! iNumItems)
    return 0;

  TagArrayIter xIter = m_oTagArray.xIterator();
  Tag* pTag;

  while ((pTag = xIter.pNext()) != nullptr) {
    if (szName(pTag)->iCompare(szNewName) >= 0)
      return xIter.iArrayIndex();
  }
  
  return iNumItems;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(TagSet)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
