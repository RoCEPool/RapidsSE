/*============================================================================*/
/* Steps_CrwBuffer.cpp - CrwBuffer steps group                                */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_CrwBuffer(void)
{
  vAddStep("crwBuffer.operandDump"_Stz,   &Step::vCrwBufferOperandDump);
  vAddStep("crwBuffer.snapshot"_Stz,      &Step::vCrwBufferSnapshot);
}

/*----------------------------------------------------------------------------*/
/* crwBuffer.operandDump -- dump information about crw buffer                 */
/*----------------------------------------------------------------------------*/
void Step::vCrwBufferOperandDump(void)
{
  ENTRY();

  CrwBuffer* pCrwBuffer = _pCrwBuffer(m_pDataStack->pPopPtr(__func__));

  i8 iDetail = m_pDataStack->iPopInt(__func__);
  
  pCrwBuffer->vOperandDump(iDetail);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* crwBuffer.snapshot -- take read-consistent snapshot of crw buffer          */
/*----------------------------------------------------------------------------*/
void Step::vCrwBufferSnapshot(void)
{
  ENTRY();
  
  CrwBuffer* pCrwBuffer = _pCrwBuffer(m_pDataStack->pPopPtr(__func__));

  Table* pTable = pCrwBuffer->pTable();
  i8 i_Ptn = pCrwBuffer->iPartitionNo();
  TablePtn* pTablePtn = pTable->pPartition(i_Ptn);
  
  t8 tPerspective = AM::tThisTLSPespective();

  CrwBuffer* pCrwBuffer_snaphot = pTablePtn->pSnapshot(pCrwBuffer,tPerspective);

  vTrace("\n*************************************\n"_Stz);
  vTrace("SNAPSHOT BUFFER %p (t%lld)\n"_Stz, pCrwBuffer_snaphot, tPerspective);
  vTrace("*************************************\n\n"_Stz);
  
  m_pDataStack->vPushPtr(pCrwBuffer_snaphot);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
