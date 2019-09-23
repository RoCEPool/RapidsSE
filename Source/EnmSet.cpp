/*============================================================================*/
/* EnmSet.cpp -- ename-value set                                              */
/*============================================================================*/

#include "Gadgets.h"

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
EnmSet* Gadgets::_pEnmSet(void* aAddr)
{
  EnmSet* pEnmSet = reinterpret_cast<EnmSet*>(aAddr);
  
  RASSERT_TYPE(pEnmSet,CN_ENMSET);
  
  return pEnmSet;
}

/*----------------------------------------------------------------------------*/
/* new operator                                                               */
/*----------------------------------------------------------------------------*/
void* EnmSet::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_ENMSET);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* delete operator                                                            */
/*----------------------------------------------------------------------------*/
void EnmSet::operator delete(void* aAddr)
{
  Object* pObject = reinterpret_cast<Object*>(aAddr);
  
  HeapMem::vFree(pObject);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EnmSet::EnmSet(void)
: Object(CN_ENMSET)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
 
  m_nEnm = 0;
  
  for (i8 i_Eno = 0; i_Eno < C_ENM_SET; i_Eno++) {
    m_xEnnPair[i_Eno].m_i_Name  = C_UNKNOWN;
    m_xEnnPair[i_Eno].m_i_Value = C_UNKNOWN;
    m_xEnnPair[i_Eno].m_eToken  = TN_NONE;
  }
  
  _checkInitialised_(CN_ENMSET);
  
  _postcondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
EnmSet::~EnmSet(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add                                                                        */
/*----------------------------------------------------------------------------*/
void EnmSet::vAdd(Stz* szName, Stz* szValue, eTN eToken)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
  
  m_xEnnPair[m_nEnm].m_i_Name  = m_bStrings.iAppend(szName);
  m_xEnnPair[m_nEnm].m_i_Value = m_bStrings.iAppend(szValue);
  m_xEnnPair[m_nEnm].m_eToken  = eToken;
  
  m_nEnm++;
}

/*----------------------------------------------------------------------------*/
/* get nEno                                                                   */
/*----------------------------------------------------------------------------*/
i8 EnmSet::nEnm(void)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
  
  return m_nEnm;
}

/*----------------------------------------------------------------------------*/
/* get name                                                                   */
/*----------------------------------------------------------------------------*/
Stz* EnmSet::szName(i8 i_Enm)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
  
  Stz* szName = m_bStrings.szGet(m_xEnnPair[i_Enm].m_i_Name);
  
  return szName;
}

/*----------------------------------------------------------------------------*/
/* get token kind                                                             */
/*----------------------------------------------------------------------------*/
void EnmSet::vReset(void)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);

  m_nEnm = 0;
  
  m_bStrings.vReset();
}

/*----------------------------------------------------------------------------*/
/* get token kind                                                             */
/*----------------------------------------------------------------------------*/
eTN EnmSet::eToken(i8 i_Enm)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
  
  eTN eToken = m_xEnnPair[i_Enm].m_eToken;
  
  return eToken;
}

/*----------------------------------------------------------------------------*/
/* get value                                                                  */
/*----------------------------------------------------------------------------*/
Stz* EnmSet::szValue(i8 i_Enm)
{
  _precondition_(_MAGIC_,CN_ENMSET,_FOOTER_);
  
  Stz* szValue = m_bStrings.szGet(m_xEnnPair[i_Enm].m_i_Value);

  return szValue;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(EnmSet)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
