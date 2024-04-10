/*============================================================================*/
/* IndexScan.cpp -- Code Fragments                                            */
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
/*-- vIndexScanBegin ---------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vIndexScanBegin
void vIndexScanBegin(Dynamic_T* aDynamicT) {

  i8 i_CurrentRow;
  i8*   paW;

  char* __INDEXNAME__;
  IndexInfo __INDEX_INFO__;
	RowInfo __ROW_INFO_TO__;

	char* szStrArea;
  i8 nAreaRemaining;

  __L_LABEL_LOOP__:

  // marker-start
  struct __STRUCT_TO__* p__STRUCT_TO__;

  __L_LABEL__:

  p__STRUCT_TO__ = (struct __STRUCT_TO__*)__ROW_INFO_TO__.m_aRowCellArray;
	__ROW_INFO_TO__.m_acStrArea = __ROW_INFO_TO__.m_acStrAreaSav;
	szStrArea = __ROW_INFO_TO__.m_acStrArea;
  nAreaRemaining = __ROW_INFO_TO__.m_nStringAreaSize;

  paW = &aDynamicT->m_aW[0];

  #ifdef WITH_CLANG
		aDynamicT->m_fp_vExtern_IndexFind(&aDynamicT->m_xIndexInfoT,"__TABLENAME__","__INDEXNAME__");
  #endif

    i_CurrentRow = -1;

  // marker-end

  L_Return:
  return;

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
