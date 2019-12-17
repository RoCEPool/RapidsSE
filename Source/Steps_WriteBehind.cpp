/*============================================================================*/
/* Steps_Operand.cpp -- Operand step group                                    */
/*============================================================================*/

#include "Steps.h"
#include "ePX.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_WriteBehind(void)
{
  vAddStep("WriteBehind[].create"_Stz,   &Step::vWriteBehindCreate);
  vAddStep("WriteBehind[].get"_Stz,      &Step::vWriteBehindGet);
  vAddStep("WriteBehind[].pipeout"_Stz,  &Step::vWriteBehindPipeOut);
  vAddStep("WriteBehind[].set"_Stz,      &Step::vWriteBehindset);
}

/*----------------------------------------------------------------------------*/
/* WriteBehind[].create -- create the writeBehind                             */
/*----------------------------------------------------------------------------*/
void Step::vWriteBehindCreate(void)
{
  ENTRY(); // ENTRY("vWriteBehindGet"_Stz);
  
  WriteBehind* pWriteBehind = new WriteBehind(DS_INSERT);
  pWriteBehind->m_pTable = *m_pRegSet->m_xpTable;
  pWriteBehind->m_oFormatters.vSet(AM::pFmt());
  pWriteBehind->m_oFormatters.m_cRecordTerminator = '\n';
  m_pRegSet->m_xpWriteBehind = pWriteBehind;
}

/*----------------------------------------------------------------------------*/
/* WriteBehind[].get -- get the writeBehind                                   */
/*----------------------------------------------------------------------------*/
void Step::vWriteBehindGet(void)
{
  ENTRY(); // ENTRY("vWriteBehindGet"_Stz);
  
  m_pDataStack->vPushPtr(*m_pRegSet->m_xpWriteBehind);
}

/*----------------------------------------------------------------------------*/
/* WriteBehind[].pipeout -- pipeout the writeBehind operands                  */
/*----------------------------------------------------------------------------*/
void Step::vWriteBehindPipeOut(void)
{
  ENTRY(); // ENTRY("vWriteBehindPipeOut"_Stz);
  
  Op* pOp = m_pTask->m_pJob->m_pOp;
  WriteBehind* pWriteBehind = *m_pRegSet->m_xpWriteBehind;
  
  for (i8 i_Ptn = 0; i_Ptn < AM::nPartitions(); i_Ptn++) {
    
    CrwBuffer* pCrwBuffer = pWriteBehind->pCrwBuffer(i_Ptn);
    if (pCrwBuffer) {
    
      // close the append
      pCrwBuffer->vAppendClose();
      pOp->vPipeOut(pCrwBuffer);
    }
    
  }
}

/*----------------------------------------------------------------------------*/
/* WriteBehind[].set -- set the writeBehind                                   */
/*----------------------------------------------------------------------------*/
void Step::vWriteBehindset(void)
{
  ENTRY(); // ENTRY("vWriteBehindset"_Stz);
  
  m_pRegSet->m_xpWriteBehind = _pWriteBehind(m_pDataStack->pPopPtr(__func__));
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
