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
  
  RowBuffer* pRsbBuffer = _pRowBuffer(m_pDataStack->pPopPtr(__func__));

  i8 iDetail = m_pDataStack->iPopInt(__func__);
  
  if (iDetail)
    pRsbBuffer->vDumpAllRows();
  
  pRsbBuffer->vSlabReport();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
