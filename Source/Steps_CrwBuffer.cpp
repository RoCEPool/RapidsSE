/*============================================================================*/
/* Steps_CrwBuffer.cpp - CrwBuffer steps group                                */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_CrwBuffer(void)
{
  vAddStep("crwBuffer.snapshot"_Stz,      &Step::vCrwBufferSnapshot);
}

/*----------------------------------------------------------------------------*/
/* crwBuffer.snapshot -- take read-consistent snapshot of crw buffer          */
/*----------------------------------------------------------------------------*/
void Step::vCrwBufferSnapshot(void)
{
  ENTRY(); // ENTRY("vCrwBufferSnapshot"_Stz);
  
  CrwBuffer* pCrwBuffer = _pCrwBuffer(m_pDataStack->pPopPtr(__func__));

  Table* pTable = pCrwBuffer->pTable();
  i8 i_Ptn = pCrwBuffer->iPartitionNo();
  TablePtn* pTablePtn = pTable->pPartition(i_Ptn);
  
  t8 tperspective = AM::tThisTLSPespective();

  CrwBuffer* pCrwBuffer_snaphot = pCrwBuffer->pSnapshot(tperspective);

  m_pDataStack->vPushPtr(pCrwBuffer_snaphot);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
