/*============================================================================*/
/* Steps_CsvBuffer.cpp - CsvBuffer step group                                 */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RsbBuffer(void)
{
  vAddStep("rsbBuffer.operandDump"_Stz, &Step::vRsbBufferOperandDump);
}

/*----------------------------------------------------------------------------*/
/* csvBuffer.operandDump -- operand dump                                      */
/*----------------------------------------------------------------------------*/
void Step::vRsbBufferOperandDump(void)
{
  ENTRY();
  
  RsbBuffer* pRsbBuffer = _pRsbBuffer(m_pDataStack->pPopPtr(__func__));

  pRsbBuffer->pRowSlab()->vDumpAllRows();
  pRsbBuffer->pRowSlab()->vSlabReport();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
