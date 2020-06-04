/*============================================================================*/
/* Steps_CsvBuffer.cpp - CsvBuffer step group                                 */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_CsvBuffer(void)
{
  vAddStep("csvBuffer::new"_Stz,        &Step::vCsvBufferNew);
  
  vAddStep("csvBuffer.delete"_Stz,      &Step::vCsvBufferDelete);
  vAddStep("csvBuffer.operandDump"_Stz, &Step::vCsvBufferOperandDump);
  vAddStep("csvBuffer.print"_Stz,       &Step::vCsvBufferPrint);
}

/*----------------------------------------------------------------------------*/
/* csvBuffer::new -- create a new csv buffer                                  */
/*----------------------------------------------------------------------------*/
void Step::vCsvBufferNew(void)
{
  ENTRY();
  
  CsvBuffer* pCsvBuffer = new CsvBuffer;
    
  m_pDataStack->vPushPtr(pCsvBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* csvBuffer.delete -- delete                                                 */
/*----------------------------------------------------------------------------*/
void Step::vCsvBufferDelete(void)
{
  ENTRY();
  
  CsvBuffer* pCsvBuffer = _pCsvBuffer(m_pDataStack->pPopPtr(__func__));

  delete pCsvBuffer;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* csvBuffer.operandDump -- operand dump                                      */
/*----------------------------------------------------------------------------*/
void Step::vCsvBufferOperandDump(void)
{
  ENTRY();
  
  CsvBuffer* pCsvBuffer = _pCsvBuffer(m_pDataStack->pPopPtr(__func__));

  pCsvBuffer->vPrintRecords();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* csvBuffer.print -- print                                                   */
/*----------------------------------------------------------------------------*/
void Step::vCsvBufferPrint(void)
{
  ENTRY();
  
  CsvBuffer* pCsvBuffer = _pCsvBuffer(m_pDataStack->pPopPtr(__func__));

  pCsvBuffer->vPrintRecords();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
