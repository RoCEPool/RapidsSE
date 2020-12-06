/*============================================================================*/
/* Steps_RowBuffer.cpp - RowBuffer step group                                 */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RowBuffer(void)
{
  vAddStep("rowBuffer.operandDump"_Stz, &Step::vRowBufferOperandDump);
}

/*----------------------------------------------------------------------------*/
/* rowBuffer.operandDump -- operand dump of rowBuffer                         */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferOperandDump(void)
{
  ENTRY();
  
  RowBuffer* pRowBuffer = _pRowBuffer(m_pDataStack->pPopPtr(__func__));

  i8 iDetail = m_pDataStack->iPopInt(__func__);
  
  if (iDetail)
    pRowBuffer->vDumpAllRows();
  
  pRowBuffer->vSlabReport();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
