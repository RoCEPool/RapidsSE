/*============================================================================*/
/* Steps_JobWriteRow.cpp - job step group                                    */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_JobWriteRow(void)
{
  vAddStep("job_writeRow.insertRow"_Stz,     &Step::vJob_WriteRowInsertRow);
  vAddStep("job_writeRow.encodeRow"_Stz,     &Step::vJob_WriteRowEncodeRow);
}

/*----------------------------------------------------------------------------*/
/* job_writeRow.encodeRow -- hash row to a partition                            */
/*----------------------------------------------------------------------------*/
void Step::vJob_WriteRowEncodeRow(void)
{
  ENTRY();
  
//  Job_WriteRow* pJob_WriteRow = _pJob_WriteRow(m_pDataStack->pPopPtr(__func__));
//
//  Job_InsertRow* pJob_InsertRow = _pJob_InsertRow(m_pDataStack->pPopPtr(__func__));
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // first encode the row
//  bool bBlankLine;
//  //vTrace("sRecord %s\n"_Stz, pJob_InsertRow->m_pRecord->szRecStr());
//  bool bResult = pJob_WriteRow->bExtractRow(pJob_InsertRow->m_pRecord->szRecStr(),
//                                            bBlankLine);
//
//  if ( ! bResult )
//    m_pDataStack->vPushInt(0);
//  else {
//    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//    // use row hash to get partition
//    i8 iHash = pJob_WriteRow->m_pRow->m_iHash;
//    Table* pTable = pJob_WriteRow->m_pTable;
//    pJob_WriteRow->m_iPartitionId = pTable->iToPartition(iHash);
//    pJob_InsertRow->m_iPartitionId = pJob_WriteRow->m_iPartitionId ;
//
//    // now delete the rowCompactors and the row from writeRow job before
//    // forwarding to another node
//    delete pJob_WriteRow->m_pRow;
//    pJob_WriteRow->m_pRow = nullptr;
//
//    for (i8 i_Ptn = 0; i_Ptn < AM::pGrid()->nPartitions(); i_Ptn++) {
//      delete pJob_WriteRow->m_paRowCompactor[i_Ptn];
//      pJob_WriteRow->m_paRowCompactor[i_Ptn] = nullptr;
//    }
//
//    m_pDataStack->vPushInt(1);
//  }
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* job_WriteRow.insertRow -- insert row into the table                        */
/*----------------------------------------------------------------------------*/
void Step::vJob_WriteRowInsertRow(void)
{
  ENTRY();
  
//  Job_WriteRow* pJob_WriteRow = _pJob_WriteRow(m_pDataStack->pPopPtr(__func__));
//
//  pJob_WriteRow->vInsertRow();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
