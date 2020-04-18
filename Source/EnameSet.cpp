/*============================================================================*/
/* EnameSet.cpp -- ename-value set                                            */
/*============================================================================*/

#include "Gadgets.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
EnameSet* Gadgets::_pEnameSet(void* aAddr)
{
  EnameSet* pEnameSet = reinterpret_cast<EnameSet*>(aAddr);
  
  RASSERT_TYPE(pEnameSet,CN_ENMSET);
  
  return pEnameSet;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* EnameSet::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_ENMSET);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void EnameSet::operator delete(void* aAddr)
{
  Object* pObject = reinterpret_cast<Object*>(aAddr);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EnameSet::EnameSet(void)
: Object(CN_ENMSET)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
 
  m_nEnames = 0;
  
  for (i8 i_Eno = 0; i_Eno < C_ENAME_SET; i_Eno++) {
    m_xEname[i_Eno].m_i_Name  = C_UNKNOWN;
    m_xEname[i_Eno].m_i_Value = C_UNKNOWN;
    m_xEname[i_Eno].m_eToken  = TN_NONE;
  }
  
  _checkInitialised_(CN_ENMSET);
  
  _postcondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
EnameSet::~EnameSet(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append name only                                                           */
/*----------------------------------------------------------------------------*/
void EnameSet::vAppend(Stz* szName)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);

  i8 i_Ename = m_nEnames;

  m_xEname[i_Ename].m_i_Name  = m_xyStrings.iAppend(szName);
  m_xEname[i_Ename].m_i_Value = C_UNKNOWN;
  m_xEname[i_Ename].m_eToken  = TN_STRING;

  m_nEnames++;

  //vDumpSet();
  if (! bHasName(szName)) {
    RASSERT(0);
  }
}

/*----------------------------------------------------------------------------*/
/* append name = int                                                          */
/*----------------------------------------------------------------------------*/
void EnameSet::vAppend(Stz* szName, i8 iValue)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);

  i8 i_Ename = m_nEnames;

  m_xEname[i_Ename].m_i_Name  = m_xyStrings.iAppend(szName);
  m_xEname[i_Ename].m_i_Value = iValue;
  m_xEname[i_Ename].m_eToken  = TN_INTEGER;

  m_nEnames++;

  //vDumpSet();
  if (! bHasName(szName)) {
    RASSERT(0);
  }
}

/*----------------------------------------------------------------------------*/
/* append name = str                                                          */
/*----------------------------------------------------------------------------*/
void EnameSet::vAppend(Stz* szName, Stz* szValue)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);

  i8 i_Ename = m_nEnames;

  m_xEname[i_Ename].m_i_Name  = m_xyStrings.iAppend(szName);
  m_xEname[i_Ename].m_i_Value = m_xyStrings.iAppend(szValue);
  m_xEname[i_Ename].m_eToken  = TN_STRING;

  m_nEnames++;

  //vDumpSet();
  if (! bHasName(szName)) {
    RASSERT(0);
  }
}

/*----------------------------------------------------------------------------*/
/* concat enameset to cmd string                                              */
/*----------------------------------------------------------------------------*/
void EnameSet::vConcat(Stz* szCmd)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
  
  for (i8 i_Ename = 0; i_Ename < m_nEnames; i_Ename++) {
    
    szCmd->vConcat(" "_Stz);
    
    if (TN_INTEGER == m_xEname[i_Ename].m_eToken) {
      
      szCmd->vCatSprint("%lld "_Stz,
             m_xEname[i_Ename].m_i_Value
             );
      continue;
    }
    
    szCmd->vConcat(m_xyStrings.szGet(m_xEname[i_Ename].m_i_Value));
  }
}

/*----------------------------------------------------------------------------*/
/* append enameset to cmd string                                              */
/*----------------------------------------------------------------------------*/
void EnameSet::vConcat(Stz* szCmd, i8 i_Ename)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
  
  szCmd->vConcat(" "_Stz);
  
  if (TN_INTEGER == m_xEname[i_Ename].m_eToken) {
    
    szCmd->vCatSprint("%lld "_Stz,
                      m_xEname[i_Ename].m_i_Value
                      );
  }
  else
    szCmd->vConcat(m_xyStrings.szGet(m_xEname[i_Ename].m_i_Value));
}

