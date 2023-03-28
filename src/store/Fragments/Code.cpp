/*============================================================================*/
/* Code.cpp -- Code Fragments                                                 */
/*============================================================================*/

#include "BASE.h"
#include "Combined.hpp"
#include "TccStuff.h"

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
  struct __STRUCT_FROM__* p__STRUCT_FROM__;

  __L_LABEL_LOOP__:

  // marker-start

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // FILTER-OP
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

__L_LABEL__:

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

  struct __STRUCT_TO__* p__STRUCT_TO__;

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

  char* szStrArea;
  char* acStrArea;
  void* aSubReturnLabel;
  struct __STRUCT_TO__* p__STRUCT_TO__;

  // marker-start
  aSubReturnLabel = && L_InflateCrwRet;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // INFLATE-CRW-RET
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  L_InflateCrwRet:

  aCombinedT->m_racStrAreaT = acStrArea;
  p__STRUCT_TO__ = (struct __STRUCT_TO__*)aCombinedT->m_aRowCellArrayT;
  szStrArea = aCombinedT->m_racStrAreaT;

  // marker-end

  return;
}

/*----------------------------------------------------------------------------*/
/*-- vProjectBegin -----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vProjectBegin
void vProjectBegin(Combined_T* aCombinedT) {

  struct __STRUCT_FROM__* p__STRUCT_FROM__;

  char* szStrArea;

  __L_LABEL_LOOP__:

  // marker-start



  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // PROJECT-OP
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  struct __STRUCT_TO__* p__STRUCT_TO__;

  __L_LABEL__:

  if (!aCombinedT->m_iProcessRow) {
    aCombinedT->m_iProcessRow = 1;
    goto __L_LABEL_LOOP__;
  }

  aCombinedT->m_racStrAreaP = aCombinedT->m_racStrAreaPSav;
  p__STRUCT_TO__ = (struct __STRUCT_TO__*)aCombinedT->m_aRowCellArrayP;
  szStrArea = aCombinedT->m_racStrAreaP;

  // marker-end

  return;

}

/*----------------------------------------------------------------------------*/
/*-- vProjectEnd -------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vProjectEnd
i8 iProjectEnd(Combined_T* aCombinedT) {

  struct __STRUCT_TO__*   p__STRUCT_TO__;

  char* szStrArea;

  // marker-start

  if (aCombinedT->m_iErrorCode != (i8)EC_NONE) {
    return 0;
  }

  p__STRUCT_TO__ = (struct __STRUCT_TO__*)aCombinedT->m_aRowCellArrayP;
  szStrArea = aCombinedT->m_racStrAreaP;

  // marker-end

  return 0;

}

/*----------------------------------------------------------------------------*/
/*-- vTubBegin ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iTubBegin
i8 iTubBegin(Combined_T* aCombinedT) {

  struct __STRUCT_TO__ {
    i8 iDummy;
  };

  char* szStrArea;

  __L_LABEL_LOOP__:

  // marker-start


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // TUB
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  struct __STRUCT_TO__* p__STRUCT_TO__;

  __L_LABEL__:

  if (!aCombinedT->m_iProcessRow) {
    aCombinedT->m_iProcessRow = 1;
    goto __L_LABEL_LOOP__;
  }

  // allocate a row from the tub
#ifdef WITH_TINYC
  aCombinedT->m_aRowTub = (byte*)aTub_AllotRow((void*)aCombinedT->m_pTub,&aCombinedT->m_racTubStrArea,&aCombinedT->m_iErrorCode);
#endif
  if (aCombinedT->m_iErrorCode != (i8)EC_NONE) {
    return 0;
  }

  p__STRUCT_TO__ = (struct __STRUCT_TO__*)aCombinedT->m_aRowTub;

  // marker-end

  return 0;
}

/*----------------------------------------------------------------------------*/
/*-- vTubCol -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vTubIntCol
void vTubIntCol(Combined_T* aCombinedT) {

  struct __STRUCT_FROM__ {
    union Int_T __COLUMN_FROM__;
  };

  struct __STRUCT_FROM__* p__STRUCT_FROM__;

  struct __STRUCT_TO__ {
    union Int_T __COLUMN_TO__;
  };

  struct __STRUCT_TO__* p__STRUCT_TO__;

  // marker-start
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // column __COLUMN_NO__

  p__STRUCT_TO__->__COLUMN_TO__.m_iAsI8 = p__STRUCT_FROM__->__COLUMN_FROM__.m_iAsI8;

  // marker-end
}

// function-vTubVchCol
void vTubVchCol(Combined_T* aCombinedT) {

  struct __STRUCT_FROM__ {
    union Vch_T __COLUMN_FROM__;
  };

  struct __STRUCT_FROM__* p__STRUCT_FROM__;

  struct __STRUCT_TO__ {
    union Vch_T __COLUMN_TO__;
  };

  struct __STRUCT_TO__* p__STRUCT_TO__;

  char* szSrcStz;
  char* szDstStz;

  i8  nSrcSize;
  i8  iOffset;
  m8* amCellAddress;

  // marker-start
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // column __COLUMN_NO__

  // get the source string
  szSrcStz = (char*)p__STRUCT_FROM__->__COLUMN_FROM__.m_iOffset;
  if (szSrcStz == (char*)C_DATUM_NULL) {
    // copy the source to the destination
    p__STRUCT_TO__->__COLUMN_TO__.m_iOffset = C_DATUM_NULL;
  }
  else {
    nSrcSize = strlen(szSrcStz) + 1;

    // get the destination string
    szDstStz = aCombinedT->m_racTubStrArea;
    strncpy(szDstStz, szSrcStz, nSrcSize);

    // copy the source to the destination
    amCellAddress = (m8*) &p__STRUCT_TO__->__COLUMN_TO__.m_iOffset;
    iOffset = - nDifference(aCombinedT->m_racTubStrArea,amCellAddress);
    p__STRUCT_TO__->__COLUMN_TO__.m_iOffset = iOffset;

    // update the tub string size
    aCombinedT->m_racTubStrArea += nSrcSize;
#ifdef WITH_TINYC
    vTub_RowStrSize(aCombinedT->m_pTub,nSrcSize);
#endif
  }

  // marker-end
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
