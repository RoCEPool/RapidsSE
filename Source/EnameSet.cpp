/*============================================================================*/
/* EnameSet.cpp -- ename-value set                                              */
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
/* add                                                                        */
/*----------------------------------------------------------------------------*/
void EnameSet::vAdd(Stz* szName, i8 iValue)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
  
  i8 i_Ename = m_nEnames;
  
  m_xEname[i_Ename].m_i_Name  = m_bStrings.iAppend(szName);
  m_xEname[i_Ename].m_i_Value = iValue;
  m_xEname[i_Ename].m_eToken  = TN_INTEGER;
  
  m_nEnames++;
}

/*----------------------------------------------------------------------------*/
/* add                                                                        */
/*----------------------------------------------------------------------------*/
void EnameSet::vAdd(Stz* szName, Stz* szValue)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
  
  i8 i_Ename = m_nEnames;
  
  m_xEname[i_Ename].m_i_Name  = m_bStrings.iAppend(szName);
  m_xEname[i_Ename].m_i_Value = m_bStrings.iAppend(szValue);
  m_xEname[i_Ename].m_eToken  = TN_STRING;
  
  m_nEnames++;
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
  
  vPrint();
}

/*----------------------------------------------------------------------------*/
/* copy source ename-set into this ename-set                                  */
/*----------------------------------------------------------------------------*/
void EnameSet::vCopy(EnameSet* pScrEnameSet)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
  
  for (i8 i_Ename = 0; i_Ename < pScrEnameSet->nEnames(); i_Ename++) {
    
    if (TN_STRING == pScrEnameSet->eToken(i_Ename))
      vAdd(pScrEnameSet->szName(i_Ename),pScrEnameSet->szValue(i_Ename));
    else
      vAdd(pScrEnameSet->szName(i_Ename),pScrEnameSet->iValue(i_Ename));
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
    if (m_bStrings.szGet(m_xEname[i_Ename].m_i_Name)->bSame(szName))
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
/* get name                                                                   */
/*----------------------------------------------------------------------------*/
Stz* EnameSet::szName(i8 i_Ename)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
  
  Stz* szName = m_bStrings.szGet(m_xEname[i_Ename].m_i_Name);
  
  return szName;
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void EnameSet::vPrint(void)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
  
  vTrace("{ "_Stz);
  for (i8 i_Ename = 0; i_Ename < m_nEnames; i_Ename++) {
    
    if (i_Ename)
      vTrace(", "_Stz);
    
    if (TN_INTEGER == m_xEname[i_Ename].m_eToken) {
      
      vTrace("%s=%lld"_Stz,
             
             m_bStrings.szGet(m_xEname[i_Ename].m_i_Name),
             m_xEname[i_Ename].m_i_Value,
             
             C_ENDARGS
             );
      continue;
    }
    
    vTrace("%s=%s"_Stz,
    
           m_bStrings.szGet(m_xEname[i_Ename].m_i_Name),
           m_bStrings.szGet(m_xEname[i_Ename].m_i_Value),
           
           C_ENDARGS
           );
  }
 
  vTrace(" }\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void EnameSet::vPrintDetailed(void)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
  
  for (i8 i_Ename = 0; i_Ename < m_nEnames; i_Ename++) {
    vTrace("  [%lld] %s _%lld, %s _%lld (%s)\n"_Stz,
           
           i_Ename,
           
           m_bStrings.szGet(m_xEname[i_Ename].m_i_Name),
           m_xEname[i_Ename].m_i_Name,
           
           m_bStrings.szGet(m_xEname[i_Ename].m_i_Value),
           m_xEname[i_Ename].m_i_Value,
           
           aTN[m_xEname[i_Ename].m_eToken],
           
           C_ENDARGS
           );
  }
}

/*----------------------------------------------------------------------------*/
/* copy source ename-set into this ename-set                                  */
/*----------------------------------------------------------------------------*/
void EnameSet::vReplace(Stz* szName, Stz* szValue)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
  
  EnameSet xEnameSet;
  
  xEnameSet.vCopy(this);
  vReset();
  
  for (i8 i_Ename = 0; i_Ename < xEnameSet.nEnames(); i_Ename++) {
    
    if (TN_STRING == xEnameSet.eToken(i_Ename)) {
      
      if (xEnameSet.szName(i_Ename)->bSame(szName))
        vAdd(xEnameSet.szName(i_Ename),szValue);
      else
        vAdd(xEnameSet.szName(i_Ename),xEnameSet.szValue(i_Ename));
    }
    else
      vAdd(xEnameSet.szName(i_Ename),xEnameSet.iValue(i_Ename));
  }
}

/*----------------------------------------------------------------------------*/
/* get token kind                                                             */
/*----------------------------------------------------------------------------*/
void EnameSet::vReset(void)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);

  m_nEnames = 0;
  
  m_bStrings.vReset();
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
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
    
    sArgs->vConcat(m_bStrings.szGet(m_xEname[i_Ename].m_i_Value));
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
  
  //Stz* szValue = m_bStrings.szGet(m_xEname[i_Ename].m_i_Value);
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
  Stz* szValue = m_bStrings.szGet(m_xEname[i_Ename].m_i_Value);

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
  Stz* szValue = m_bStrings.szGet(m_xEname[i_Ename].m_i_Value);
  
  return szValue;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(EnameSet)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