/*----------------------------------------------------------------------------*/
/* content dump                                                               */
/*----------------------------------------------------------------------------*/
void EnameSet::vContentDump(void)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
  
  vTrace("\n"_Stz);
  vTrace("Ename Set\n"_Stz);
  vTrace("---------\n"_Stz);
  
  vDumpSet();
}

/*----------------------------------------------------------------------------*/
/* copy source ename-set into this ename-set                                  */
/*----------------------------------------------------------------------------*/
void EnameSet::vCopy(EnameSet* pScrEnameSet)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
  
  for (i8 i_Ename = 0; i_Ename < pScrEnameSet->nEnames(); i_Ename++) {
    
    Stz* szName = pScrEnameSet->szName(i_Ename);
    
    if (bHasName(szName)) {
      if (TN_STRING == pScrEnameSet->eToken(i_Ename)) {
        vReplace(szName,pScrEnameSet->szValue(i_Ename));
      }
      else {
        vReplace(szName,pScrEnameSet->iValue(i_Ename));
      }
    }
    else {
      if (TN_STRING == pScrEnameSet->eToken(i_Ename)) {
        vInsert(szName,pScrEnameSet->szValue(i_Ename));
      }
      else {
        vInsert(szName,pScrEnameSet->iValue(i_Ename));
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/* copy source ename-set into this ename-set and replace an ename pair        */
/*----------------------------------------------------------------------------*/
void EnameSet::vDelete(Stz* szName)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
  
  EnameSet xEnameSet;
  
  bool bWasCopied = false;
  
  xEnameSet.vCopy(this);
  vReset();
  
  for (i8 i_Ename = 0; i_Ename < xEnameSet.nEnames(); i_Ename++) {
    
    if (xEnameSet.szName(i_Ename)->bSame(szName))
      continue;
    
    if (TN_STRING == xEnameSet.eToken(i_Ename))
      vInsert(xEnameSet.szName(i_Ename),xEnameSet.szValue(i_Ename));
    else
      vInsert(xEnameSet.szName(i_Ename),xEnameSet.iValue(i_Ename));
  }
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void EnameSet::vDumpSet(bool bCarriageReturns)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
  
  vTrace("{ "_Stz);
  if (bCarriageReturns)
    vTrace("\n"_Stz);

  for (i8 i_Ename = 0; i_Ename < m_nEnames; i_Ename++) {
    
    if (i_Ename)
      vTrace(", "_Stz);
    
    if (i_Ename && bCarriageReturns)
      vTrace("\n  "_Stz);

    if (TN_INTEGER == m_xEname[i_Ename].m_eToken) {
      
      vTrace("%s=%lld"_Stz,
             
             m_xyStrings.szGet(m_xEname[i_Ename].m_i_Name),
             m_xEname[i_Ename].m_i_Value,
             
             C_VARGS_END
             );
      continue;
    }
    
    vTrace("%s=%s"_Stz,
    
           m_xyStrings.szGet(m_xEname[i_Ename].m_i_Name),
           m_xyStrings.szGet(m_xEname[i_Ename].m_i_Value),
           
           C_VARGS_END
           );
  }
 
  if (bCarriageReturns)
    vTrace("\n}\n"_Stz);
  else
    vTrace(" }\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void EnameSet::vDumpSetDetailed(void)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
  
  for (i8 i_Ename = 0; i_Ename < m_nEnames; i_Ename++) {
    vTrace("  [%lld] %s _%lld, %s _%lld (%s)\n"_Stz,
           
           i_Ename,
           
           m_xyStrings.szGet(m_xEname[i_Ename].m_i_Name),
           m_xEname[i_Ename].m_i_Name,
           
           m_xyStrings.szGet(m_xEname[i_Ename].m_i_Value),
           m_xEname[i_Ename].m_i_Value,
           
           aTN[m_xEname[i_Ename].m_eToken],
           
           C_VARGS_END
           );
  }
}

/*----------------------------------------------------------------------------*/
/* get nEnames                                                                */
/*----------------------------------------------------------------------------*/
i8 EnameSet::nEnames(void)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
  
  return m_nEnames;
}

/*----------------------------------------------------------------------------*/
/* find name                                                                  */
/*----------------------------------------------------------------------------*/
i8 EnameSet::i_Find(Stz *szName)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
  
  for (i8 i_Ename = 0; i_Ename < m_nEnames; i_Ename++) {
    if (m_xyStrings.szGet(m_xEname[i_Ename].m_i_Name)->bSame(szName))
      return i_Ename;
  }
  
  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* test if ename-set has a pair with given name                               */
/*----------------------------------------------------------------------------*/
bool EnameSet::bHasName(Stz *szName)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
  
  i8 i_Ename = i_Find(szName);
  
  if (C_UNKNOWN == i_Ename)
    return false;
  else
    return true;
}

/*----------------------------------------------------------------------------*/
/* add name only                                                              */
/*----------------------------------------------------------------------------*/
//void EnameSet::vInsert(Stz* szName)
//{
//  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
//
//  i8 i_Ename = m_nEnames;
//
//  m_xEname[i_Ename].m_i_Name  = m_xyStrings.iAppend(szName);
//  m_xEname[i_Ename].m_i_Value = C_UNKNOWN;
//  m_xEname[i_Ename].m_eToken  = TN_STRING;
//
//  m_nEnames++;
//
//  //vDumpSet();
//  if (! bHasName(szName)) {
//    RASSERT(0);
//  }
//}
/*----------------------------------------------------------------------------*/
void EnameSet::vInsert(Stz* szName)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);

  i8 i_Ename = i_InsertPoint(szName);
  vShiftDown(i_Ename);

  m_xEname[i_Ename].m_i_Name  = m_xyStrings.iAppend(szName);
  m_xEname[i_Ename].m_i_Value = C_UNKNOWN;
  m_xEname[i_Ename].m_eToken  = TN_STRING;

  m_nEnames++;

  //vDumpSet();
  ASSERT(bHasName(szName));
}

