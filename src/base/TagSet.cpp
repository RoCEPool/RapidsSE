/*============================================================================*/
/* TagSet.cpp -- name-value tag set                                           */
/*============================================================================*/

#include "BASE.h"

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
 
  m_nItems = 0;
  
  for (i8 i_Tag = 0; i_Tag < C_TAG_SET; i_Tag++) {
    m_xaTag[i_Tag].m_i_Name  = C_UNKNOWN;
    m_xaTag[i_Tag].m_i_Value = C_UNKNOWN;
    m_xaTag[i_Tag].m_eToken  = TN_NONE;
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

  i8 i_Tag = m_nItems;

  m_xaTag[i_Tag].m_i_Name  = m_xyStrings.iAppend(szName);
  m_xaTag[i_Tag].m_i_Value = C_UNKNOWN;
  m_xaTag[i_Tag].m_eToken  = TN_STRING;

  m_nItems++;

//  if (szName->bSame("<param0>"_Stz))
//    vDumpSet(true);
      
  if (! bHasName(szName)) {
    RASSERT(0);
  }
}

/*----------------------------------------------------------------------------*/
/* append name = int                                                          */
/*----------------------------------------------------------------------------*/
void TagSet::vAppend(Stz* szName, i8 iValue)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  i8 i_Tag = m_nItems;

  m_xaTag[i_Tag].m_i_Name  = m_xyStrings.iAppend(szName);
  m_xaTag[i_Tag].m_i_Value = iValue;
  m_xaTag[i_Tag].m_eToken  = TN_INTEGER;

  m_nItems++;

//  if (szName->bSame("<param0>"_Stz))
//    vDumpSet(true);
      
  if (! bHasName(szName)) {
    RASSERT(0);
  }
}

/*----------------------------------------------------------------------------*/
/* append name = int                                                          */
/*----------------------------------------------------------------------------*/
void TagSet::vAppend(Stz* szName, Object* pObject)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  i8 i_Tag = m_nItems;

  m_xaTag[i_Tag].m_i_Name  = m_xyStrings.iAppend(szName);
  m_xaTag[i_Tag].m_i_Value = (i8)pObject;
  m_xaTag[i_Tag].m_eToken  = TN_OBJECT;

  m_nItems++;

//  if (szName->bSame("<param0>"_Stz))
//    vDumpSet(true);
      
  if (! bHasName(szName)) {
    RASSERT(0);
  }
}

/*----------------------------------------------------------------------------*/
/* append name = str                                                          */
/*----------------------------------------------------------------------------*/
void TagSet::vAppend(Stz* szName, Stz* szValue)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  i8 i_Tag = m_nItems;

  m_xaTag[i_Tag].m_i_Name  = m_xyStrings.iAppend(szName);
  m_xaTag[i_Tag].m_i_Value = m_xyStrings.iAppend(szValue);
  m_xaTag[i_Tag].m_eToken  = TN_STRING;

  m_nItems++;

//  if (szName->bSame("<param0>"_Stz))
//    vDumpSet(true);
      
  if (! bHasName(szName)) {
    RASSERT(0);
  }
}

