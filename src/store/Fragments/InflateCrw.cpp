/*============================================================================*/
/* Code.cpp -- Code Fragments                                                 */
/*============================================================================*/

#include "BASE.h"
#include "Dynamic.hpp"

/*----------------------------------------------------------------------------*/
/*-- Code Fragments ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- vInflateCrwBegin --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vInflateCrwBegin
void vInflateCrwBegin(Dynamic_T* aDynamicT) {

  RowInfo __ROW_INFO_TO__[C_MAX_DUP_OPS];

  i8* paW;
  i8 i_CurrentRow;
  i8 CURRENT_DUP_OP;

  // marker-start
  struct __STRUCT_TO___CURRENT_OP* p__STRUCT_TO___CURRENT_OP;

  __L_LABEL__:

  paW = &aDynamicT->m_aW[0];

  i_CurrentRow = -1;

  // process the row
	aDynamicT->m_bProcessRow = true;

  // set the string area
	p__STRUCT_TO___CURRENT_OP = (struct __STRUCT_TO___CURRENT_OP*)__ROW_INFO_TO__[CURRENT_DUP_OP].m_aRowCellArray;
  __ROW_INFO_TO__[CURRENT_DUP_OP].m_acStrArea  = __ROW_INFO_TO__[CURRENT_DUP_OP].m_acStrAreaSav;

  // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vInflateCrwCol ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vInflateCrwCol
void vInflateCrwCol(Dynamic_T* aDynamicT) {

  void* aSubReturnLabel;
	byte* ayDstByte;
  u8 uMetaCode;
  u8* auMetaCode;
  u8 uHigh;
  u8 uLow;
  i8 i_Col;
  i8 __COLUMN_NO__;
	struct __STRUCT_TO___CURRENT_OP {
			union Int_T __COLUMN_TO__;
	};

	struct __STRUCT_TO___CURRENT_OP* p__STRUCT_TO___CURRENT_OP;

  // marker-start
  // - - - - - - - - - - - -
  i_Col = __COLUMN_NO__;
  uMetaCode = *auMetaCode++;

  uHigh = uMetaCode >> 4;
  uLow  = uMetaCode & 0x0F;
	ayDstByte = (byte*)&p__STRUCT_TO___CURRENT_OP->__COLUMN_TO__.m_iAsI8;
  aSubReturnLabel = && __L_RETURN_TO_LABEL__;
  goto L_InflateAColumn;

  __L_RETURN_TO_LABEL__:

  // marker-end
  L_InflateAColumn:
  return;
}

/*----------------------------------------------------------------------------*/
/*-- vPriorCrwCol ------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vPriorCrwCol
void vPriorCrwCol(Dynamic_T* aDynamicT) {

	struct __STRUCT_TO___CURRENT_OP {
			union Int_T __COLUMN_TO__;
			union Int_T __COLUMN_PRIOR__;
	};

	struct __STRUCT_TO___CURRENT_OP* p__STRUCT_TO___CURRENT_OP;

	// marker-start
	// - - - - - - - - - - - -
	p__STRUCT_TO___CURRENT_OP->__COLUMN_TO__.m_iAsI8 = p__STRUCT_TO___CURRENT_OP->__COLUMN_PRIOR__.m_iAsI8;

	// marker-end
	return;
}

/*----------------------------------------------------------------------------*/
/*-- vSkipCrwCol -------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vSkipCrwCol
void vSkipCrwCol(Dynamic_T* aDynamicT) {

  void* aSubReturnLabel;
  u8 uMetaCode;
  u8* auMetaCode;
  u8 uHigh;
  u8 uLow;
  i8 i_Col;
  i8 __COLUMN_NO__;
  byte* ayDstByte;

  // marker-start
  // - - - - - - - - - - - -
  i_Col = __COLUMN_NO__;
  uMetaCode = *auMetaCode++;

  uHigh = uMetaCode >> 4;
  uLow  = uMetaCode & 0x0F;

  aSubReturnLabel = && __L_RETURN_TO_LABEL__;
  goto L_SkipAColumn;

  __L_RETURN_TO_LABEL__:

  // marker-end
  L_SkipAColumn:
  return;
}

/*----------------------------------------------------------------------------*/
/*-- vInflateCrwReturn -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vInflateCrwReturn
void vInflateCrwReturn(Dynamic_T* aDynamicT) {

  char* szStrArea;
  i8 nAreaRemaining;
  char* acStrArea;
  void* aSubReturnLabel;
  i8 CURRENT_DUP_OP;
  struct __STRUCT_TO___CURRENT_OP* p__STRUCT_TO___CURRENT_OP;

  // marker-start
  aSubReturnLabel = && L_InflateCrwRet;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // INFLATE-CRW-RET
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  L_InflateCrwRet:

  aDynamicT->m_xRowInfoIT[CURRENT_DUP_OP].m_acStrArea = acStrArea;
  p__STRUCT_TO___CURRENT_OP = (struct __STRUCT_TO___CURRENT_OP*)aDynamicT->m_xRowInfoIT[CURRENT_DUP_OP].m_aRowCellArray;
  szStrArea = aDynamicT->m_xRowInfoIT[CURRENT_DUP_OP].m_acStrArea;
  nAreaRemaining = aDynamicT->m_xRowInfoIT[CURRENT_DUP_OP].m_nStringAreaSize;

  // marker-end

  return;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
