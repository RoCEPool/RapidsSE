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
  
  m_pLocalStrs = new LocalStrs;
  
  m_nLocalVars = 0;
  
  for (i8 i_LocalVar = 0; i_LocalVar < C_LOCAL_VARS; i_LocalVar++) {
    LocalVar& rxLocalVar = m_xaLocalVar[i_LocalVar];

    rxLocalVar.m_pLocalVars    = this;
    rxLocalVar.m_aDiscardPoint = nullptr;
    rxLocalVar.m_axLocalStr     = nullptr;
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
/* return top-most discard point                                              */
/*----------------------------------------------------------------------------*/
void* LocalVars::aDiscardPoint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALVARS,_FOOTER_);
  
  if (! m_nLocalVars)
    return nullptr;
  
  LocalVar& rxLocalVar = m_xaLocalVar[m_nLocalVars - 1];
  void* aDiscardPoint = rxLocalVar.m_aDiscardPoint;

  return aDiscardPoint;
}

/*----------------------------------------------------------------------------*/
/* pop top frame of locals                                                    */
/*----------------------------------------------------------------------------*/
void LocalVars::vFramePop(void* aDiscardPoint_return)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALVARS,_FOOTER_);

  static i8 nCall = 0;
  
  RASSERT(m_nLocalVars);
  
  //vVerify();
  
  LocalVar& rxLocalVar = m_xaLocalVar[m_nLocalVars - 1];
  void* aDiscardPoint = rxLocalVar.m_aDiscardPoint;
  
  RASSERT(aDiscardPoint == aDiscardPoint_return);
  
  i8 nDiscards = 0;
  while (m_nLocalVars) {
    //vVerify();

    LocalVar& rxLocalVar = m_xaLocalVar[m_nLocalVars - 1];

    if (rxLocalVar.m_aDiscardPoint != aDiscardPoint)
      break;

    //vVerify();

    // test for string
    eCN eType = rxLocalVar.eType();
    if (CN_STZVAL == eType)
      rxLocalVar.vLocalStrRevert();

    //vVerify();

    rxLocalVar.m_aDiscardPoint = nullptr;

    //vVerify();

    m_nLocalVars--;
    nDiscards++;
  }
  
  //vVerify();

  RASSERT(m_nLocalVars >= 0);
  
  nCall++;
}

/*----------------------------------------------------------------------------*/
/* push new frame of locals                                                   */
/*----------------------------------------------------------------------------*/
bool LocalVars::bFramePush(i8 nLocals, void* aDiscardPoint)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALVARS,_FOOTER_);

  RASSERT(nLocals);
  RASSERT(m_nLocalVars + nLocals <= C_LOCAL_VARS);

  //vVerify();

  if (m_nLocalVars) {
    LocalVar& rxLocalVar = m_xaLocalVar[m_nLocalVars - 1];
    void* aDiscardPoint_top = rxLocalVar.m_aDiscardPoint;

    //RASSERT(aDiscardPoint != aDiscardPoint_top);
    if (aDiscardPoint == aDiscardPoint_top)
      return false;
  }
  
  for (i8 iLocal = 0; iLocal < nLocals; iLocal++) {
    
    i8 i_LocalVar = m_nLocalVars++;
    RASSERT(i_LocalVar < C_LOCAL_VARS);

    LocalVar& rxLocalVar = m_xaLocalVar[i_LocalVar];
    
    rxLocalVar.m_aDiscardPoint = aDiscardPoint;
  }
  
  //vVerify();

  RASSERT(m_nLocalVars <= C_LOCAL_VARS);
  
  return true;
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

/*----------------------------------------------------------------------------*/
/* allot a localStr                                                           */
/*----------------------------------------------------------------------------*/
LocalStr* LocalVars::axLocalStrAllot(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALVARS,_FOOTER_);

  LocalStr* axLocalStr = m_pLocalStrs->axAllot();
  
  return axLocalStr;
}

/*----------------------------------------------------------------------------*/
/* revert a localStr                                                          */
/*----------------------------------------------------------------------------*/
void LocalVars::vLocalStrRevert(LocalStr* axLocalStr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALVARS,_FOOTER_);

  m_pLocalStrs->vRevert(axLocalStr);
}

/*----------------------------------------------------------------------------*/
/* verify                                                                     */
/*----------------------------------------------------------------------------*/
//void LocalVars::vVerify(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALVARS,_FOOTER_);
//  
//  for (i8 i_LocalVar = 0; i_LocalVar < C_LOCAL_VARS; i_LocalVar++) {
//    LocalVar& rxLocalVar = m_xaLocalVar[i_LocalVar];
//
//    RASSERT(this == rxLocalVar.m_pLocalVars);
//    RASSERT(i_LocalVar == rxLocalVar.m_i_LocalVar);
//  }
//}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