/*----------------------------------------------------------------------------*/
/* concat tagSet to cmd string                                              */
/*----------------------------------------------------------------------------*/
void TagSet::vConcat(Stz* szCmd)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
  
  for (i8 i_Tag = 0; i_Tag < m_nItems; i_Tag++) {
    
    szCmd->vConcat(" "_Stz);
    
    if (TN_INTEGER == m_xaTag[i_Tag].m_eToken) {
      
      szCmd->vCatSprint("%lld "_Stz,
             m_xaTag[i_Tag].m_i_Value
             );
      continue;
    }
    szCmd->vConcat("\""_Stz);
    szCmd->vConcat(m_xyStrings.szGet(m_xaTag[i_Tag].m_i_Value));
    szCmd->vConcat("\""_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* append tagSet to cmd string                                                */
/*----------------------------------------------------------------------------*/
void TagSet::vConcat(Stz* szCmd, i8 i_Tag)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
  
  szCmd->vConcat(" "_Stz);
  
  if (TN_INTEGER == m_xaTag[i_Tag].m_eToken) {
    
    szCmd->vCatSprint("%lld "_Stz,
                      m_xaTag[i_Tag].m_i_Value
                      );
  }
  else {
    szCmd->vConcat("\""_Stz);
    szCmd->vConcat(m_xyStrings.szGet(m_xaTag[i_Tag].m_i_Value));
    szCmd->vConcat("\""_Stz);
  }
}

/*----------------------------------------------------------------------------*/
/* content dump                                                               */
/*----------------------------------------------------------------------------*/
void TagSet::vContentDump(void)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  if (m_nItems) {
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
      else {
        vReplace(szName,pScrTagSet->iValue(i_Tag));
      }
    }
    else {
      if (TN_STRING == pScrTagSet->eToken(i_Tag)) {
        vInsert(szName,pScrTagSet->szValue(i_Tag));
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

  for (i8 i_Tag = 0; i_Tag < m_nItems; i_Tag++) {
    
    if ((! bCarriageReturns) && i_Tag)
      vTrace(", "_Stz);
    
    if (bCarriageReturns && i_Tag)
      vTrace("\n"_Stz);
    
    if (bCarriageReturns)
      vTrace("  [%02lld] %7s : "_Stz
             ,i_Tag
             ,aTN[m_xaTag[i_Tag].m_eToken]);

    if (TN_INTEGER == m_xaTag[i_Tag].m_eToken) {
      vTrace(bCarriageReturns
               ? "%16s = %lld"_Stz
               : "%s=%lld"_Stz
             
             ,m_xyStrings.szGet(m_xaTag[i_Tag].m_i_Name)
             ,m_xaTag[i_Tag].m_i_Value
              );
    }
    else {
      vTrace(bCarriageReturns
               ? "%16s = %s"_Stz
               : "%s=%s"_Stz

             ,m_xyStrings.szGet(m_xaTag[i_Tag].m_i_Name)
             ,m_xyStrings.szGet(m_xaTag[i_Tag].m_i_Value)
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
  
  for (i8 i_Tag = 0; i_Tag < m_nItems; i_Tag++) {
    vTrace("  [%lld] %s _%lld, %s _%lld (%s)\n"_Stz,
           
           i_Tag,
           
           m_xyStrings.szGet(m_xaTag[i_Tag].m_i_Name),
           m_xaTag[i_Tag].m_i_Name,
           
           m_xyStrings.szGet(m_xaTag[i_Tag].m_i_Value),
           m_xaTag[i_Tag].m_i_Value,
           
           aTN[m_xaTag[i_Tag].m_eToken],
           
           C_VARGS_END
           );
  }
}

/*----------------------------------------------------------------------------*/
/* find name                                                                  */
/*----------------------------------------------------------------------------*/
i8 TagSet::i_Find(Stz *szName)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
  
  for (i8 i_Tag = 0; i_Tag < m_nItems; i_Tag++) {
    if (m_xyStrings.szGet(m_xaTag[i_Tag].m_i_Name)->bSame(szName))
      return i_Tag;
  }
  
  return C_UNKNOWN;
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
/* add name only                                                              */
/*----------------------------------------------------------------------------*/
//void TagSet::vInsert(Stz* szName)
//{
//  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
//
//  i8 i_Tag = m_nEnames;
//
//  m_xaTag[i_Tag].m_i_Name  = m_xyStrings.iAppend(szName);
//  m_xaTag[i_Tag].m_i_Value = C_UNKNOWN;
//  m_xaTag[i_Tag].m_eToken  = TN_STRING;
//
//  m_nEnames++;
//
//  //vDumpSet();
//  if (! bHasName(szName)) {
//    RASSERT(0);
//  }
//}
/*----------------------------------------------------------------------------*/
void TagSet::vInsert(Stz* szName)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  i8 i_Tag = i_InsertPoint(szName);
  vShiftDown(i_Tag);

  m_xaTag[i_Tag].m_i_Name  = m_xyStrings.iAppend(szName);
  m_xaTag[i_Tag].m_i_Value = C_UNKNOWN;
  m_xaTag[i_Tag].m_eToken  = TN_STRING;

  m_nItems++;

//  if (szName->bSame("<param0>"_Stz))
//    vDumpSet(true);
      
  ASSERT(bHasName(szName));
}

/*----------------------------------------------------------------------------*/
/* add                                                                        */
/*----------------------------------------------------------------------------*/
//void TagSet::vInsert(Stz* szName, i8 iValue)
//{
//  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
//
//  i8 i_Tag = m_nEnames;
//
//  m_xaTag[i_Tag].m_i_Name  = m_xyStrings.iAppend(szName);
//  m_xaTag[i_Tag].m_i_Value = iValue;
//  m_xaTag[i_Tag].m_eToken  = TN_INTEGER;
//
//  m_nEnames++;
//
//  //vDumpSet();
//  if (! bHasName(szName)) {
//    RASSERT(0);
//  }
//}
/*----------------------------------------------------------------------------*/
void TagSet::vInsert(Stz* szName, i8 iValue)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  i8 i_Tag = i_InsertPoint(szName);
  vShiftDown(i_Tag);

  m_xaTag[i_Tag].m_i_Name  = m_xyStrings.iAppend(szName);
  m_xaTag[i_Tag].m_i_Value = iValue;
  m_xaTag[i_Tag].m_eToken  = TN_INTEGER;

  m_nItems++;

//  if (szName->bSame("<param0>"_Stz)) {
//    vTrace("TagSet::vInsert i : %s %lld\n"_Stz,szName,iValue);
//    vDumpSet(true);
//  }
  
  ASSERT(bHasName(szName));
}

/*----------------------------------------------------------------------------*/
/* add name and value                                                         */
/*----------------------------------------------------------------------------*/
void TagSet::vInsert(Stz* szName, Object* pValue)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  i8 i_Tag = i_InsertPoint(szName);
  vShiftDown(i_Tag);

  m_xaTag[i_Tag].m_i_Name  = m_xyStrings.iAppend(szName);
  m_xaTag[i_Tag].m_i_Value = (i8)pValue;
  m_xaTag[i_Tag].m_eToken  = TN_OBJECT;

  m_nItems++;

//  if (szName->bSame("<param0>"_Stz)) {
//
//    vTrace("TagSet::vInsert s : %s %s\n"_Stz,szName,szValue);
//    vDumpSet(true);
//  }
  ASSERT(bHasName(szName));
}


/*----------------------------------------------------------------------------*/
/* add name and value                                                         */
/*----------------------------------------------------------------------------*/
//void TagSet::vInsert(Stz* szName, Stz* szValue)
//{
//  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
//
//  i8 i_Tag = m_nEnames;
//
//  m_xaTag[i_Tag].m_i_Name  = m_xyStrings.iAppend(szName);
//  m_xaTag[i_Tag].m_i_Value = m_xyStrings.iAppend(szValue);
//  m_xaTag[i_Tag].m_eToken  = TN_STRING;
//
//  m_nEnames++;
//
//  //vDumpSet();
//  if (! bHasName(szName)) {
//    RASSERT(0);
//  }
//}
/*----------------------------------------------------------------------------*/
void TagSet::vInsert(Stz* szName, Stz* szValue)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  i8 i_Tag = i_InsertPoint(szName);
  vShiftDown(i_Tag);

  m_xaTag[i_Tag].m_i_Name  = m_xyStrings.iAppend(szName);
  m_xaTag[i_Tag].m_i_Value = m_xyStrings.iAppend(szValue);
  m_xaTag[i_Tag].m_eToken  = TN_STRING;

  m_nItems++;

//  if (szName->bSame("<param0>"_Stz)) {
//
//    vTrace("TagSet::vInsert s : %s %s\n"_Stz,szName,szValue);
//    vDumpSet(true);
//  }
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
/* is token kind an integer?                                                  */
/*----------------------------------------------------------------------------*/
bool TagSet::bIsInteger(i8 i_Tag)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
  
  eTN eToken = m_xaTag[i_Tag].m_eToken;
  
  if (TN_INTEGER == eToken)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is token kind a string?                                                    */
/*----------------------------------------------------------------------------*/
bool TagSet::bIsString(i8 i_Tag)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
  
  eTN eToken = m_xaTag[i_Tag].m_eToken;
  
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
  
  return m_nItems;
}

/*----------------------------------------------------------------------------*/
/* get name                                                                   */
/*----------------------------------------------------------------------------*/
Stz* TagSet::szName(i8 i_Tag)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
  
  Stz* szName = m_xyStrings.szGet(m_xaTag[i_Tag].m_i_Name);
  
  return szName;
}

