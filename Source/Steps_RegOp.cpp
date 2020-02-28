/*============================================================================*/
/* Steps_RegOp.cpp - op[] step group                                          */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RegOp(void)
{
  vAddStep("op[]"_Stz,          &Step::vRegOpGet);
  vAddStep("op[].handler@"_Stz, &Step::vRegOpHandlerFetch);
  vAddStep("op[].handler!"_Stz, &Step::vRegOpHandlerStore);
  vAddStep("op[].init"_Stz,     &Step::vRegOpInit);
}

/*----------------------------------------------------------------------------*/
/* op[].get -- get op                                                         */
/*----------------------------------------------------------------------------*/
void Step::vRegOpGet(void)
{
  ENTRY(); // ENTRY("vRegOpGet"_Stz);
    
  m_pDataStack->vPushPtr(*m_pRegSet->m_xpOp);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* op[].handler@ -- get this op's handler for given operand                   */
/*----------------------------------------------------------------------------*/
void Step::vRegOpHandlerFetch(void)
{
  ENTRY(); // ENTRY("vRegOpHandlerFetch"_Stz);

  i8 i_OperandKind = m_pDataStack->iPopInt(__func__);

  Word* pHandler = m_pRegSet->m_xpOp->pOperandHandler(i_OperandKind);
    
  m_pDataStack->vPushPtr(pHandler);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* op[].handler! -- set this op's handler for given operand                   */
/*----------------------------------------------------------------------------*/
void Step::vRegOpHandlerStore(void)
{
  ENTRY(); // ENTRY("vRegOpHandlerStore"_Stz);

  Word* pHandler   = _pWord(m_pDataStack->pPopPtr(__func__));
  i8 i_OperandKind = m_pDataStack->iPopInt(__func__);

  m_pRegSet->m_xpOp->vOperandHandler(i_OperandKind,pHandler);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* op[].init -- init op                                                       */
/*----------------------------------------------------------------------------*/
void Step::vRegOpInit(void)
{
  ENTRY(); // ENTRY("vRegOpInit"_Stz);

  Op* pOp = m_pTask->m_pJob->m_pOp;

  m_pRegSet->m_xpOp = pOp;

  EXIT();
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
