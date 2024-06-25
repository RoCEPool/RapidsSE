/*============================================================================*/
/* RrfJoin.cpp -- Code Fragments                                              */
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
#include "eKE.h"

/*----------------------------------------------------------------------------*/
/*-- Code Fragments ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/*-- vRrfJoinIntKey --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vRrfJoinIntKey
void vRrfJoinIntKey(Dynamic_T* aDynamicT) {

  bool bLeftOuterRow;
  i8 __KEY_KIND__;
  i8 CURRENT_DUP_OP;

  struct __STRUCT_FROM___PREV_OP* p__STRUCT_FROM___PREV_OP;
  RowInfo __ROW_INFO_FROM__;

  __L_LABEL_OUTER_LOOP__:

  // marker-start
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // init RrfJoin (Integer key)
  if (aDynamicT->m_xIndexInfoJ[CURRENT_DUP_OP].m_bInitialize) {

#ifdef WITH_CLANG
		aDynamicT->m_fp_vExtern_IndexFind(&aDynamicT->m_xIndexInfoJ[CURRENT_DUP_OP],"__TABLENAME__","__INDEXNAME__");
#endif
    bLeftOuterRow = false;
    aDynamicT->m_xIndexInfoJ[CURRENT_DUP_OP].m_bInitialize = false;

  }

// marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vRrfJoinGenKey --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vRrfJoinGenKey
void vRrfJoinGenKey(Dynamic_T* aDynamicT) {

  bool bLeftOuterRow;
  i8 __KEY_KIND__;
  i8 CURRENT_DUP_OP;

  __L_LABEL_OUTER_LOOP__:

  // marker-start
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // init RrfJoin (general key)
  if (aDynamicT->m_xIndexInfoJ[CURRENT_DUP_OP].m_bInitialize) {

#ifdef WITH_CLANG
		aDynamicT->m_fp_vExtern_IndexFind(&aDynamicT->m_xIndexInfoJ[CURRENT_DUP_OP],"__TABLENAME__","__INDEXNAME__");
#endif
    bLeftOuterRow = false;
    aDynamicT->m_xIndexInfoJ[CURRENT_DUP_OP].m_bInitialize = false;

  }
  else {

#ifdef WITH_CLANG
		aDynamicT->m_fp_vExtern_EntWrkReset(aDynamicT->m_xIndexInfoJ[CURRENT_DUP_OP].m_aEntWrk);
#endif

  }

// marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vRrfJoinContinueLO1 ---------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vRrfJoinContinueLO1
void vRrfJoinContinueLO1(Dynamic_T* aDynamicT) {

  bool bLeftOuterRow;
  // marker-start
	__L_LABEL_JOIN_NULLS__:

  // did we join any rows with current left row ?
  if (aDynamicT->m_iJoinedRows == 0) {

		aDynamicT->m_bProcessRow = true;
    bLeftOuterRow = true;
    //printf("leftOuter = 1\n");

  // marker-end

  }
}

/*----------------------------------------------------------------------------*/
/*-- vRrfJoinContinueLO2 ---------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vRrfJoinContinueLO2
void vRrfJoinContinueLO2(Dynamic_T* aDynamicT) {

  __L_LABEL_OUTER_LOOP__:

  if (true) {

  // marker-start
    goto __L_LABEL_NEXT__;

  } else {

		aDynamicT->m_bProcessRow = false;
      goto __L_LABEL_OUTER_LOOP__;

  }

  // marker-end
  __L_LABEL_NEXT__:
  return;
}

/*----------------------------------------------------------------------------*/
/*-- vRrfJoinCrossPosition ------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vRrfJoinCrossPosition
void vRrfJoinCrossPosition(Dynamic_T* aDynamicT) {

  IndexInfo __INDEX_INFO__;
  i8 CURRENT_DUP_OP;

  __L_LABEL_OUTER_LOOP__:

  // marker-start

#ifdef WITH_CLANG
  // position at beginning of the index
  __INDEX_INFO__[CURRENT_DUP_OP].m_aEntInt =(struct Ent_int*)aDynamicT->m_fp_aExtern_IndexTreeBegin(
        __INDEX_INFO__[CURRENT_DUP_OP].m_aIndexTree, &__INDEX_INFO__[CURRENT_DUP_OP].m_aEntBuffer);
#endif

  __L_LABEL_INDEX_DEREF__:

  // did we find the entry ?
  if ( ! aDynamicT->m_xIndexInfoJ[CURRENT_DUP_OP].m_aEntInt ) {
		aDynamicT->m_bProcessRow = false;
    goto __L_LABEL_OUTER_LOOP__;
  }

	aDynamicT->m_bProcessRow = true;

  // marker-end

  L_Return:
  return;

}

/*----------------------------------------------------------------------------*/
/*-- vRrfJoinDereference ---------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vRrfJoinDereference
void vRrfJoinDereference(Dynamic_T* aDynamicT) {

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

  // process the row
	aDynamicT->m_bProcessRow = true;

  aHeadLineSav = aHeadLine;

  // dereference the entry
  iIndexBuffer = __INDEX_INFO__[CURRENT_DUP_OP].m_aEntInt->Rrf.Std.m_uBuffer;
  iIndexItem   = __INDEX_INFO__[CURRENT_DUP_OP].m_aEntInt->Rrf.Std.m_uItem;
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
/*-- vRrfJoinPosition_gen --------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vRrfJoinPosition_gen
void vRrfJoinPosition_gen(Dynamic_T* aDynamicT) {

  IndexInfo __INDEX_INFO__;
  i8 CURRENT_DUP_OP;

  __L_LABEL_OUTER_LOOP__:

  // marker-start

#ifdef WITH_CLANG
  // position at lower bound
	__INDEX_POSITION_LOWER__(
      &__INDEX_INFO__[CURRENT_DUP_OP]);
#endif

  // did we find the entry ?
  if ( ! aDynamicT->m_xIndexInfoJ[CURRENT_DUP_OP].m_aEntInt ) {
		aDynamicT->m_bProcessRow = false;
    goto __L_LABEL_OUTER_LOOP__;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // find the upper bound entry

#ifdef WITH_CLANG
  aDynamicT->m_fp_vExtern_IndexTreePositionLE_gen(&aDynamicT->m_xIndexInfoJ[CURRENT_DUP_OP]);
#endif
  __L_LABEL_INDEX_DEREF__:
	aDynamicT->m_bProcessRow = true;

  if (aDynamicT->m_xIndexInfoJ[CURRENT_DUP_OP].m_aEntInt == aDynamicT->m_xIndexInfoJ[CURRENT_DUP_OP].m_aEntUpper) {

		aDynamicT->m_bProcessRow = false;
    goto __L_LABEL_OUTER_LOOP__;
  }

  // marker-end

  L_Return:
  return;

}

/*----------------------------------------------------------------------------*/
/*-- vRrfJoinPosition_int -----------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vRrfJoinPosition_int
void vRrfJoinPosition_int(Dynamic_T* aDynamicT) {

	IndexInfo __INDEX_INFO__;
  i8 CURRENT_DUP_OP;

	__L_LABEL_OUTER_LOOP__:

	// marker-start

#ifdef WITH_CLANG
	// position at lower bound
	__INDEX_POSITION_LOWER__(
      &__INDEX_INFO__[CURRENT_DUP_OP]);
#endif

	// did we find the entry ?
	if ( ! aDynamicT->m_xIndexInfoJ[CURRENT_DUP_OP].m_aEntInt ) {
		aDynamicT->m_bProcessRow = false;
		goto __L_LABEL_OUTER_LOOP__;
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// find the upper bound entry

#ifdef WITH_CLANG
  aDynamicT->m_fp_vExtern_IndexTreePositionLE_int(&aDynamicT->m_xIndexInfoJ[CURRENT_DUP_OP]);
#endif
	__L_LABEL_INDEX_DEREF__:
	aDynamicT->m_bProcessRow = true;

	if (aDynamicT->m_xIndexInfoJ[CURRENT_DUP_OP].m_aEntInt == aDynamicT->m_xIndexInfoJ[CURRENT_DUP_OP].m_aEntUpper) {

		aDynamicT->m_bProcessRow = false;
		goto __L_LABEL_OUTER_LOOP__;
	}

	// marker-end

	L_Return:
	return;

}

/*----------------------------------------------------------------------------*/
/*-- vRrfJoinReturn ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vRrfJoinReturn
void vRrfJoinReturn(Dynamic_T* aDynamicT) {

  void* aHeadLine;
  void* aHeadLineSav;
  void* aSubReturnLabel;

  i8 CURRENT_DUP_OP;

  IndexInfo __INDEX_INFO__[C_MAX_DUP_OPS];
  RowInfo __ROW_INFO_TO__[C_MAX_DUP_OPS];

  bool bLeftOuterRow;

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

  __ROW_INFO_TO__[CURRENT_DUP_OP].m_acStrArea = __ROW_INFO_TO__[CURRENT_DUP_OP].m_acStrAreaSav;

  goto __L_LABEL_INDEX_DEREF__;

  // marker-end

  __L_LABEL_NEXT__:
  __L_LABEL_INDEX_DEREF__:
  __L_LABEL_OUTER_LOOP__:

  return;
}

/*----------------------------------------------------------------------------*/
/*-- iRrfJoinConstraint ------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iRrfJoinConstraint
i8 iRrfJoinConstraint(Dynamic_T* aDynamicT) {

  void* aHeadLine;
  void* aHeadLineSav;
  bool bFilterResult;
	char* szStrArea;

  i8 CURRENT_DUP_OP;

  IndexInfo __INDEX_INFO__;
  RowInfo __ROW_INFO_TO__[C_MAX_DUP_OPS];

  i8 __CONSTRAINTSTACK__;

  bool bLeftOuterRow;

  // marker-start
  goto __L_LABEL_INDEX_RETURN__;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // INFLATE-CRW-RET
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  __L_LABEL_INDEX_RETURN__:

  aHeadLine = aHeadLineSav;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // join constraint
	szStrArea = __ROW_INFO_TO__[CURRENT_DUP_OP].m_acStrArea;

  __CONSTRAINTSTACK__;

  // check for error
  if (aDynamicT->m_iErrorCode != (i8)EC_NONE) {
    return 0;
  }

  // check for constraint result
  if (bFilterResult && bFilterResult != C_DATUM_NULL) {
		aDynamicT->m_bProcessRow = true;
    aDynamicT->m_iJoinedRows++;
  } else {
		aDynamicT->m_bProcessRow = false;
  }

  goto __L_LABEL_NEXT__;

  __L_LABEL_LOOP__:

  __ROW_INFO_TO__[CURRENT_DUP_OP].m_acStrArea = __ROW_INFO_TO__[CURRENT_DUP_OP].m_acStrAreaSav;

  // check if last row was a leftOuter
  if ( bLeftOuterRow ) {
    //printf("got leftOuter \n");
    bLeftOuterRow = false;
    goto __L_LABEL_OUTER_LOOP__;
  }

  // move to next entry
#ifdef WITH_CLANG
  __INDEX_INFO__[CURRENT_DUP_OP].m_aEntInt = (struct Ent_int*)aDynamicT->m_fp_aExtern_EntBufferNextEntry(
      &__INDEX_INFO__[CURRENT_DUP_OP].m_aEntBuffer, __INDEX_INFO__[CURRENT_DUP_OP].m_aEntInt);
#endif

  goto __L_LABEL_INDEX_DEREF__;

  // marker-end

  __L_LABEL_NEXT__:
  __L_LABEL_INDEX_DEREF__:
  __L_LABEL_OUTER_LOOP__:

  return 0;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