/*----------------------------------------------------------------------------*/
/* add                                                                        */
/*----------------------------------------------------------------------------*/
//void EnameSet::vInsert(Stz* szName, i8 iValue)
//{
//  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
//
//  i8 i_Ename = m_nEnames;
//
//  m_xEname[i_Ename].m_i_Name  = m_xyStrings.iAppend(szName);
//  m_xEname[i_Ename].m_i_Value = iValue;
//  m_xEname[i_Ename].m_eToken  = TN_INTEGER;
//
//  m_nEnames++;
//
//  //vDumpSet();
//  if (! bHasName(szName)) {
//    RASSERT(0);
//  }
//}
/*----------------------------------------------------------------------------*/
void EnameSet::vInsert(Stz* szName, i8 iValue)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);

  i8 i_Ename = i_InsertPoint(szName);
  vShiftDown(i_Ename);

  m_xEname[i_Ename].m_i_Name  = m_xyStrings.iAppend(szName);
  m_xEname[i_Ename].m_i_Value = iValue;
  m_xEname[i_Ename].m_eToken  = TN_INTEGER;

  m_nEnames++;

  //vDumpSet();
  ASSERT(bHasName(szName));
}

/*----------------------------------------------------------------------------*/
/* add name and value                                                         */
/*----------------------------------------------------------------------------*/
//void EnameSet::vInsert(Stz* szName, Stz* szValue)
//{
//  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
//
//  i8 i_Ename = m_nEnames;
//
//  m_xEname[i_Ename].m_i_Name  = m_xyStrings.iAppend(szName);
//  m_xEname[i_Ename].m_i_Value = m_xyStrings.iAppend(szValue);
//  m_xEname[i_Ename].m_eToken  = TN_STRING;
//
//  m_nEnames++;
//
//  //vDumpSet();
//  if (! bHasName(szName)) {
//    RASSERT(0);
//  }
//}
/*----------------------------------------------------------------------------*/
void EnameSet::vInsert(Stz* szName, Stz* szValue)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);

  i8 i_Ename = i_InsertPoint(szName);
  vShiftDown(i_Ename);

  m_xEname[i_Ename].m_i_Name  = m_xyStrings.iAppend(szName);
  m_xEname[i_Ename].m_i_Value = m_xyStrings.iAppend(szValue);
  m_xEname[i_Ename].m_eToken  = TN_STRING;

  m_nEnames++;

  //vDumpSet();
  ASSERT(bHasName(szName));
}

