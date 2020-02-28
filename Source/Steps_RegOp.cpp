/*============================================================================*/
/* Steps_RegOp.cpp - op[] step group                                          */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RegOp(void)
{
  vAddStep("op[]"_Stz,         &Step::vRegOpGet);
  vAddStep("op[].handler"_Stz, &Step::vRegOpHandler);
  vAddStep("op[].init"_Stz,    &Step::vRegOpInit);
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
/* op[].handler -- get this op's handler for given operand                    */
/*----------------------------------------------------------------------------*/
void Step::vRegOpHandler(void)
{
  ENTRY(); // ENTRY("vRegOpHandler"_Stz);

  i8 i_OperandKind = m_pDataStack->iPopInt(__func__);

  Word* pHandler = m_pRegSet->m_xpOp->pOperandHandler(i_OperandKind);
    
  m_pDataStack->vPushPtr(pHandler);

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
