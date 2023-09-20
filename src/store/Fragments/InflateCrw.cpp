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

  RowInfo __ROW_INFO_TO__;

  i8* paW;
  i8 i_CurrentRow;
  i8 __COLUMNS_IN__;

  // marker-start
  struct __STRUCT_TO__* p__STRUCT_TO__;

  __L_LABEL__:

  paW = &aDynamicT->m_aW[0];

  i_CurrentRow = -1;

  // process the row
	aDynamicT->m_bProcessRow = true;

  // set the string area
	p__STRUCT_TO__ = (struct __STRUCT_TO__*)__ROW_INFO_TO__.m_aRowCellArray;
  __ROW_INFO_TO__.m_acStrArea  = __ROW_INFO_TO__.m_acStrAreaSav;

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
	struct __STRUCT_TO__ {
			union Int_T __COLUMN_TO__;
	};

	struct __STRUCT_TO__* p__STRUCT_TO__;

  // marker-start
  // - - - - - - - - - - - -
  i_Col = __COLUMN_NO__;
  uMetaCode = *auMetaCode++;

  uHigh = uMetaCode >> 4;
  uLow  = uMetaCode & 0x0F;
#ifdef Z_FUSE
	ayDstByte = (byte*)&p__STRUCT_TO__->__COLUMN_TO__.m_iAsI8;
#endif
  aSubReturnLabel = && __L_RETURN_TO_LABEL__;
  goto L_InflateAColumn;

  __L_RETURN_TO_LABEL__:

  // marker-end
  L_InflateAColumn:
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
#ifndef Z_FUSE
  ayDstByte += 8;
#endif
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
  char* acStrArea;
  void* aSubReturnLabel;
  struct __STRUCT_TO__* p__STRUCT_TO__;

  // marker-start
  aSubReturnLabel = && L_InflateCrwRet;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // INFLATE-CRW-RET
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  L_InflateCrwRet:

  aDynamicT->m_xRowInfoIT.m_acStrArea = acStrArea;
  p__STRUCT_TO__ = (struct __STRUCT_TO__*)aDynamicT->m_xRowInfoIT.m_aRowCellArray;
  szStrArea = aDynamicT->m_xRowInfoIT.m_acStrArea;

  // marker-end

  return;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
