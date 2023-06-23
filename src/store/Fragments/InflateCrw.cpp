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
  i8 iaPow10[15];

  // marker-start
  struct __STRUCT_TO__* p__STRUCT_TO__;

  __L_LABEL__:

#ifdef WITH_DEBUG
  printf("INFLATE-CRW\n");
#endif

  iaPow10[0] = 1;
  iaPow10[1] = 10;
  iaPow10[2] = 100;
  iaPow10[3] = 1000;
  iaPow10[4] = 10000;
  iaPow10[5] = 100000;
  iaPow10[6] = 1000000;
  iaPow10[7] = 10000000;
  iaPow10[8] = 100000000;
  iaPow10[9] = 1000000000;
  iaPow10[10] = 10000000000;
  iaPow10[11] = 100000000000;
  iaPow10[12] = 1000000000000;
  iaPow10[13] = 10000000000000;
  iaPow10[14] = 100000000000000;

  paW = &aDynamicT->m_aW[0];

  i_CurrentRow = -1;

  // process the row
  aDynamicT->m_iProcessRow = 1;

  // set the string area
  __ROW_INFO_TO__.m_acStrArea  = __ROW_INFO_TO__.m_acStrAreaSav;

  // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vInflateCrwCol ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vInflateCrwCol
void vInflateCrwCol(Dynamic_T* aDynamicT) {

  void* aSubReturnLabel;
  u8 uMetaCode;
  u8* auMetaCode;
  u8 uHigh;
  u8 uLow;
  i8 i_Col;
  i8 __COLUMN_NO__;

  // marker-start
  // - - - - - - - - - - - -
  i_Col = __COLUMN_NO__;
  uMetaCode = *auMetaCode++;

  uHigh = uMetaCode >> 4;
  uLow  = uMetaCode & 0x0F;

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
  ayDstByte += 8;
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
  //p__STRUCT_TO__ = (struct __STRUCT_TO__*)aDynamicT->m_xRowInfoIT.m_aRowCellArray;
  //szStrArea = aDynamicT->m_xRowInfoIT.m_acStrArea;

  // marker-end

  return;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