/*----------------------------------------------------------------------------*/
/* produce names string                                                       */
/*----------------------------------------------------------------------------*/
void TagSet::vNamesString(String* pString)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  char* azStr = pString->azStr();
  UNUSED(azStr);
  char* azCrs = pString->azStr();
  
  for (i8 i_Tag = 0; i_Tag < m_nItems; i_Tag++) {
    if (i_Tag)
      StrLib::vAppendChar(&azCrs,' ');
    char* azName = szName(i_Tag)->azChar();
    StrLib::vAppendString(&azCrs,azName);
  }
}

/*----------------------------------------------------------------------------*/
/* copy source ename-set into this ename-set and replace an ename pair        */
/*----------------------------------------------------------------------------*/
void TagSet::vReplace(Stz* szName, i8 iValue)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

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
      vInsert(xOriginalTagSet.szName(i_Tag),xOriginalTagSet.iValue(i_Tag));
  }

  if (! bWasCopied)
    vInsert(szName,iValue);
}
/*----------------------------------------------------------------------------*/
//void TagSet::vReplace(Stz* szName, i8 iValue)
//{
//  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
//
//  TagSet xTagSet;
//
//  bool bWasCopied = false;
//
//  xTagSet.vCopy(this);
//  vReset();
//
//  for (i8 i_Tag = 0; i_Tag < xTagSet.nEnames(); i_Tag++) {
//
//    if (TN_STRING == xTagSet.eToken(i_Tag)) {
//      vInsert(xTagSet.szName(i_Tag),xTagSet.szValue(i_Tag));
//    }
//    else {
//      if (xTagSet.szName(i_Tag)->bSame(szName)) {
//        vInsert(xTagSet.szName(i_Tag),iValue);
//        bWasCopied = true;
//      }
//      else {
//        vInsert(xTagSet.szName(i_Tag),xTagSet.iValue(i_Tag));
//      }
//    }
//  }
//
//  if (! bWasCopied)
//    vInsert(szName,iValue);
//}

