/*============================================================================*/
/* Code.cpp -- Code Fragments                                                 */
/*============================================================================*/

#include "BASE.h"
#include "Combined.hpp"

/*----------------------------------------------------------------------------*/
/*-- Code Fragments ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- vExprCheck---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vExprCheck
i8 vExprCheck(Combined_T* aCombinedT) {

  // marker-start

  if (aCombinedT->m_iErrorCode != (i8)EC_NONE) {
    return 0;
  }

  // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vFilterBegin ------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vFilterBegin
void vFilterBegin(Combined_T* aCombinedT) {

  char* szStrArea;
  struct SR_IT* pSR_IT;

  __L_LABEL_LOOP__:

  // marker-start
  __L_LABEL__:

  pSR_IT = (struct SR_IT*)aCombinedT->m_aRowCellArrayT;
  szStrArea = aCombinedT->m_racStrAreaT;
  if (!aCombinedT->m_iProcessRow) {
    aCombinedT->m_iProcessRow = 1;
    goto __L_LABEL_LOOP__;
  }
  // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- iFilterEnd --------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iFilterEnd
i8 iFilterEnd(Combined_T* aCombinedT, bool bFilterResult) {

  // marker-start
  if (bFilterResult && bFilterResult != C_DATUM_NULL) {
    aCombinedT->m_iProcessRow = 1;
  } else {
    aCombinedT->m_iProcessRow = 0;
  }
  // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vInflateCrwBegin --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vInflateCrwBegin
void vInflateCrwBegin(Combined_T* aCombinedT) {

  i8* paW;
  i8 i_CurrentRow;

  // marker-start
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // INFLATE-CRW
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

 L_InflateT:

 paW = &aCombinedT->m_aW[0];

 i_CurrentRow = -1;

 // process the row
 aCombinedT->m_iProcessRow = 1;

 aCombinedT->m_racStrAreaT = aCombinedT->m_racStrAreaTSav;

  // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vInflateCrwCol ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vInflateCrwCol
void vInflateCrwCol(Combined_T* aCombinedT) {

  void* aSubReturnLabel;
  u8 uMetaCode;
  u8* auMetaCode;
  u8 uHigh;
  u8 uLow;
  i8 i_Col;
  i8 __COLUMN__;

  // marker-start
  // - - - - - - - - - - - -
  i_Col = __COLUMN__;
  uMetaCode = *auMetaCode++;

  uHigh = uMetaCode >> 4;
  uLow  = uMetaCode & 0x0F;

  aSubReturnLabel = && __L_ReturnToColumn__;
  goto L_InflateAColumn;

  __L_ReturnToColumn__:

  // marker-end
  L_InflateAColumn:
    return;
}

/*----------------------------------------------------------------------------*/
/*-- vInflateCrwColLast ------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vInflateCrwColLast
void vInflateCrwColLast(Combined_T* aCombinedT) {

  char* acStrArea;
  void* aSubReturnLabel;

  // marker-start
  aSubReturnLabel = && L_InflateCrwRet;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // INFLATE-CRW-RET
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  L_InflateCrwRet:
  aCombinedT->m_racStrAreaT = acStrArea;
  goto __L_LABEL_NEXT__;

  // marker-end

  __L_LABEL_NEXT__:
  return;
}

/*----------------------------------------------------------------------------*/
/*-- vProjectBegin -----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vProjectBegin
void vProjectBegin(Combined_T* aCombinedT) {

  struct SR_IT* pSR_IT;
  struct SR_PT* pSR_PT;
  char* szStrArea;

  __L_LABEL_LOOP__:

  // marker-start
  __L_LABEL__:

  pSR_IT = (struct SR_IT*)aCombinedT->m_aRowCellArrayT;

  szStrArea = aCombinedT->m_racStrAreaT;
  if (!aCombinedT->m_iProcessRow) {
    aCombinedT->m_iProcessRow = 1;
    goto __L_LABEL_LOOP__;
  }

  pSR_PT = (struct SR_PT*)aCombinedT->m_aRowCellArrayP;

  aCombinedT->m_racStrAreaP = aCombinedT->m_racStrAreaPSav;
  szStrArea = aCombinedT->m_racStrAreaP;
  // marker-end

  return;

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
