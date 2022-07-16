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
LocalStrs::LocalStrs(LocalVars* pLocalVars)
: Object(CN_LOCALSTRS)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALSTRS,_FOOTER_);
 
  m_pLocalVars = pLocalVars;

  m_i_FirstFree = C_LOCAL_STRS - 1;
  
  for (i8 i_LclStr = 0; i_LclStr < C_LOCAL_STRS; i_LclStr++) {
    _LclStr& rxLvlStr = m_xaLclStr[i_LclStr];
    
    rxLvlStr.m_pLocalStrs = this;
    rxLvlStr.m_i_LclStr   = i_LclStr;
    rxLvlStr.m_i_NextFree = i_LclStr - 1;

    rxLvlStr.m_xSizeArea.m_jStrSize = 0;
    MemLib::vZero(rxLvlStr.m_xSizeArea.m_caArea,
                  sizeof(rxLvlStr.m_xSizeArea.m_caArea),
                  CN_LOCALSTRS);
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
_LclStr* LocalStrs::axAllotLclStr(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALVARS,_FOOTER_);

  RASSERT(C_UNKNOWN != m_i_FirstFree);
  
  _LclStr* axLclStr = &m_xaLclStr[m_i_FirstFree];
  
  m_i_FirstFree = axLclStr->m_i_NextFree;
  
  return axLclStr;
}

/*----------------------------------------------------------------------------*/
/* free given local string                                                    */
/*----------------------------------------------------------------------------*/
void LocalStrs::vFreeLclStr(_LclStr* axLclStr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALVARS,_FOOTER_);

  axLclStr->m_xSizeArea.m_jStrSize = 0;
  MemLib::vZero(axLclStr->m_xSizeArea.m_caArea,
                sizeof(axLclStr->m_xSizeArea.m_caArea),
                CN_LOCALSTRS);

  axLclStr->m_i_NextFree = m_i_FirstFree;
  m_i_FirstFree = axLclStr->m_i_LclStr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
