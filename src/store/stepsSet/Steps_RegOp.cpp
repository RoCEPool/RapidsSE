/*============================================================================*/
/* Steps_RegOp.cpp - Op[] step group                                          */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "eYX.h"

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
  vAddStep("Op[]"_Stz,                 &Step::vRegOpGet);
  vAddStep("Op[].handlerEntry"_Stz,    &Step::vRegOpHandlerEntry);
  vAddStep("Op[].handlerExit"_Stz,     &Step::vRegOpHandlerExit);
  vAddStep("Op[].handler@"_Stz,        &Step::vRegOpHandlerFetch);
  vAddStep("Op[].handler!"_Stz,        &Step::vRegOpHandlerStore);
  vAddStep("Op[].handlerAll!"_Stz,     &Step::vRegOpHandlerAllStore);
  vAddStep("Op[].init"_Stz,            &Step::vRegOpInit);
  vAddStep("Op[].isOperandValid"_Stz,  &Step::vRegOpIsOperandValid);
  vAddStep("Op[].incHandlerCount"_Stz, &Step::vRegOpIncHandlerCount);
  vAddStep("Op[].incOperandCount"_Stz, &Step::vRegOpIncOperandCount);
  
  vAddStep("Op[].tagSet@"_Stz,         &Step::vOpTagSetFetch);

}

/*----------------------------------------------------------------------------*/
/* Op[].get -- get op                                                         */
/*----------------------------------------------------------------------------*/
void Step::vRegOpGet(void)
{
  ENTRY();
    
  m_pDataStack->vPushPtr(*m_p_TaskLogicUnit->m_xpOp);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op[].handlerEntry -- handler entry                                         */
/*----------------------------------------------------------------------------*/
void Step::vRegOpHandlerEntry(void)
{
  ENTRY();
  
  if (AM::TRACE::bVBlong()) {

  // revised job/op model
    Op* pOp = *m_pOp;
    
    if (m_p_TaskLogicUnit->m_xpPipein.bIsNotNull()) {
      Operand* pOperand = *m_p_TaskLogicUnit->m_xpPipein;
    
      pOp->vOpTrace(pOperand,(Stz*)__func__,true);
    }
    
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op[].handlerExit -- handler exit                                           */
/*----------------------------------------------------------------------------*/
void Step::vRegOpHandlerExit(void)
{
  ENTRY();
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op[].handler@ -- get this op's handler for given operand                   */
/*----------------------------------------------------------------------------*/
void Step::vRegOpHandlerFetch(void)
{
  ENTRY();

  i8 i_OperandKind = m_pDataStack->iPopInt(__func__);

  Word* pHandler = m_p_TaskLogicUnit->m_xpOp->pEventHandler(i_OperandKind);
    
  m_pDataStack->vPushPtr(pHandler);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op[].handler! -- set this op's handler for given operand                   */
/*----------------------------------------------------------------------------*/
void Step::vRegOpHandlerStore(void)
{
  ENTRY();

  i8 i_OpKind = m_pDataStack->iPopInt(__func__);
  
  Word* pHandler = _pWord(m_pDataStack->pPopPtr(__func__));

  m_p_TaskLogicUnit->m_xpOp->vEventHandler(i_OpKind,pHandler);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op[].handlerAll! -- set this op's handler for given operand                */
/*----------------------------------------------------------------------------*/
void Step::vRegOpHandlerAllStore(void)
{
  ENTRY();

  Word* pHandler = _pWord(m_pDataStack->pPopPtr(__func__));

  for (i8 i_OpKind = 0; i_OpKind < ZK_ENUMS; i_OpKind++)
    m_p_TaskLogicUnit->m_xpOp->vEventHandler(i_OpKind,pHandler);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op[].init -- init op                                                       */
/*----------------------------------------------------------------------------*/
void Step::vRegOpInit(void)
{
  ENTRY();

  // revised job/op model
  Op* pOp = *m_pOp;

  m_p_TaskLogicUnit->m_xpOp = pOp;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op[].isOperandValid -- check if operand is valid                           */
/*----------------------------------------------------------------------------*/
void Step::vRegOpIsOperandValid(void)
{
  ENTRY();
  
  bool bValid = true;
  
  i8 i_OperandKind = m_pDataStack->iPopInt(__func__);
  
  // skip consumeOp operand validation
  if ( ! m_p_TaskLogicUnit->m_xpOp->szOpNm()->bSame("consumeOP"_Stz))
    bValid = m_p_TaskLogicUnit->m_xpOp->bIsOperandValid(i_OperandKind);
  
  if ( ! bValid )
    RASSERT(0);
  
  m_pDataStack->vPushInt(bValid);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op[].incHandlerCount -- inc handler count                                  */
/*----------------------------------------------------------------------------*/
void Step::vRegOpIncHandlerCount(void)
{
  ENTRY();
  
  eZK eEvent = (eZK)m_pDataStack->iPopInt(__func__);
  
  m_p_TaskLogicUnit->m_xpOp->vIncHandlerCount(eEvent);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Op[].incOperandCount -- inc operand count                                  */
/*----------------------------------------------------------------------------*/
void Step::vRegOpIncOperandCount(void)
{
  ENTRY();
  
  eZK eEvent = (eZK) m_pDataStack->iPopInt(__func__);
  
  m_p_TaskLogicUnit->m_xpOp->vIncOperandCount(eEvent);
  
  EXIT();
}




/*----------------------------------------------------------------------------*/
/* Op[].tagSet@ -- push ptr to op TagSet                                       */
/*----------------------------------------------------------------------------*/
void Step::vOpTagSetFetch(void)
{
  ENTRY();
  
  Op* pOp = *m_pOp;

  m_pDataStack->vPushPtr(pOp->pOpTags());
  
  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
