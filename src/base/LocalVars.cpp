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
  
  m_nLocalVars = 0;
  
  for (i8 i_LocalVar = 0; i_LocalVar < C_LOCAL_VARS; i_LocalVar++) {
    LocalVar& rxLocalVar = m_xaLocalVar[i_LocalVar];

    rxLocalVar.m_pLocalVars    = this;
    rxLocalVar.m_i_LocalVar    = i_LocalVar;
    rxLocalVar.m_aDiscardPoint = nullptr;
    rxLocalVar.m_aLocalStr     = nullptr;
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

  RASSERT(m_nLocalVars);
  
  LocalVar& rxLocalVar = m_xaLocalVar[m_nLocalVars - 1];
  void* aDiscardPoint = rxLocalVar.m_aDiscardPoint;
  
  while (m_nLocalVars) {
    
    rxLocalVar = m_xaLocalVar[m_nLocalVars - 1];

    if (rxLocalVar.m_aDiscardPoint != aDiscardPoint)
      break;

    // test for string
    // tbd...
    
    m_nLocalVars--;
  }
  
  RASSERT(m_nLocalVars >= 0);
}

/*----------------------------------------------------------------------------*/
/* push new frame of locals                                                   */
/*----------------------------------------------------------------------------*/
void LocalVars::vFramePush(i8 nLocals, void* aDiscardPoint)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALVARS,_FOOTER_);

  RASSERT(nLocals);
  RASSERT(m_nLocalVars + nLocals <= C_LOCAL_VARS);

  for (i8 iLocal = 0; iLocal < nLocals; iLocal++) {
    
    i8 i_LocalVar = m_nLocalVars++;
    RASSERT(i_LocalVar < C_LOCAL_VARS);

    LocalVar& rxLocalVar = m_xaLocalVar[i_LocalVar];
    
    rxLocalVar.m_aDiscardPoint = aDiscardPoint;
  }
  
  RASSERT(m_nLocalVars <= C_LOCAL_VARS);
}

/*----------------------------------------------------------------------------*/
/* get given local of top frame                                               */
/*----------------------------------------------------------------------------*/
LocalVar* LocalVars::axLocalVar(i8 i_Local)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALVARS,_FOOTER_);

  RASSERT(m_nLocalVars);
  RASSERT(i_Local < m_nLocalVars);

  i8 i_LocalVar = (m_nLocalVars - 1) - i_Local;

  LocalVar* axLocalVar = &m_xaLocalVar[i_LocalVar];

  return axLocalVar;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
