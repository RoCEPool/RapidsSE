/*============================================================================*/
/* Steps_RegOp.cpp - op[] step group                                          */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RegOp(void)
{
  vAddStep("op[]"_Stz,             &Step::vRegOpGet);
  vAddStep("op[].handler@"_Stz,    &Step::vRegOpHandlerFetch);
  vAddStep("op[].handler!"_Stz,    &Step::vRegOpHandlerStore);
  vAddStep("op[].handlerAll!"_Stz, &Step::vRegOpHandlerAllStore);
  vAddStep("op[].init"_Stz,        &Step::vRegOpInit);
  vAddStep("op[].isOperandValid"_Stz,  &Step::vRegOpIsOperandValid);
  vAddStep("op[].incOperandCount"_Stz, &Step::vRegOpIncOperandCount);
}

/*----------------------------------------------------------------------------*/
/* op[].get -- get op                                                         */
/*----------------------------------------------------------------------------*/
void Step::vRegOpGet(void)
{
  ENTRY();
    
  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpOp);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* op[].handler@ -- get this op's handler for given operand                   */
/*----------------------------------------------------------------------------*/
void Step::vRegOpHandlerFetch(void)
{
  ENTRY();

  i8 i_OperandKind = m_pDataStack->iPopInt(__func__);

  Word* pHandler = m_pLogicUnit->m_xpOp->pOperandHandler(i_OperandKind);
    
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

  m_pLogicUnit->m_xpOp->vOperandHandler(i_OpKind,pHandler);
    
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
    m_pLogicUnit->m_xpOp->vOperandHandler(i_OpKind,pHandler);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* op[].init -- init op                                                       */
/*----------------------------------------------------------------------------*/
void Step::vRegOpInit(void)
{
  ENTRY();

  Op* pOp = m_pTask->m_pJob->m_pOp;

  m_pLogicUnit->m_xpOp = pOp;

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
  if ( ! m_pLogicUnit->m_xpOp->szOpNm()->bSame("consumeOP"_Stz))
    bValid = m_pLogicUnit->m_xpOp->bIsOperandValid(i_OperandKind);
  
  m_pDataStack->vPushInt(bValid);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* op[].IncOperandCount -- inc operand count                                  */
/*----------------------------------------------------------------------------*/
void Step::vRegOpIncOperandCount(void)
{
  ENTRY();
  
  i8 i_OperandKind = m_pDataStack->iPopInt(__func__);
  
  m_pLogicUnit->m_xpOp->vIncOperandCount(i_OperandKind);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