/*----------------------------------------------------------------------------*/
/* get nItems                                                                 */
/*----------------------------------------------------------------------------*/
i8 EnameSet::nItems(void)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
  
  return m_nEnames;
}

/*----------------------------------------------------------------------------*/
/* get name                                                                   */
/*----------------------------------------------------------------------------*/
Stz* EnameSet::szName(i8 i_Ename)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
  
  Stz* szName = m_xyStrings.szGet(m_xEname[i_Ename].m_i_Name);
  
  return szName;
}

/*----------------------------------------------------------------------------*/
/* produce names string                                                       */
/*----------------------------------------------------------------------------*/
void EnameSet::vNamesString(String* pString)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);

  char* azStr = pString->azStr();
  char* azCrs = pString->azStr();
  
  for (i8 i_Ename = 0; i_Ename < m_nEnames; i_Ename++) {
    if (i_Ename)
      StrLib::vAppendChar(&azCrs,' ');
    char* azName = szName(i_Ename)->azChar();
    StrLib::vAppendString(&azCrs,azName);
  }
}

/*----------------------------------------------------------------------------*/
/* copy source ename-set into this ename-set and replace an ename pair        */
/*----------------------------------------------------------------------------*/
void EnameSet::vReplace(Stz* szName, i8 iValue)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
  
  EnameSet xEnameSet;
  
  bool bWasCopied = false;
  
  xEnameSet.vCopy(this);
  vReset();
  
  for (i8 i_Ename = 0; i_Ename < xEnameSet.nEnames(); i_Ename++) {
    
    if (TN_STRING == xEnameSet.eToken(i_Ename)) {
      vInsert(xEnameSet.szName(i_Ename),xEnameSet.szValue(i_Ename));
    }
    else {
      if (xEnameSet.szName(i_Ename)->bSame(szName)) {
        vInsert(xEnameSet.szName(i_Ename),iValue);
        bWasCopied = true;
      }
      else {
        vInsert(xEnameSet.szName(i_Ename),xEnameSet.iValue(i_Ename));
      }
    }
  }
  
  if (! bWasCopied)
    vInsert(szName,iValue);
}

/*----------------------------------------------------------------------------*/
/* copy source ename-set into this ename-set and replace an ename pair        */
/*----------------------------------------------------------------------------*/
void EnameSet::vReplace(Stz* szName, Stz* szValue)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
  
  EnameSet xEnameSet;
  
  bool bWasCopied = false;
  
  xEnameSet.vCopy(this);
  vReset();
  
  for (i8 i_Ename = 0; i_Ename < xEnameSet.nEnames(); i_Ename++) {
    
    if (TN_STRING == xEnameSet.eToken(i_Ename)) {
      
      if (xEnameSet.szName(i_Ename)->bSame(szName)) {
        vInsert(xEnameSet.szName(i_Ename),szValue);
        bWasCopied = true;
      }
      else {
        vInsert(xEnameSet.szName(i_Ename),xEnameSet.szValue(i_Ename));
      }
    }
    else
      vInsert(xEnameSet.szName(i_Ename),xEnameSet.iValue(i_Ename));
  }
  
  if (! bWasCopied)
    vInsert(szName,szValue);
}

/*----------------------------------------------------------------------------*/
/* get token kind                                                             */
/*----------------------------------------------------------------------------*/
void EnameSet::vReset(void)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);

  m_nEnames = 0;
  
  m_xyStrings.vReset();
}

