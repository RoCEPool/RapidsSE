/*============================================================================*/
/* Code.cpp -- Code Fragments                                                 */
/*============================================================================*/

#include "BASE.h"
//#include "Combined.hpp"
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
/*-- vInflatePdsRowBegin -----------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vInflatePdsRowBegin
void vInflatePdsRowBegin(Dynamic_T* aDynamicT) {

	i8* piColumn;
  i8* paW;

	void* aSubReturnLabel;
  void* aCurrentRow;

  i8 CURRENT_DUP_OP;
  bool bPdsRowBuffer;

	// marker-start
	struct __STRUCT_TO___CURRENT_OP* p__STRUCT_TO___CURRENT_OP;
	aCurrentRow = nullptr;

	__L_LABEL__:

	// dummy initialization of return label
	// (caters to cases when no columns being inflated)
	aSubReturnLabel = && __L_LABEL__;

	paW = &aDynamicT->m_aW[0];

	//i_CurrentRow = -1;

	__L_LABEL_LOOP__:

	aDynamicT->m_iJoinedRows = 0;

	// process the row
	aDynamicT->m_bProcessRow = true;

	//i_CurrentRow++;
	//if (i_CurrentRow  >= aDynamicT->m_nRows) {
	//	goto L_Return;
	//}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// get the next row (should be able to add rowsize)
#ifdef WITH_CLANG
	//aDynamicT->m_xRowInfoIT[CURRENT_DUP_OP].m_aRowCellArray = aDynamicT->m_fp_aExtern_PdsRowBufferCellArray(aDynamicT->m_aPdsRowBuffer, i_CurrentRow);
	if ( ! aCurrentRow)
		aDynamicT->m_xRowInfoIT[CURRENT_DUP_OP].m_aRowCellArray = aDynamicT->m_fp_aExtern_PdsFirstRowCellArray(aDynamicT->m_xRowInfoIT[CURRENT_DUP_OP].m_aPdsRowBuffer, &aCurrentRow);
	else
		aDynamicT->m_xRowInfoIT[CURRENT_DUP_OP].m_aRowCellArray = aDynamicT->m_fp_aExtern_PdsNextRowCellArray(aDynamicT->m_xRowInfoIT[CURRENT_DUP_OP].m_aPdsRowBuffer, &aCurrentRow);
#endif

	if ( ! aCurrentRow)
		goto L_Return;

	aDynamicT->m_xRowInfoIT[CURRENT_DUP_OP].m_acStrArea = aDynamicT->m_xRowInfoIT[CURRENT_DUP_OP].m_acStrAreaSav;

	//piColumn = (i8*)aDynamicT->m_xRowInfoIT[CURRENT_DUP_OP].m_aRowCellArray;
  bPdsRowBuffer = true;

	// marker-end
	L_Return:
	return;
}

/*----------------------------------------------------------------------------*/
/*-- vInflateRowCol ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vInflateRowCol
void vInflateRowCol(Dynamic_T* aDynamicT) {

  m8* amCellAddress;
  i8* piColumn;
  i8 iOffset;
  i8 i_Col;
  i8 __COLUMN_NO__;

  // marker-start
  // - - - - - - - - - - - -
  i_Col = __COLUMN_NO__;

  iOffset = piColumn[i_Col];
  if (iOffset != C_DATUM_NULL) {
    amCellAddress = (m8*)&piColumn[i_Col];
    piColumn[i_Col] = ADDR_ADD(i8, amCellAddress, iOffset);
  }

  // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vInflatePdsRowCol -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vInflatePdsRowCol
void vInflatePdsRowCol(Dynamic_T* aDynamicT) {

	m8* amCellAddress;
	i8* piColumn;
	i8 iOffset;
	i8 i_Col;
	i8 __COLUMN_NO__;

	// marker-start
	// - - - - - - - - - - - -
	i_Col = __COLUMN_NO__;

	iOffset = piColumn[i_Col];
	if (iOffset != C_DATUM_NULL) {
		amCellAddress = (m8*)&piColumn[i_Col];
		piColumn[i_Col] = ADDR_ADD(i8, amCellAddress, iOffset);
	}

	// marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vInflateRowReturn -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vInflateRowReturn
void vInflateRowReturn(Dynamic_T* aDynamicT) {

  char* szStrArea;
  i8 nAreaRemaining;
  i8 CURRENT_DUP_OP;
  struct __STRUCT_TO___CURRENT_OP* p__STRUCT_TO___CURRENT_OP;

  // marker-start

  p__STRUCT_TO___CURRENT_OP = (struct __STRUCT_TO___CURRENT_OP*)aDynamicT->m_xRowInfoIT[CURRENT_DUP_OP].m_aRowCellArray;
  szStrArea = aDynamicT->m_xRowInfoIT[CURRENT_DUP_OP].m_acStrArea;
  nAreaRemaining = aDynamicT->m_xRowInfoIT[CURRENT_DUP_OP].m_nStringAreaSize;

  // marker-end

  return;
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
