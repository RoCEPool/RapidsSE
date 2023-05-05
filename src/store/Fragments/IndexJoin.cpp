/*============================================================================*/
/* IndexJoin.cpp -- Code Fragments                                            */
/*============================================================================*/

#include "BASE.h"
#include "Combined.hpp"
#include "IndexDef.hpp"
#include "TccStuff.h"
#include "Pay.hpp"
#include "Key.hpp"
#include "Ent.hpp"
#include "Ent_int.h"

/*----------------------------------------------------------------------------*/
/*-- Code Fragments ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- vIndexJoinIntKey --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexJoinIntKey
void vIndexJoinIntKey(Combined_T* aCombinedT) {

  i8 iLeftOuterRow;
  i8 __KEY_KIND__;

  struct __STRUCT_FROM__* p__STRUCT_FROM__;

  __L_LABEL_LOOP__:

  // marker-start

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // INDEXJOIN-OP
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  //struct __STRUCT_FROM__ *p__STRUCT_FROM__;
  p__STRUCT_FROM__ = (struct __STRUCT_FROM__ *) aCombinedT->m_xRowInfoT.m_aRowCellArray;

  struct __STRUCT_TO__ *p__STRUCT_TO__;
  p__STRUCT_TO__ = (struct __STRUCT_TO__ *) aCombinedT->m_aRowCellArrayTJ;

  __L_LABEL__:

  // init indexJoin
  if (aCombinedT->m_iIndexJoinInit == 0) {
    vEntWrk_entKind(aCombinedT->m_aEntWrkJ, 0, __KEY_KIND__);
    iLeftOuterRow = 0;
    aCombinedT->m_iIndexJoinInit = 1;
  }

  if (!aCombinedT->m_iProcessRow) {
    aCombinedT->m_iProcessRow = 1;
    goto __L_LABEL_LOOP__;
  }

// marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vIndexJoinGenKey --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexJoinGenKey
void vIndexJoinGenKey(Combined_T* aCombinedT) {

  i8 iLeftOuterRow;
  i8 __KEY_KIND__;

  L_AdvanceNextRow:

  // marker-start

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // INDEXJOIN-OP
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  struct __STRUCT_TO__ *p__STRUCT_TO__;
  p__STRUCT_TO__ = (struct __STRUCT_TO__ *) aCombinedT->m_aRowCellArrayTJ;

  __L_LABEL__:

  // init indexJoin
  if (aCombinedT->m_iIndexJoinInit == 0) {
    vEntWrk_entKind(aCombinedT->m_aEntWrkJ, 0, __KEY_KIND__);
    iLeftOuterRow = 0;
    aCombinedT->m_iIndexJoinInit = 1;
  }
  else {
    vEntWrk_reset(aCombinedT->m_aEntWrkJ);
  }

  if (!aCombinedT->m_iProcessRow) {
    aCombinedT->m_iProcessRow = 1;
    goto L_AdvanceNextRow;
  }
// marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vIndexJoinPositionLower -------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexJoinPositionLower
void vIndexJoinPositionLower(Combined_T* aCombinedT) {

  // marker-start

  // position at entry >= join key (lbound)
  aCombinedT->m_aEntIntJ = (struct Ent_int *)aIndexTree_positionGE(aCombinedT->m_aIndexTreeJ, &aCombinedT->m_aEntBufferJ, aCombinedT->m_aEntKeyJ);

  // marker-end

  L_Return:
  return;
}

/*----------------------------------------------------------------------------*/
/*-- vIndexJoinPositionBegin -------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexJoinPositionBegin
void vIndexJoinPositionBegin(Combined_T* aCombinedT) {

  // marker-start

  // cross join - position at beginning of the index
  aCombinedT->m_aEntIntJ =  (struct Ent_int*)aIndexTree_begin(aCombinedT->m_aIndexTreeJ, &aCombinedT->m_aEntBufferJ);

  // marker-end

  L_Return:
  return;
}

/*----------------------------------------------------------------------------*/
/*-- vIndexJoinContinue ------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexJoinContinue
void vIndexJoinContinue(Combined_T* aCombinedT) {

  void *aHeadLineSav;
  void *aHeadLine;
  void *aCrwJ;

  i8 iBufferJ;
  i8 iItemJ;

  static const void *C_ADR_REGION = (void *) 0x204800000000;
  static const i8 C_CRW_HEADBYTES = 32;
  static const i8 C_CRW_FIRSTHEAD = 640;

  L_AdvanceNextRow:

  // marker-start

  printf("M1\n");
  // did we find the entry ?
  if ( ! aCombinedT->m_aEntIntJ ) {
    aCombinedT->m_iProcessRow = 0;
    goto L_AdvanceNextRow;
  }

  // find the upper bound entry
  aCombinedT->m_aEntUpperJ = aIndexTree_positionLE(aCombinedT->m_aIndexTreeJ, aCombinedT->m_aEntKeyJ);

  L_IndexJoinDeref:
  aCombinedT->m_iProcessRow = 1;

  printf("M5\n");
  if (aCombinedT->m_aEntIntJ == aCombinedT->m_aEntUpperJ) {

    aCombinedT->m_iProcessRow = 0;
    printf("M6\n");
    goto L_AdvanceNextRow;              //
  }
  printf("M7\n");
  aHeadLineSav = aHeadLine;

  // dereference the entry
  iBufferJ = aCombinedT->m_aEntIntJ->Rrf.m_uBuffer;
  iItemJ   = aCombinedT->m_aEntIntJ->Rrf.m_uItem;
  aCrwJ  = ( (char*)C_ADR_REGION + iBufferJ * C_1MB+ C_CRW_FIRSTHEAD+ iItemJ * C_CRW_HEADBYTES );
  printf("M2\n");
  // marker-end

  L_Return:
  return;

}

/*----------------------------------------------------------------------------*/
/*-- vIndexJoinReturn --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexJoinReturn
void vIndexJoinReturn(Combined_T* aCombinedT) {

  void* aHeadLine;
  void* aHeadLineSav;
  void* aSubReturnLabel;

  // marker-start

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // INFLATE-CRW-RET
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  L__IndexScanRet:
  printf("M3\n");
  aHeadLine = aHeadLineSav;
  aCombinedT->m_iJoinedRows++;
  goto __L_LABEL_NEXT__;

  __L_LABEL_LOOP__:

   aCombinedT->m_racStrAreaTJ = aCombinedT->m_racStrAreaTJSav;

   // move to next entry
   aCombinedT->m_aEntIntJ = (struct Ent_int*)aEntBuffer_nextEntry(&aCombinedT->m_aEntBufferJ, aCombinedT->m_aEntIntJ);
   goto L_IndexJoinDeref;

  printf("M4\n");
  // marker-end

  __L_LABEL_NEXT__:
  L_IndexJoinDeref:

  return;
}

/*----------------------------------------------------------------------------*/
/*-- vIndexJoinDereference ---------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexJoinDereference
void vIndexJoinDereference(Combined_T* aCombinedT) {

#include "vFLAT_Parameters.h"

  i8 iIndexBuffer;
  i8 iIndexItem;
  i8 i_BaseOffsetDataJ;
  char* aIndexCrw;

  i8 __LEFT_COLUMNS__;
  i8 __RIGHT_COLUMNS__;

  static const void* C_ADR_REGION = (void*)0x204800000000;
  static const i8 C_CRW_HEADBYTES = 32;
  static const i8 C_CRW_FIRSTHEAD = 640;

  L_InflateAColumn:

  // marker-start

  // dereference the entry
  iIndexBuffer = aCombinedT->m_aEntIntJ->Rrf.m_uBuffer;
  iIndexItem   = aCombinedT->m_aEntIntJ->Rrf.m_uItem;
  aIndexCrw    = ( (char*)C_ADR_REGION + iIndexBuffer * C_1MB + C_CRW_FIRSTHEAD+ iIndexItem * C_CRW_HEADBYTES );

  // inflate the derefenced entry
  aHeadLine = (struct HeadLine_T *)aIndexCrw;
  auMetaCode = META_AREA(aHeadLine);
  ayDstByte = (byte*)aCombinedT->m_aRowCellArrayTJ + 8*__LEFT_COLUMNS__;
  acStrArea = (char*)aCombinedT->m_racStrAreaTJ;
  racStrArea = (char*)aCombinedT->m_racStrAreaTJ;
  eStrInflateRule = SI_STRAREA_ADR;
  i_BaseOffsetDataJ = C_1B  + C_8B + __RIGHT_COLUMNS__;
  ayDataByte = ADDR_ADD(byte*,aHeadLine,i_BaseOffsetDataJ);

  // marker-end

  __L_LABEL_NEXT__:

  L_Return:
  return;

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
