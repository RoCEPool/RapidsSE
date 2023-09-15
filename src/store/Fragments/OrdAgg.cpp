/*============================================================================*/
/* Aggreg.cpp -- Code Fragments for aggregate operations                      */
/*============================================================================*/

#include "BASE.h"
#include "ClangDynamic.h"

/*----------------------------------------------------------------------------*/
/*-- Code Fragments ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- vOrdAggBegin ------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vOrdAggBegin
void vOrdAggBegin(Dynamic_T* aDynamicT) {

  struct __STRUCT_FROM__* p__STRUCT_FROM__;
  RowInfo __ROW_INFO_FROM__;
  RowInfo __ROW_INFO_TO__;

  char* szStrArea;

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

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Process row ?
  if ( ! aDynamicT->m_iProcessRow ) {

		// did we get an aggregate row ?
		if (aDynamicT->m_bHasAggRow) {
			aDynamicT->m_bHasAggRow = false;
			aDynamicT->m_iProcessRow = 1;
		}
		goto __L_LABEL_NEXT__;
  }

  // marker-end
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
