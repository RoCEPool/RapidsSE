/*============================================================================*/
/* IndexJoin.cpp -- Code Fragments                                            */
/*============================================================================*/

#include "BASE.h"
#include "Dynamic.hpp"
#include "IndexDef.hpp"
#include "CodeSynthCommon.h"
#include "ClangDynamic.h"
#include "ClangStuff.h"
#include "Pay.hpp"
#include "Key.hpp"
#include "Ent.hpp"
#include "Ent_int.h"

/*----------------------------------------------------------------------------*/
/*-- Code Fragments ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- vSemiJoinEnd ------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vSemiJoinEnd
void vSemiJoinEnd(Dynamic_T* aDynamicT) {

  struct __STRUCT_FROM__* p__STRUCT_FROM__;
  RowInfo __ROW_INFO_FROM__;

  __L_LABEL_INDEX_DEREF__:
  __L_LABEL_LOOP__:

  // marker-start
  if ( ! aDynamicT->m_iProcessRow ) {
#ifdef WITH_CLANG
    // move to next entry
    aDynamicT->m_xIndexInfoJ.m_aEntInt = (struct Ent_int*)aExtern_EntBufferNextEntry(
        &aDynamicT->m_xIndexInfoJ.m_aEntBuffer, aDynamicT->m_xIndexInfoJ.m_aEntInt);
#endif
    goto __L_LABEL_INDEX_DEREF__;
 }

// marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vAntiJoinEnd ------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vAntiJoinEnd
void vAntiJoinEnd(Dynamic_T* aDynamicT) {

  struct __STRUCT_FROM__* p__STRUCT_FROM__;
  RowInfo __ROW_INFO_FROM__;

  __L_LABEL_INDEX_DEREF__:
  __L_LABEL_LOOP__:

  // marker-start
  if ( aDynamicT->m_iProcessRow ) {
#ifdef WITH_CLANG

    aDynamicT->m_iProcessRow = 0;

    // move to next entry
    aDynamicT->m_xIndexInfoJ.m_aEntInt = (struct Ent_int*)aExtern_EntBufferNextEntry(
        &aDynamicT->m_xIndexInfoJ.m_aEntBuffer, aDynamicT->m_xIndexInfoJ.m_aEntInt);
#endif
    goto __L_LABEL_INDEX_DEREF__;
  }
  else {
    aDynamicT->m_iProcessRow = 1;
  }

// marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vIndexJoinIntKey --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexJoinIntKey
void vIndexJoinIntKey(Dynamic_T* aDynamicT) {

  i8 iLeftOuterRow;
  i8 __KEY_KIND__;

  struct __STRUCT_FROM__* p__STRUCT_FROM__;
  RowInfo __ROW_INFO_FROM__;

  __L_LABEL_OUTER_LOOP__:

  // marker-start

  // init indexJoin
  if (aDynamicT->m_xIndexInfoJ.m_iInitialize) {
#ifdef WITH_CLANG
    strcpy(&aDynamicT->m_xIndexInfoJ.m_cTableName[0], "__TABLENAME__");
    strcpy(&aDynamicT->m_xIndexInfoJ.m_cIndexName[0], "__INDEXNAME__");

    aDynamicT->m_xIndexInfoJ.m_aTable = aExtern_FindTable(aDynamicT->m_xIndexInfoJ.m_cTableName);
    aDynamicT->m_xIndexInfoJ.m_aIndex = aExtern_FindIndex(aDynamicT->m_xIndexInfoJ.m_aTable, aDynamicT->m_xIndexInfoJ.m_cIndexName);
    aDynamicT->m_xIndexInfoJ.m_aIndexDef = aExtern_GetIndexDef(aDynamicT->m_xIndexInfoJ.m_aIndex);
    aDynamicT->m_xIndexInfoJ.m_aIndexTree = aExtern_GetIndexTree(aDynamicT->m_xIndexInfoJ.m_aTable, aDynamicT->m_xIndexInfoJ.m_aIndex);

    vExtern_EntWrkEntKind(aDynamicT->m_xIndexInfoJ.m_aEntWrk, 0, __KEY_KIND__);
#endif
    iLeftOuterRow = 0;
    aDynamicT->m_xIndexInfoJ.m_iInitialize = 0;
  }

  if (!aDynamicT->m_iProcessRow) {
    aDynamicT->m_iProcessRow = 1;
    goto __L_LABEL_OUTER_LOOP__;
  }

// marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vIndexJoinGenKey --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexJoinGenKey
void vIndexJoinGenKey(Dynamic_T* aDynamicT) {

  i8 iLeftOuterRow;
  i8 __KEY_KIND__;

  __L_LABEL_OUTER_LOOP__:

  // marker-start

  // init indexJoin
  if (aDynamicT->m_xIndexInfoJ.m_iInitialize) {
#ifdef WITH_CLANG
    strcpy(&aDynamicT->m_xIndexInfoJ.m_cTableName[0], "__TABLENAME__");
    strcpy(&aDynamicT->m_xIndexInfoJ.m_cIndexName[0], "__INDEXNAME__");

    aDynamicT->m_xIndexInfoJ.m_aTable = aExtern_FindTable(aDynamicT->m_xIndexInfoJ.m_cTableName);
    aDynamicT->m_xIndexInfoJ.m_aIndex = aExtern_FindIndex(aDynamicT->m_xIndexInfoJ.m_aTable, aDynamicT->m_xIndexInfoJ.m_cIndexName);
    aDynamicT->m_xIndexInfoJ.m_aIndexDef = aExtern_GetIndexDef(aDynamicT->m_xIndexInfoJ.m_aIndex);
    aDynamicT->m_xIndexInfoJ.m_aIndexTree = aExtern_GetIndexTree(aDynamicT->m_xIndexInfoJ.m_aTable, aDynamicT->m_xIndexInfoJ.m_aIndex);

    vExtern_EntWrkEntKind(aDynamicT->m_xIndexInfoJ.m_aEntWrk, 0, __KEY_KIND__);
#endif
    iLeftOuterRow = 0;
    aDynamicT->m_xIndexInfoJ.m_iInitialize = 0;
  }
  else {
#ifdef WITH_CLANG
    vExtern_EntWrkReset(aDynamicT->m_xIndexInfoJ.m_aEntWrk);
#endif
  }

  if (!aDynamicT->m_iProcessRow) {
    aDynamicT->m_iProcessRow = 1;
    goto __L_LABEL_OUTER_LOOP__;
  }

// marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vIndexJoinContinue ------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexJoinContinue
void vIndexJoinContinue(Dynamic_T* aDynamicT) {

  void *aHeadLineSav;
  void *aHeadLine;
  void *aCrwJ;

  i8 iBufferJ;
  i8 iItemJ;

  static const void *C_ADR_REGION = (void *) 0x204800000000;
  static const i8 C_CRW_HEADBYTES = 32;
  static const i8 C_CRW_FIRSTHEAD = 640;

  __L_LABEL_OUTER_LOOP__:
  __L_LABEL_LOOP__:

  // marker-start

  // did we find the entry ?
  if ( ! aDynamicT->m_xIndexInfoJ.m_aEntInt ) {
    aDynamicT->m_iProcessRow = 0;
    goto __L_LABEL_OUTER_LOOP__;
  }

  // find the upper bound entry
#ifdef WITH_CLANG
  aDynamicT->m_xIndexInfoJ.m_aEntUpper = aExtern_IndexTreePositionLE(
      aDynamicT->m_xIndexInfoJ.m_aIndexTree, aDynamicT->m_xIndexInfoJ.m_aEntKey);
#endif
  __L_LABEL_INDEX_DEREF__:
  aDynamicT->m_iProcessRow = 1;

  if (aDynamicT->m_xIndexInfoJ.m_aEntInt == aDynamicT->m_xIndexInfoJ.m_aEntUpper) {

    aDynamicT->m_iProcessRow = 0;
    goto __L_LABEL_OUTER_LOOP__;
  }

  // marker-end

  L_Return:
  return;

}

/*----------------------------------------------------------------------------*/
/*-- vIndexCrossJoinContinue ------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexCrossJoinContinue
void vIndexCrossJoinContinue(Dynamic_T* aDynamicT) {

  void *aHeadLineSav;
  void *aHeadLine;
  void *aCrwJ;

  i8 iBufferJ;
  i8 iItemJ;

  static const void *C_ADR_REGION = (void *) 0x204800000000;
  static const i8 C_CRW_HEADBYTES = 32;
  static const i8 C_CRW_FIRSTHEAD = 640;

  __L_LABEL_OUTER_LOOP__:
  __L_LABEL_LOOP__:

  // marker-start

  __L_LABEL_INDEX_DEREF__:

  // did we find the entry ?
  if ( ! aDynamicT->m_xIndexInfoJ.m_aEntInt ) {
    aDynamicT->m_iProcessRow = 0;
    goto __L_LABEL_OUTER_LOOP__;
  }

  aDynamicT->m_iProcessRow = 1;

  // marker-end

  L_Return:
  return;

}

/*----------------------------------------------------------------------------*/
/*-- vIndexJoinContinueLO0 ---------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexJoinContinueLO0
void vIndexJoinContinueLO0(Dynamic_T* aDynamicT) {

  // marker-start

  // did we find the entry ?
  if ( ! aDynamicT->m_xIndexInfoJ.m_aEntInt ) {

    // marker-end
  }
}

/*----------------------------------------------------------------------------*/
/*-- vIndexJoinContinueLO1 ---------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexJoinContinueLO1
void vIndexJoinContinueLO1(Dynamic_T* aDynamicT) {

  i8 iLeftOuterRow;
  // marker-start

    // did we join any rows with current left row ?
    if (aDynamicT->m_iJoinedRows == 0) {

      aDynamicT->m_iProcessRow = 1;
      iLeftOuterRow = 1;
      printf("leftOuter = 1\n");

      // marker-end

  }
}

/*----------------------------------------------------------------------------*/
/*-- vIndexJoinContinueLO2 ---------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexJoinContinueLO2
void vIndexJoinContinueLO2(Dynamic_T* aDynamicT) {

  __L_LABEL_OUTER_LOOP__:

  {
    if (true) {
      // marker-start
      goto __L_LABEL_NEXT__;
    } else {
      aDynamicT->m_iProcessRow = 0;
      goto __L_LABEL_OUTER_LOOP__;
    }
  }
    // marker-end
  __L_LABEL_NEXT__:
  return;
}

/*----------------------------------------------------------------------------*/
/*-- vIndexJoinContinueLO3 ---------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexJoinContinueLO3
void vIndexJoinContinueLO3(Dynamic_T* aDynamicT) {

  __L_LABEL_OUTER_LOOP__:
  __L_LABEL_LOOP__:

  // marker-start

  // find the upper bound entry
#ifdef WITH_CLANG
  aDynamicT->m_xIndexInfoJ.m_aEntUpper = aExtern_IndexTreePositionLE(
      aDynamicT->m_xIndexInfoJ.m_aIndexTree, aDynamicT->m_xIndexInfoJ.m_aEntKey);
#endif
  __L_LABEL_INDEX_DEREF__:
  aDynamicT->m_iProcessRow = 1;

  if (aDynamicT->m_xIndexInfoJ.m_aEntInt == aDynamicT->m_xIndexInfoJ.m_aEntUpper) {

    // marker-end
  }
}

/*----------------------------------------------------------------------------*/
/*-- vIndexJoinDereference ---------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexJoinDereference
void vIndexJoinDereference(Dynamic_T* aDynamicT) {

#include "vFLAT_Parameters_v2.h"

  i8 iIndexBuffer;
  i8 iIndexItem;
  i8 i_BaseOffsetDataX;
  char* aIndexCrw;

  IndexInfo __INDEX_INFO__;
  RowInfo   __ROW_INFO_TO__;
  i8 __COLUMNS_TO_INFLATE_;
  i8 __COLUMNS_INFLATED__;

  static const void* C_ADR_REGION = (void*)0x204800000000;
  static const i8 C_CRW_HEADBYTES = 32;
  static const i8 C_CRW_FIRSTHEAD = 640;

  L_InflateAColumn:

  // marker-start

  // process the row
  aDynamicT->m_iProcessRow = 1;

  aHeadLineSav = aHeadLine;

  // dereference the entry
  iIndexBuffer = __INDEX_INFO__.m_aEntInt->Rrf.m_uBuffer;
  iIndexItem   = __INDEX_INFO__.m_aEntInt->Rrf.m_uItem;
  aIndexCrw    = ( (char*)C_ADR_REGION + iIndexBuffer * C_1MB + C_CRW_FIRSTHEAD+ iIndexItem * C_CRW_HEADBYTES );

  // inflate the derefenced entry
  aHeadLine = (struct HeadLine_T *)aIndexCrw;
  auMetaCode = META_AREA(aHeadLine);
  ayDstByte = (byte*)__ROW_INFO_TO__.m_aRowCellArray + 8*__COLUMNS_INFLATED__;
  acStrArea = (char*)__ROW_INFO_TO__.m_acStrArea;
  racStrArea = (char*)__ROW_INFO_TO__.m_acStrArea;
  eStrInflateRule = SI_STRAREA_ADR;
  i_BaseOffsetDataX = C_1B  + C_8B  + __COLUMNS_TO_INFLATE_;
  ayDataByte = ADDR_ADD(byte*,aHeadLine,i_BaseOffsetDataX);

  // marker-end

  __L_LABEL_NEXT__:

  L_Return:
  return;

}

/*----------------------------------------------------------------------------*/
/*-- vIndexJoinReturn --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexJoinReturn
void vIndexJoinReturn(Dynamic_T* aDynamicT) {

  void* aHeadLine;
  void* aHeadLineSav;
  void* aSubReturnLabel;

  IndexInfo __INDEX_INFO__;
  RowInfo __ROW_INFO_TO__;

  i8 iLeftOuterRow;

  // marker-start
  goto __L_LABEL_INDEX_RETURN__;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // INFLATE-CRW-RET
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  __L_LABEL_INDEX_RETURN__:

  aHeadLine = aHeadLineSav;
  aDynamicT->m_iJoinedRows++;

  goto __L_LABEL_NEXT__;

  __L_LABEL_LOOP__:

  __ROW_INFO_TO__.m_acStrArea = __ROW_INFO_TO__.m_acStrAreaSav;

  // check if last row was a leftOuter
  if ( iLeftOuterRow ) {
    printf("got leftOuter \n");
    iLeftOuterRow = 0;
    goto __L_LABEL_OUTER_LOOP__;
  }

  // move to next entry
#ifdef WITH_CLANG
  __INDEX_INFO__.m_aEntInt = (struct Ent_int*)aExtern_EntBufferNextEntry(
      &__INDEX_INFO__.m_aEntBuffer, __INDEX_INFO__.m_aEntInt);
#endif

  goto __L_LABEL_INDEX_DEREF__;

  // marker-end

  __L_LABEL_NEXT__:
  __L_LABEL_INDEX_DEREF__:
  __L_LABEL_OUTER_LOOP__:

  return;
}

/*----------------------------------------------------------------------------*/
/*-- vIndexJoinReturn01 ------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexJoinReturn01
void vIndexJoinReturn01(Dynamic_T* aDynamicT) {

  void* aHeadLine;
  void* aHeadLineSav;
  void* aSubReturnLabel;

  IndexInfo __INDEX_INFO__;
  RowInfo __ROW_INFO_TO__;

  i8 iLeftOuterRow;

  // marker-start
  goto __L_LABEL_INDEX_RETURN__;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // INFLATE-CRW-RET
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  __L_LABEL_INDEX_RETURN__:

  aHeadLine = aHeadLineSav;
  // marker-end

  return;
}

/*----------------------------------------------------------------------------*/
/*-- vIndexJoinReturn02 ------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexJoinReturn02
void vIndexJoinReturn02(Dynamic_T* aDynamicT) {

  void* aHeadLine;
  void* aHeadLineSav;
  void* aSubReturnLabel;

  IndexInfo __INDEX_INFO__;
  RowInfo __ROW_INFO_TO__;

  i8 iLeftOuterRow;

  // marker-start
  goto __L_LABEL_NEXT__;

  __L_LABEL_LOOP__:

  __ROW_INFO_TO__.m_acStrArea = __ROW_INFO_TO__.m_acStrAreaSav;

  // check if last row was a leftOuter
  if ( iLeftOuterRow ) {
    printf("got leftOuter \n");
    iLeftOuterRow = 0;
    goto __L_LABEL_OUTER_LOOP__;
  }

  // move to next entry
#ifdef WITH_CLANG
  __INDEX_INFO__.m_aEntInt = (struct Ent_int*)aExtern_EntBufferNextEntry(
      &__INDEX_INFO__.m_aEntBuffer, __INDEX_INFO__.m_aEntInt);
#endif

  goto __L_LABEL_INDEX_DEREF__;

  // marker-end

  __L_LABEL_NEXT__:
  __L_LABEL_INDEX_DEREF__:
  __L_LABEL_OUTER_LOOP__:

  return;
}

/*----------------------------------------------------------------------------*/
/*-- vFilterEndIndexJoin -----------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vFilterEndIndexJoin
void vFilterEndIndexJoin(Dynamic_T* aDynamicT, bool bFilterResult) {

  __L_LABEL_OUTER_LOOP__:

  // marker-start
  if (bFilterResult && bFilterResult != C_DATUM_NULL) {
    aDynamicT->m_iProcessRow = 1;
    aDynamicT->m_iJoinedRows++;
  } else {
    aDynamicT->m_iProcessRow = 0;
  }

  // marker-end

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