/*----------------------------------------------------------------------------*/
/* copy source ename-set into this ename-set and replace an ename pair        */
/*----------------------------------------------------------------------------*/
void TagSet::vReplace(Stz* szName, Object* pValue)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

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
      vInsert(xOriginalTagSet.szName(i_Tag),xOriginalTagSet.iValue(i_Tag));
  }

  if (! bWasCopied)
    vInsert(szName,szValue);
}
/*----------------------------------------------------------------------------*/
//void TagSet::vReplace(Stz* szName, Stz* szValue)
//{
//  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
//
//  TagSet xTagSet;
//
//  bool bWasCopied = false;
//
//  xTagSet.vCopy(this);
//  vReset();
//
//  for (i8 i_Tag = 0; i_Tag < xTagSet.nEnames(); i_Tag++) {
//
//    if (TN_STRING == xTagSet.eToken(i_Tag)) {
//
//      if (xTagSet.szName(i_Tag)->bSame(szName)) {
//        vInsert(xTagSet.szName(i_Tag),szValue);
//        bWasCopied = true;
//      }
//      else {
//        vInsert(xTagSet.szName(i_Tag),xTagSet.szValue(i_Tag));
//      }
//    }
//    else
//      vInsert(xTagSet.szName(i_Tag),xTagSet.iValue(i_Tag));
//  }
//
//  if (! bWasCopied)
//    vInsert(szName,szValue);
//}

/*----------------------------------------------------------------------------*/
/* get token kind                                                             */
/*----------------------------------------------------------------------------*/
void TagSet::vReset(void)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);

  m_nItems = 0;
  
  m_xyStrings.vReset();
}

/*----------------------------------------------------------------------------*/
/* sprint                                                                     */
/*----------------------------------------------------------------------------*/
void TagSet::vSprint(Stz* sArgs, bool bParenth, bool bNames, bool bDecode)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
  
  if (bParenth)
    sArgs->vCopy("("_Stz);
  
  for (i8 i_Tag = 0; i_Tag < m_nItems; i_Tag++) {
    
    if (i_Tag)
      sArgs->vConcat(", "_Stz);
    
    if (bNames) {
      sArgs->vConcat(szName(i_Tag));
      sArgs->vConcat("="_Stz);
    }
    
    if (bIsString(i_Tag)) {
      sArgs->vConcat(m_xyStrings.szGet(m_xaTag[i_Tag].m_i_Value));
      continue;
    }
    
    ASSERT(bIsInteger(i_Tag));

    s32 sValue;
    
    if (bIsDecodable(i_Tag))
      sValue = szValueDecoded(i_Tag);
    else
      sValue.vSprint("%lld"_Stz,iValue(i_Tag));
    sArgs->vConcat(sValue);
  }
  
  if (bParenth)
    sArgs->vConcat(")"_Stz);
}

