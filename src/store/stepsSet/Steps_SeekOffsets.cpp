/*============================================================================*/
/* Steps_SeekOffsets.cpp - SeekOffsets step group                             */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_SeekOffsets(void)
{
//  vAddStep("seekOffsets.operandDump"_Stz,     &Step::vSeekOffsetsOperandDump);
}

/*----------------------------------------------------------------------------*/
/* seekOffsets.operandDump -- operand dump of seek offsets info set           */
/*----------------------------------------------------------------------------*/
//void Step::vSeekOffsetsOperandDump(void)
//{
//  ENTRY();
//
//	RowBuffer* pRowBuffer = _pRowBuffer(m_oDataStack.pPopPtr(__func__));
//
//  i8 iDetail = m_oDataStack.iPopInt(__func__);
//
//  if (iDetail) {
//    i8 i_Ofs = 0;
//    for (Row* pRow = pRowBuffer->pFirstRow();
//         pRow;
//         pRow = pRowBuffer->pNextRow(pRow)) {
//
//      i8 iOffset = pRow->iColVal_int(0);
//      i8 nBytes  = pRow->iColVal_int(1);
//
//      vTrace("[%lld] Offset %lld Bytes %lld\n"_Stz,
//             i_Ofs++,iOffset,nBytes);
//    }
//  }
//
//  vTrace("%s : %lld rows"_Stz,
//				 pRowBuffer->szBufferNm(),
//				 pRowBuffer->nRows());
//
//  EXIT();
//}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
