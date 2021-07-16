/*============================================================================*/
/* Steps_RegOp.cpp - op[] step group                                          */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "ePX.h"

#include "Node_I.hpp"
#include "Op.hpp"
#include "Job.hpp"
#include "LogicUnit.hpp"
#include "StageSpec.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RegOp(void)
{
  vAddStep("op[]"_Stz,                 &Step::vRegOpGet);
  vAddStep("op[].handlerEntry"_Stz,    &Step::vRegOpHandlerEntry);
  vAddStep("op[].handlerExit"_Stz,     &Step::vRegOpHandlerExit);
  vAddStep("op[].handler@"_Stz,        &Step::vRegOpHandlerFetch);
  vAddStep("op[].handler!"_Stz,        &Step::vRegOpHandlerStore);
  vAddStep("op[].handlerAll!"_Stz,     &Step::vRegOpHandlerAllStore);
  vAddStep("op[].init"_Stz,            &Step::vRegOpInit);
  vAddStep("op[].isOperandValid"_Stz,  &Step::vRegOpIsOperandValid);
  vAddStep("op[].incHandlerCount"_Stz, &Step::vRegOpIncHandlerCount);
  vAddStep("op[].incOperandCount"_Stz, &Step::vRegOpIncOperandCount);
}

/*----------------------------------------------------------------------------*/
/* op[].get -- get op                                                         */
/*----------------------------------------------------------------------------*/
void Step::vRegOpGet(void)
{
  ENTRY();
    
  m_pDataStack->vPushPtr(*m_pJob->pLogicUnit_job()->m_xpOp);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* op[].handlerEntry -- handler entry                                         */
/*----------------------------------------------------------------------------*/
void Step::vRegOpHandlerEntry(void)
{
  ENTRY();
  
  Op* pOp = m_pJob->m_pOp;
  
  if (m_pJob->pLogicUnit_job()->m_xpPipein.bIsNotNull()) {
    Operand* pOperand = *m_pJob->pLogicUnit_job()->m_xpPipein;
  
    pOp->vOpTrace(pOperand,(Stz*)__func__,true);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* op[].handlerExit -- handler exit                                           */
/*----------------------------------------------------------------------------*/
void Step::vRegOpHandlerExit(void)
{
  ENTRY();
  
//  Op* pOp = m_pJob->m_pOp;
//  
//  if (m_pJob->pLogicUnit_job()->m_xpPipein.bIsNotNull()) {
//    Operand* pOperand = *m_pJob->pLogicUnit_job()->m_xpPipein;
//  
//    pOp->vOpTrace(pOperand,(Stz*)__func__,true);
//  }
//  else {
//    pOp->vOpTrace(nullptr,(Stz*)__func__,true);
//  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* op[].handler@ -- get this op's handler for given operand                   */
/*----------------------------------------------------------------------------*/
void Step::vRegOpHandlerFetch(void)
{
  ENTRY();

  i8 i_OperandKind = m_pDataStack->iPopInt(__func__);

  Word* pHandler = m_pJob->pLogicUnit_job()->m_xpOp->pOperandHandler(i_OperandKind);
    
  m_pDataStack->vPushPtr(pHandler);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* op[].handler! -- set this op's handler for given operand                   */
/*----------------------------------------------------------------------------*/
void Step::vRegOpHandlerStore(void)
{
  ENTRY();

  i8 i_OpKind = m_pDataStack->iPopInt(__func__);
  
  Word* pHandler = _pWord(m_pDataStack->pPopPtr(__func__));

  m_pJob->pLogicUnit_job()->m_xpOp->vOperandHandler(i_OpKind,pHandler);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* op[].handlerAll! -- set this op's handler for given operand                */
/*----------------------------------------------------------------------------*/
void Step::vRegOpHandlerAllStore(void)
{
  ENTRY();

  Word* pHandler = _pWord(m_pDataStack->pPopPtr(__func__));

  for (i8 i_OpKind = 0; i_OpKind < ZK_ENUMS; i_OpKind++)
    m_pJob->pLogicUnit_job()->m_xpOp->vOperandHandler(i_OpKind,pHandler);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* op[].init -- init op                                                       */
/*----------------------------------------------------------------------------*/
void Step::vRegOpInit(void)
{
  ENTRY();

  Op* pOp = m_pJob->m_pOp;

  m_pJob->pLogicUnit_job()->m_xpOp = pOp;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* op[].isOperandValid -- check if operand is valid                           */
/*----------------------------------------------------------------------------*/
void Step::vRegOpIsOperandValid(void)
{
  ENTRY();
  
  bool bValid = true;
  
  i8 i_OperandKind = m_pDataStack->iPopInt(__func__);
  
  // skip consumeOp operand validation
  if ( ! m_pJob->pLogicUnit_job()->m_xpOp->szOpNm()->bSame("consumeOP"_Stz))
    bValid = m_pJob->pLogicUnit_job()->m_xpOp->bIsOperandValid(i_OperandKind);
  
  if ( ! bValid )
    RASSERT(0);
  
  m_pDataStack->vPushInt(bValid);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* op[].incHandlerCount -- inc handler count                                  */
/*----------------------------------------------------------------------------*/
void Step::vRegOpIncHandlerCount(void)
{
  ENTRY();
  
  i8 i_Handler = m_pDataStack->iPopInt(__func__);
  
  m_pJob->pLogicUnit_job()->m_xpOp->vIncHandlerCount(i_Handler);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* op[].incOperandCount -- inc operand count                                  */
/*----------------------------------------------------------------------------*/
void Step::vRegOpIncOperandCount(void)
{
  ENTRY();
  
  i8 i_OperandKind = m_pDataStack->iPopInt(__func__);
  
  m_pJob->pLogicUnit_job()->m_xpOp->vIncOperandCount(i_OperandKind);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
