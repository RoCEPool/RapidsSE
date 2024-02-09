/*============================================================================*/
/* Aggreg.cpp -- Code Fragments for aggregate operations                      */
/*============================================================================*/

#include "BASE.h"
#include "ClangDynamic.h"

/*----------------------------------------------------------------------------*/
/*-- Code Fragments ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- iOrdAggBegin ------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iOrdAggBegin
i8 iOrdAggBegin(Dynamic_T* aDynamicT) {

  struct __STRUCT_FROM__* p__STRUCT_FROM__;
  RowInfo __ROW_INFO_FROM__;
  RowInfo __ROW_INFO_TO__;

  char* szStrArea;
  i8 nAreaRemaining;
	bool bKeyMatch;

  i8 __EXPRSTACK__;
	i8 __GBYSTACK__;
	i8 __INITSTACK__;
	i8 __ORDAGGSTACK__;

	__L_LABEL_NEXT__:
  __L_LABEL_OUTER_LOOP__:

  // marker-start
  struct __STRUCT_TO__* p__STRUCT_TO__;

  __L_LABEL__:

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // FROM
  p__STRUCT_FROM__ = (struct __STRUCT_FROM__*)__ROW_INFO_FROM__.m_aRowCellArray;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // TO
  p__STRUCT_TO__ = (struct __STRUCT_TO__*)__ROW_INFO_TO__.m_aRowCellArray;
  //__ROW_INFO_TO__.m_acStrArea = __ROW_INFO_TO__.m_acStrAreaSav;
  szStrArea = __ROW_INFO_TO__.m_acStrArea;
  nAreaRemaining = __ROW_INFO_TO__.m_nStringAreaSize;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Process row ?
  if ( ! aDynamicT->m_bProcessRow ) {
		aDynamicT->m_bProcessRow = true;
		goto __L_LABEL_OUTER_LOOP__;
  }

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// do we have an aggregate row
	if (  ! aDynamicT->m_bHasAggRow ) {
		aDynamicT->m_bHasAggRow = true;
		aDynamicT->m_bHasRowToAgg = false;

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		// populate the key
		__GBYSTACK__;
		if (aDynamicT->m_iErrorCode != (i8)EC_NONE) {
			return 0;
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		// initialize the aggregates
		__INITSTACK__;
		if (aDynamicT->m_iErrorCode != (i8)EC_NONE) {
			return 0;
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		// aggregate
		__EXPRSTACK__;
		if (aDynamicT->m_iErrorCode != (i8)EC_NONE) {
			return 0;
		}

		goto __L_LABEL_OUTER_LOOP__;
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// check the key
	__ORDAGGSTACK__;
	if (aDynamicT->m_iErrorCode != (i8)EC_NONE) {
		return 0;
	}

	if (bKeyMatch) {
		__EXPRSTACK__;
		if (aDynamicT->m_iErrorCode != (i8)EC_NONE) {
			return 0;
		}

		goto __L_LABEL_OUTER_LOOP__;
	}

	aDynamicT->m_bHasRowToAgg = true;
	aDynamicT->m_bHasAggRow = false;
	goto __L_LABEL_NEXT__;

  // marker-end

	return 0;
}

/*----------------------------------------------------------------------------*/
/*-- iOrdAggConstraint -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iOrdAggConstraint
i8 iOrdAggConstraint(Dynamic_T* aDynamicT) {

	i8 __CONSTRAINTSTACK__;

	bool bFilterResult;

	__LABEL_ORDAGG__:

	// marker-start

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// ORDAGG CONSTRAINT
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	__CONSTRAINTSTACK__;

	// check for error
	if (aDynamicT->m_iErrorCode != (i8)EC_NONE) {
		return 0;
	}

	// check for constraint result
	if ( ! bFilterResult || bFilterResult == C_DATUM_NULL) {
		aDynamicT->m_bHasAggRow = false;

		if ( aDynamicT->m_bHasRowToAgg ) {
			goto __LABEL_ORDAGG__;
		}
		goto L_Return;
	}

	// marker-end
	L_Return:
	  return 0;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
