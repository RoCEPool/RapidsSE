/*============================================================================*/
/* Steps_Operand.cpp -- Operand step group                                    */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_WriteBehind(void)
{
  vAddStep("WriteBehind[].create"_Stz,         &Step::vWriteBehindCreate);
  vAddStep("WriteBehind[].delete"_Stz,         &Step::vWriteBehindDelete);
  vAddStep("WriteBehind[].get"_Stz,            &Step::vWriteBehindGet);
  vAddStep("WriteBehind[].handleDangling"_Stz, &Step::vWriteBehindHandleDangling);
  vAddStep("WriteBehind[].set"_Stz,            &Step::vWriteBehindset);
}

/*----------------------------------------------------------------------------*/
/* WriteBehind[].create -- create the writeBehind                             */
/*----------------------------------------------------------------------------*/
void Step::vWriteBehindCreate(void)
{
  ENTRY();
  
  WriteBehind* pWriteBehind = new WriteBehind(*m_pRegSet->m_xpTable);
  
  m_pRegSet->m_xpWriteBehind = pWriteBehind;
}

/*----------------------------------------------------------------------------*/
/* WriteBehind[].delete -- delete the writeBehind                             */
/*----------------------------------------------------------------------------*/
void Step::vWriteBehindDelete(void)
{
  ENTRY();
  
  WriteBehind* pWriteBehind = *m_pRegSet->m_xpWriteBehind;
  delete pWriteBehind;
  
  m_pRegSet->m_xpWriteBehind = nullptr;
}

/*----------------------------------------------------------------------------*/
/* WriteBehind[].get -- get the writeBehind                                   */
/*----------------------------------------------------------------------------*/
void Step::vWriteBehindGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pRegSet->m_xpWriteBehind);
}

/*----------------------------------------------------------------------------*/
/* WriteBehind[].handleDangling -- pipeout the writeBehind dangling operands  */
/*----------------------------------------------------------------------------*/
void Step::vWriteBehindHandleDangling(void)
{
  ENTRY();
  
  Op* pOp = m_pTask->m_pJob->m_pOp;
  WriteBehind* pWriteBehind = *m_pRegSet->m_xpWriteBehind;
  
  for (i8 i_Ptn = 0; i_Ptn < AM::nPartitions(); i_Ptn++) {
    
    CrwBuffer* pCrwBuffer = pWriteBehind->pCrwBuffer(i_Ptn);
    
    if (pCrwBuffer) {
      // close the append
      pCrwBuffer->vAppendClose();
      pOp->vPipeOut_nextOp(pCrwBuffer,m_pTask);
    }
    pWriteBehind->vCrwBuffer(i_Ptn, nullptr);
    
  }
}

/*----------------------------------------------------------------------------*/
/* WriteBehind[].set -- set the writeBehind                                   */
/*----------------------------------------------------------------------------*/
void Step::vWriteBehindset(void)
{
  ENTRY();
  
  m_pRegSet->m_xpWriteBehind = _pWriteBehind(m_pDataStack->pPopPtr(__func__));
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