/*----------------------------------------------------------------------------*/
/* get token kind                                                             */
/*----------------------------------------------------------------------------*/
eTN TagSet::eToken(i8 i_Tag)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
  
  eTN eToken = m_xaTag[i_Tag].m_eToken;
  
  return eToken;
}

/*----------------------------------------------------------------------------*/
/* get value                                                                  */
/*----------------------------------------------------------------------------*/
i8 TagSet::iValue(i8 i_Tag)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
  
  //Stz* szValue = m_xyStrings.szGet(m_xaTag[i_Tag].m_i_Value);
  //i8 iValue = szValue->iToI8();
  
  ASSERT(TN_INTEGER == m_xaTag[i_Tag].m_eToken);
  
  i8 iValue = m_xaTag[i_Tag].m_i_Value;
  
  return iValue;
}

/*----------------------------------------------------------------------------*/
/* get value                                                                  */
/*----------------------------------------------------------------------------*/
Stz* TagSet::szValue(i8 i_Tag)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
  
  ASSERT(TN_INTEGER != m_xaTag[i_Tag].m_eToken);
  Stz* szValue = m_xyStrings.szGet(m_xaTag[i_Tag].m_i_Value);

  return szValue;
}

/*----------------------------------------------------------------------------*/
/* get decoded value                                                          */
/*----------------------------------------------------------------------------*/
Stz* TagSet::szValueDecoded(i8 i_Tag)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
  
  ASSERT(TN_INTEGER == m_xaTag[i_Tag].m_eToken);
  Stz* szValue = "cantDecode"_Stz;

  Stz* szName = this->szName(i_Tag);

  if (szName->bSame("<crossTopo>"_Stz)) {
    i8 iValue = this->iValue(i_Tag);
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
  
  i8 i_Tag = i_Find(szName);
  
  if (C_UNKNOWN == i_Tag)
    return 0;
  //vPrint();
  ASSERT(TN_INTEGER == m_xaTag[i_Tag].m_eToken);
  i8 iValue = m_xaTag[i_Tag].m_i_Value;

  return iValue;
}

/*----------------------------------------------------------------------------*/
/* get value                                                                  */
/*----------------------------------------------------------------------------*/
Object* TagSet::pValue(i8 i_Tag)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
  
  ASSERT(TN_INTEGER == m_xaTag[i_Tag].m_eToken);
  Object* pObject = (Object*)m_xaTag[i_Tag].m_i_Value;

  return pObject;
}

/*----------------------------------------------------------------------------*/
/* get value                                                                  */
/*----------------------------------------------------------------------------*/
Object* TagSet::pValue(Stz *szName)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
  
  i8 i_Tag = i_Find(szName);
  
  if (C_UNKNOWN == i_Tag)
    return 0;
  //vPrint();
  ASSERT(TN_INTEGER == m_xaTag[i_Tag].m_eToken);
  Object* pObject = (Object*)m_xaTag[i_Tag].m_i_Value;

  return pObject;
}

/*----------------------------------------------------------------------------*/
/* get value                                                                  */
/*----------------------------------------------------------------------------*/
Stz* TagSet::szValue(Stz *szName)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
  
  i8 i_Tag = i_Find(szName);
  
  if (C_UNKNOWN == i_Tag)
    return ""_Stz;
  
  ASSERT(TN_INTEGER != m_xaTag[i_Tag].m_eToken);
  Stz* szValue = m_xyStrings.szGet(m_xaTag[i_Tag].m_i_Value);
  
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
  
  if (! m_nItems)
    return 0;
  
  for (i8 i_Scan = 0; i_Scan < m_nItems; i_Scan++)
    if (szName(i_Scan)->iCompare(szNewName) >= 0)
      return i_Scan;
  
  return m_nItems;
}


/*----------------------------------------------------------------------------*/
/* shift sub set down so as to open up i_From                                 */
/*----------------------------------------------------------------------------*/
void TagSet::vShiftDown(i8 i_From)
{
  _precondition_(_MAGIC_,CN_TAGSET,_FOOTER_);
  
  for (i8 i_Dst = m_nItems; i_Dst > i_From; i_Dst--) {
    m_xaTag[i_Dst].m_i_Name  = m_xaTag[i_Dst - 1].m_i_Name;
    m_xaTag[i_Dst].m_i_Value = m_xaTag[i_Dst - 1].m_i_Value;
    m_xaTag[i_Dst].m_eToken  = m_xaTag[i_Dst - 1].m_eToken;
  }
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(TagSet)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
