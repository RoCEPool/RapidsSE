/*============================================================================*/
/* LocalVars.cpp -- Local Variables                                           */
/*============================================================================*/

#include "BASE.h"

#include "LocalVars.hpp"
#include "LocalStrs.hpp"

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
  
  m_pLocalStrs = new LocalStrs(this);
  
  m_nLocalsVars = 0;
  
  for (i8 i_LclVar = 0; i_LclVar < C_LOCAL_VARS; i_LclVar++) {
    _LclVar& rxLvlVar = m_xaLclVar[i_LclVar];

    rxLvlVar.m_pLocalVars    = this;
    rxLvlVar.m_i_LclVar    = i_LclVar;
    rxLvlVar.m_aDiscardPoint = nullptr;
    rxLvlVar.m_aLocalStr     = nullptr;
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
  
  delete m_pLocalStrs;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* pop top frame of locals                                                    */
/*----------------------------------------------------------------------------*/
void LocalVars::vFramePop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALVARS,_FOOTER_);

  RASSERT(m_nLocalsVars);
  
  _LclVar& rxLvlVar = m_xaLclVar[m_nLocalsVars - 1];
  void* aDiscardPoint = rxLvlVar.m_aDiscardPoint;
  
  while (m_nLocalsVars && aDiscardPoint == rxLvlVar.m_aDiscardPoint) {
    rxLvlVar = m_xaLclVar[m_nLocalsVars - 1];
  
    // test for string
    // tbd...
    
    m_nLocalsVars--;
  }
  
  RASSERT(m_nLocalsVars >= 0);
}

/*----------------------------------------------------------------------------*/
/* push new frame of locals                                                   */
/*----------------------------------------------------------------------------*/
void LocalVars::vFramePush(i8 nLocals, void* aDiscardPoint)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALVARS,_FOOTER_);

  RASSERT(m_nLocalsVars + nLocals <= C_LOCAL_VARS);

  for (i8 iLocal = 0; iLocal < nLocals; iLocal++) {
    
    i8 i_LclVar = m_nLocalsVars++;
    RASSERT(i_LclVar < C_LOCAL_VARS);

    _LclVar& rxLvlVar = m_xaLclVar[i_LclVar];
    
    rxLvlVar.m_aDiscardPoint = aDiscardPoint;
  }
  
  RASSERT(m_nLocalsVars <= C_LOCAL_VARS);
}

/*----------------------------------------------------------------------------*/
/* get given local of top frame                                               */
/*----------------------------------------------------------------------------*/
_LclVar* LocalVars::axGetLocal(i8 i_Local)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALVARS,_FOOTER_);

  RASSERT(m_nLocalsVars);
  RASSERT(i_Local < m_nLocalsVars);

  i8 i_LclVar = (m_nLocalsVars - 1) - i_Local;

  _LclVar* axLvlVar = &m_xaLclVar[i_LclVar];

  return axLvlVar;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
