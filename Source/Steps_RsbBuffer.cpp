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
/* rsbBuffer.operandDump -- operand dump of rsbBuffer                         */
/*----------------------------------------------------------------------------*/
void Step::vRsbBufferOperandDump(void)
{
  ENTRY();
  
  RsbBuffer* pRsbBuffer = _pRsbBuffer(m_pDataStack->pPopPtr(__func__));

  i8 iDetail = m_pDataStack->iPopInt(__func__);
  
  pRsbBuffer->pRowSlab()->vDumpAllRows();
  pRsbBuffer->pRowSlab()->vSlabReport();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
