/*============================================================================*/
/* Index.cpp -- Code Fragments                                                */
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
/*-- vIndexDereferenceLB -----------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexDereferenceLB
void vIndexDereferenceLB(Dynamic_T* aDynamicT) {

#include "vFLAT_Parameters.h"

  i8 iIndexBuffer;
  i8 iIndexItem;
  i8 i_BaseOffsetDataX;
  i8 CURRENT_DUP_OP;
  char* aIndexCrw;

  IndexInfo __INDEX_INFO__[C_MAX_DUP_OPS];
  RowInfo   __ROW_INFO_TO__[C_MAX_DUP_OPS];
  i8 __COLUMNS_TO_INFLATE_;
  i8 __COLUMNS_INFLATED__;

  static const void* C_ADR_REGION = (void*)0x204800000000;
  static const i8 C_CRW_HEADBYTES = 32;
  static const i8 C_CRW_FIRSTHEAD = 640;

  L_InflateAColumn:

  // marker-start

  __L_LABEL_INDEX_DEREF__:

  // reached end ?
  if ( ! __INDEX_INFO__[CURRENT_DUP_OP].m_aEntInt) {
    goto L_Return;
  }

  // process the row
  aDynamicT->m_bProcessRow = true;

  // dereference the entry
  iIndexBuffer = __INDEX_INFO__[CURRENT_DUP_OP].m_aEntInt->Rrf.m_uBuffer;
  iIndexItem   = __INDEX_INFO__[CURRENT_DUP_OP].m_aEntInt->Rrf.m_uItem;
  aIndexCrw    = ( (char*)C_ADR_REGION + iIndexBuffer * C_1MB + C_CRW_FIRSTHEAD + iIndexItem * C_CRW_HEADBYTES );

  // inflate the derefenced entry
  aHeadLine = (struct HeadLine_T *)aIndexCrw;
  auMetaCode = META_AREA(aHeadLine);
  ayDstByte = (byte*)__ROW_INFO_TO__[CURRENT_DUP_OP].m_aRowCellArray + 8*__COLUMNS_INFLATED__;
  acStrArea = (char*)__ROW_INFO_TO__[CURRENT_DUP_OP].m_acStrArea;
	racStrArea = acStrArea;
  eStrInflateRule = SI_STRAREA_ADR;
  i_BaseOffsetDataX = C_1B  + C_8B  + __COLUMNS_TO_INFLATE_;
  ayDataByte = ADDR_ADD(byte*,aHeadLine,i_BaseOffsetDataX);

  // marker-end

  __L_LABEL_NEXT__:

  L_Return:
  return;

}

/*----------------------------------------------------------------------------*/
/*-- vIndexDereferenceUB -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexDereferenceUB
void vIndexDereferenceUB(Dynamic_T* aDynamicT) {

#include "vFLAT_Parameters.h"

  i8 iIndexBuffer;
  i8 iIndexItem;
  i8 i_BaseOffsetDataX;
  i8 CURRENT_DUP_OP;
  char* aIndexCrw;

  IndexInfo __INDEX_INFO__[C_MAX_DUP_OPS];
  RowInfo   __ROW_INFO_TO__[C_MAX_DUP_OPS];
  i8 __COLUMNS_TO_INFLATE_;
  i8 __COLUMNS_INFLATED__;

  static const void* C_ADR_REGION = (void*)0x204800000000;
  static const i8 C_CRW_HEADBYTES = 32;
  static const i8 C_CRW_FIRSTHEAD = 640;

  L_InflateAColumn:

  // marker-start

  __L_LABEL_INDEX_DEREF__:

  // reached upper bound ?
  if (__INDEX_INFO__[CURRENT_DUP_OP].m_aEntInt == __INDEX_INFO__[CURRENT_DUP_OP].m_aEntUpper) {
    goto L_Return;
  }

  // process the row
  aDynamicT->m_bProcessRow = true;

  // dereference the entry
  iIndexBuffer = __INDEX_INFO__[CURRENT_DUP_OP].m_aEntInt->Rrf.m_uBuffer;
  iIndexItem   = __INDEX_INFO__[CURRENT_DUP_OP].m_aEntInt->Rrf.m_uItem;
  aIndexCrw    = ( (char*)C_ADR_REGION + iIndexBuffer * C_1MB + C_CRW_FIRSTHEAD + iIndexItem * C_CRW_HEADBYTES );

  // inflate the derefenced entry
  aHeadLine = (struct HeadLine_T *)aIndexCrw;
  auMetaCode = META_AREA(aHeadLine);
  ayDstByte = (byte*)__ROW_INFO_TO__[CURRENT_DUP_OP].m_aRowCellArray + 8*__COLUMNS_INFLATED__;
  acStrArea = (char*)__ROW_INFO_TO__[CURRENT_DUP_OP].m_acStrArea;
  racStrArea = acStrArea;
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

  i8 CURRENT_DUP_OP;
  IndexInfo __INDEX_INFO__[C_MAX_DUP_OPS];

  // marker-start

#ifdef WITH_CLANG
  // position at beginning of the index
  __INDEX_INFO__[CURRENT_DUP_OP].m_aEntInt =(struct Ent_int*)aDynamicT->m_fp_aExtern_IndexTreeBegin(
        __INDEX_INFO__[CURRENT_DUP_OP].m_aIndexTree, &__INDEX_INFO__[CURRENT_DUP_OP].m_aEntBuffer);
#endif

  if ( ! aDynamicT->m_xIndexInfoT[CURRENT_DUP_OP].m_aEntInt )
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

#ifdef WITH_CLANG
  // position at lower bound
	__INDEX_POSITION_LOWER__(&__INDEX_INFO__[CURRENT_DUP_OP]);
#endif

  // marker-end

  L_Return:
  return;

}

/*----------------------------------------------------------------------------*/
/*-- vIndexScanContinue-------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexScanContinue
void vIndexScanContinue(Dynamic_T* aDynamicT) {

  i8 CURRENT_DUP_OP;
  IndexInfo __INDEX_INFO__[C_MAX_DUP_OPS];

  // marker-start

  if ( ! __INDEX_INFO__[CURRENT_DUP_OP].m_aEntInt )
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

#ifdef WITH_CLANG
  // position at upper bound
  __INDEX_POSITION_UPPER__(&__INDEX_INFO__[CURRENT_DUP_OP]);
#endif

  // marker-end

  L_Return:
  return;

}

/*----------------------------------------------------------------------------*/
/*-- vIndexCheckUpper --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexCheckUpper
void vIndexCheckUpper(Dynamic_T* aDynamicT) {

  i8 CURRENT_DUP_OP;
  IndexInfo __INDEX_INFO__[C_MAX_DUP_OPS];

  // marker-start

  // reached upper bound ?
  if (__INDEX_INFO__[CURRENT_DUP_OP].m_aEntInt == __INDEX_INFO__[CURRENT_DUP_OP].m_aEntUpper) {
    goto L_Return;
  }

  // process the row
	aDynamicT->m_bProcessRow = true;

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

  aHeadLine = aHeadLineSav;

  goto __L_LABEL_NEXT__;

  __L_LABEL_LOOP__:

  // move to next entry
  //printf("moving to next index scan\n");

#ifdef WITH_CLANG
  __INDEX_INFO__[CURRENT_DUP_OP].m_aEntInt = (struct Ent_int*)aDynamicT->m_fp_aExtern_EntBufferNextEntry(
      &__INDEX_INFO__[CURRENT_DUP_OP].m_aEntBuffer, __INDEX_INFO__[CURRENT_DUP_OP].m_aEntInt);
#endif

  goto __L_LABEL_INDEX_DEREF__;

  // marker-end

  __L_LABEL_NEXT__:
  __L_LABEL_INDEX_DEREF__:
  __L_LABEL_OUTER_LOOP__:

  return;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