/*----------------------------------------------------------------------------*/
/* sprint                                                                     */
/*----------------------------------------------------------------------------*/
void EnameSet::vSprint(Stz* sArgs)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
  
  sArgs->vCopy("("_Stz);
  for (i8 i_Ename = 0; i_Ename < m_nEnames; i_Ename++) {
    
    if (i_Ename)
      sArgs->vConcat(", "_Stz);
    
    if (TN_INTEGER == m_xEname[i_Ename].m_eToken) {
      s32 sValue;
      sValue.vSprint("%lld"_Stz,m_xEname[i_Ename].m_i_Value);
      sArgs->vConcat(sValue);
      continue;
    }
    
    sArgs->vConcat(m_xyStrings.szGet(m_xEname[i_Ename].m_i_Value));
  }
  
  sArgs->vConcat(") "_Stz);
}

/*----------------------------------------------------------------------------*/
/* get token kind                                                             */
/*----------------------------------------------------------------------------*/
eTN EnameSet::eToken(i8 i_Ename)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
  
  eTN eToken = m_xEname[i_Ename].m_eToken;
  
  return eToken;
}

/*----------------------------------------------------------------------------*/
/* get value                                                                  */
/*----------------------------------------------------------------------------*/
i8 EnameSet::iValue(i8 i_Ename)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
  
  //Stz* szValue = m_xyStrings.szGet(m_xEname[i_Ename].m_i_Value);
  //i8 iValue = szValue->iToI8();
  
  ASSERT(TN_INTEGER == m_xEname[i_Ename].m_eToken);
  
  i8 iValue = m_xEname[i_Ename].m_i_Value;
  
  return iValue;
}

/*----------------------------------------------------------------------------*/
/* get value                                                                  */
/*----------------------------------------------------------------------------*/
Stz* EnameSet::szValue(i8 i_Ename)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
  
  ASSERT(TN_INTEGER != m_xEname[i_Ename].m_eToken);
  Stz* szValue = m_xyStrings.szGet(m_xEname[i_Ename].m_i_Value);

  return szValue;
}

/*----------------------------------------------------------------------------*/
/* get value                                                                  */
/*----------------------------------------------------------------------------*/
i8 EnameSet::iValue(Stz *szName)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
  
  i8 i_Ename = i_Find(szName);
  
  if (C_UNKNOWN == i_Ename)
    return 0;
  //vPrint();
  ASSERT(TN_INTEGER == m_xEname[i_Ename].m_eToken);
  i8 iValue = m_xEname[i_Ename].m_i_Value;

  return iValue;
}

/*----------------------------------------------------------------------------*/
/* get value                                                                  */
/*----------------------------------------------------------------------------*/
Stz* EnameSet::szValue(Stz *szName)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
  
  i8 i_Ename = i_Find(szName);
  
  if (C_UNKNOWN == i_Ename)
    return ""_Stz;
  
  ASSERT(TN_INTEGER != m_xEname[i_Ename].m_eToken);
  Stz* szValue = m_xyStrings.szGet(m_xEname[i_Ename].m_i_Value);
  
  return szValue;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* find where to add the given name                                           */
/*----------------------------------------------------------------------------*/
i8 EnameSet::i_InsertPoint(Stz* szNewName)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
  
  if (! m_nEnames)
    return 0;
  
  for (i8 i_Scan = 0; i_Scan < m_nEnames; i_Scan++)
    if (szName(i_Scan)->iCompare(szNewName) >= 0)
      return i_Scan;
  
  return m_nEnames;
}


/*----------------------------------------------------------------------------*/
/* shift sub set down so as to open up i_From                                 */
/*----------------------------------------------------------------------------*/
void EnameSet::vShiftDown(i8 i_From)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
  
  for (i8 i_Dst = m_nEnames; i_Dst > i_From; i_Dst--) {
    m_xEname[i_Dst].m_i_Name  = m_xEname[i_Dst - 1].m_i_Name;
    m_xEname[i_Dst].m_i_Value = m_xEname[i_Dst - 1].m_i_Value;
    m_xEname[i_Dst].m_eToken  = m_xEname[i_Dst - 1].m_eToken;
  }
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(EnameSet)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
