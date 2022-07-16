/*============================================================================*/
/* LocalVars.cpp -- Local Variables                                           */
/*============================================================================*/

#include "BASE.h"

#include "LocalVars.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,LocalVars,CN_LOCALVARS)
__aNewFunction(LocalVars,CN_LOCALVARS)
__vDeleteFunction(LocalVars,CN_LOCALVARS)
__aFooterFunction(LocalVars)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
LocalVars::LocalVars(void)
: Object(CN_LOCALVARS)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALVARS,_FOOTER_);
  
  m_pLocalStrs = nullptr;
  
  m_nLocals = 0;
  
  for (i8 i_LclVar = 0; i_LclVar < C_LOCAL_VARS; i_LclVar++) {
    _LclVar& rxLvlVar = m_xaLclVar[i_LclVar];

    rxLvlVar.m_i_Local       = i_LclVar;
    rxLvlVar.m_aDiscardPoint = nullptr;
  }
  
  _checkInitialised_(CN_LOCALVARS);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_LOCALVARS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
LocalVars::~LocalVars(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALVARS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* remove frame of locals                                                     */
/*----------------------------------------------------------------------------*/
void LocalVars::vFramePop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALVARS,_FOOTER_);

  RASSERT(m_nLocals);
  
  _LclVar& rxLvlVar = m_xaLclVar[m_nLocals - 1];
  void* aDiscardPoint = rxLvlVar.m_aDiscardPoint;
  
  while (aDiscardPoint == rxLvlVar.m_aDiscardPoint) {
    
    if (! --m_nLocals)
      break;
    
    rxLvlVar = m_xaLclVar[m_nLocals - 1];
  }
  
  RASSERT(m_nLocals >= 0);

}

/*----------------------------------------------------------------------------*/
/* add new frame of locals                                                    */
/*----------------------------------------------------------------------------*/
void LocalVars::vFramePush(i8 nLocals, void* aDiscardPoint)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALVARS,_FOOTER_);

  for (i8 iLocal = 0; iLocal < nLocals; iLocal++) {
    
    i8 i_Local = m_nLocals++;
    RASSERT(i_Local < C_LOCAL_VARS);

    _LclVar& rxLvlVar = m_xaLclVar[i_Local];
    
    rxLvlVar.m_aDiscardPoint = aDiscardPoint;
  }
  
  RASSERT(m_nLocals <= C_LOCAL_VARS);

}

/*----------------------------------------------------------------------------*/
/* get given local                                                            */
/*----------------------------------------------------------------------------*/
_LclVar* LocalVars::aGetLocal(i8 i_Local)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALVARS,_FOOTER_);

  return nullptr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
