/*============================================================================*/
/* Steps_SeekOffsets.cpp - SeekOffsets step group                             */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_SeekOffsets(void)
{
  vAddStep("seekOffsets.operandDump"_Stz,     &Step::vSeekOffsetsOperandDump);
}

/*----------------------------------------------------------------------------*/
/* seekOffsets.operandDump -- operand dump of seek offsets info set           */
/*----------------------------------------------------------------------------*/
void Step::vSeekOffsetsOperandDump(void)
{
  ENTRY();
  
  InfoSet* pInfoSet = _pInfoSet(m_pDataStack->pPopPtr(__func__));

  i8 iDetail = m_pDataStack->iPopInt(__func__);
  
  if (iDetail) {
    i8 i_Ofs = 0;
    for (Row* pRow = pInfoSet->aFirstRow();
         pRow;
         pRow = pInfoSet->aNextRow(pRow)) {
      
      i8 iOffset = pRow->m_waColVal[0];
      i8 nBytes  = pRow->m_waColVal[1];

      vTrace("[%lld] Offset %lld Bytes %lld\n"_Stz,
             i_Ofs++,iOffset,nBytes);
    }
  }
  
  vTrace("%s : %lld rows"_Stz,
         pInfoSet->szInfoSetNm(),
         pInfoSet->nInfoRows());
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
