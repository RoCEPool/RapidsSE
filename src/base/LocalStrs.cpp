/*============================================================================*/
/* LocalStrs.cpp -- Local Text                                                */
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

  m_i_FirstFree = C_LOCAL_STRS - 1;
  
  for (i8 i_LclStr = 0; i_LclStr < C_LOCAL_STRS; i_LclStr++) {
    LocalStr& rxLocalStr = m_xaLocalStr[i_LclStr];
    rxLocalStr.vInitialise(i_LclStr,i_LclStr - 1);
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
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* repair the local strings                                                   */
/*----------------------------------------------------------------------------*/
void LocalStrs::vRepair(Object* pObject)
{
  _static_

  LocalStrs* pLocalStrs = _pLocalStrs(pObject);

  // delete the localStrs
  delete pLocalStrs;
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
  RASSERT(axLocalStr->i_LocalStr() == m_i_FirstFree);

  m_i_FirstFree = axLocalStr->i_NextFree();
  
  axLocalStr->v_NextFree(C_UNKNOWN);

  return axLocalStr;
}

/*----------------------------------------------------------------------------*/
/* revert given local string                                                  */
/*----------------------------------------------------------------------------*/
void LocalStrs::vRevert(LocalStr* axLocalStr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALSTRS,_FOOTER_);

  RASSERT(C_UNKNOWN == axLocalStr->i_NextFree());

  axLocalStr->vRevertBuf();
  axLocalStr->v_NextFree(m_i_FirstFree);
  m_i_FirstFree = axLocalStr->i_LocalStr();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
