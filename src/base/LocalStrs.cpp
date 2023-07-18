/*============================================================================*/
/* LocalStrs.cpp -- Local Variables                                           */
/*============================================================================*/

#include "BASE.h"

#include "LocalStrs.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,LocalStrs,CN_LOCALSTRS)
__aNewFunction(LocalStrs,CN_LOCALSTRS)
__vDeleteFunction(LocalStrs,CN_LOCALSTRS)
__aFooterFunction(LocalStrs)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
LocalStrs::LocalStrs(void)
: Object(CN_LOCALSTRS)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALSTRS,_FOOTER_);

  m_i_FirstFree = C_LOCAL_STRS_reduced - 1;
  
  for (i8 i_LclStr = 0; i_LclStr < C_LOCAL_STRS_reduced; i_LclStr++) {
    LocalStr& rxLocalStr = m_xaLocalStr[i_LclStr];

    rxLocalStr.m_i_LclStr   = i_LclStr;
    rxLocalStr.m_i_NextFree = i_LclStr - 1;

    rxLocalStr.vReInitialise();
    rxLocalStr.m_pText = new Text;

  }
  
  _checkInitialised_(CN_LOCALSTRS);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_LOCALSTRS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
LocalStrs::~LocalStrs(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALSTRS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot a local string                                                       */
/*----------------------------------------------------------------------------*/
LocalStr* LocalStrs::axAllot(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALSTRS,_FOOTER_);

  RASSERT(C_UNKNOWN != m_i_FirstFree);
  
  LocalStr* axLocalStr = &m_xaLocalStr[m_i_FirstFree];
  RASSERT(axLocalStr->m_i_LclStr == m_i_FirstFree);

  m_i_FirstFree = axLocalStr->m_i_NextFree;
  
  axLocalStr->m_i_NextFree = C_UNKNOWN;
  
  //vTrace("LocalStr: Allot : %lld\n"_Stz,axLocalStr->m_i_LclStr);
  
  return axLocalStr;
}

/*----------------------------------------------------------------------------*/
/* revert given local string                                                  */
/*----------------------------------------------------------------------------*/
void LocalStrs::vRevert(LocalStr* axLocalStr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALSTRS,_FOOTER_);

  //vTrace("LocalStr:  Revert : %lld\n"_Stz,axLocalStr->m_i_LclStr);
  
  RASSERT(C_UNKNOWN == axLocalStr->m_i_NextFree);
  axLocalStr->vReInitialise();

  axLocalStr->m_i_NextFree = m_i_FirstFree;
  m_i_FirstFree = axLocalStr->m_i_LclStr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
