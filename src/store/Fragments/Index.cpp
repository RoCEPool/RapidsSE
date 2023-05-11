/*============================================================================*/
/* IndexScan.cpp -- Code Fragments                                            */
/*============================================================================*/

#include "BASE.h"
#include "Dynamic.hpp"
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
/*-- vIndexDereference -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexDereference
void vIndexDereference(Dynamic_T* aDynamicT) {

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
/*-- vIndexPositionBegin -----------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexPositionBegin
void vIndexPositionBegin(Dynamic_T* aDynamicT) {

  IndexInfo __INDEX_INFO__;

  // marker-start

#ifdef WITH_TINYC
  // position at beginning of the index
  __INDEX_INFO__.m_aEntInt =(struct Ent_int*)aIndexTree_begin(
        __INDEX_INFO__.m_aIndexTree, &__INDEX_INFO__.m_aEntBuffer);
#endif

  if ( ! aDynamicT->m_xIndexInfoT.m_aEntInt )
    goto L_Return;

  // marker-end

  L_Return:
  return;

}


/*----------------------------------------------------------------------------*/
/*-- vIndexPositionLower -----------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexPositionLower
void vIndexPositionLower(Dynamic_T* aDynamicT) {

  IndexInfo __INDEX_INFO__;

  // marker-start

#ifdef WITH_TINYC
  // position at lower bound
  __INDEX_INFO__.m_aEntInt =  (struct Ent_int *)__INDEX_POSITION_LOWER__(
      __INDEX_INFO__.m_aIndexTree, &__INDEX_INFO__.m_aEntBuffer, __INDEX_INFO__.m_aEntKey);
#endif

  // marker-end

  L_Return:
  return;

}

/*----------------------------------------------------------------------------*/
/*-- vIndexScanContinue-----------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexScanContinue
void vIndexScanContinue(Dynamic_T* aDynamicT) {

  IndexInfo __INDEX_INFO__;

  // marker-start

  if ( ! __INDEX_INFO__.m_aEntInt )
    goto L_Return;

  // marker-end

  L_Return:
  return;

}

/*----------------------------------------------------------------------------*/
/*-- vIndexPositionUpper -----------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexPositionUpper
void vIndexPositionUpper(Dynamic_T* aDynamicT) {

  IndexInfo __INDEX_INFO__;

  // marker-start

#ifdef WITH_TINYC
  // position at upper bound
  __INDEX_INFO__.m_aEntUpper =  (struct Ent_int *)__INDEX_POSITION_UPPER__(
      __INDEX_INFO__.m_aIndexTree, __INDEX_INFO__.m_aEntKey);
#endif

  // reached upper bound ?
  if (__INDEX_INFO__.m_aEntInt == __INDEX_INFO__.m_aEntUpper) {
    goto L_Return;
  }

  // process the row
  aDynamicT->m_iProcessRow = 1;

  // marker-end

  L_Return:
  return;

}

/*----------------------------------------------------------------------------*/
/*-- vIndexCheckUpper --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexCheckUpper
void vIndexCheckUpper(Dynamic_T* aDynamicT) {

  IndexInfo __INDEX_INFO__;

  // marker-start

  __L_LABEL_INDEX_DEREF__:

  // reached upper bound ?
  if (__INDEX_INFO__.m_aEntInt == __INDEX_INFO__.m_aEntUpper) {
    goto L_Return;
  }

  // process the row
  aDynamicT->m_iProcessRow = 1;

  // marker-end

  L_Return:
  return;

}

/*----------------------------------------------------------------------------*/
/*-- vIndexReturn ------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexReturn
void vIndexReturn(Dynamic_T* aDynamicT) {

  void* aHeadLine;
  void* aHeadLineSav;
  void* aSubReturnLabel;

  IndexInfo __INDEX_INFO__;
  RowInfo __ROW_INFO_TO__;

  // marker-start
  goto __L_LABEL_INDEX_RETURN__;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // INFLATE-CRW-RET
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  __L_LABEL_INDEX_RETURN__:
  printf("L_indexReturn\n");

  //aHeadLine = aHeadLineSav;
  aDynamicT->m_iJoinedRows++;   // ?? only for join
  goto __L_LABEL_NEXT__;

  __L_LABEL_LOOP__:

  __ROW_INFO_TO__.m_acStrArea = __ROW_INFO_TO__.m_acStrAreaSav;

  // move to next entry
  __INDEX_INFO__.m_aEntInt = (struct Ent_int*)aEntBuffer_nextEntry(
      &__INDEX_INFO__.m_aEntBuffer, __INDEX_INFO__.m_aEntInt);
  goto __L_LABEL_INDEX_DEREF__;

  // marker-end

  __L_LABEL_NEXT__:
  __L_LABEL_INDEX_DEREF__:

  return;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
