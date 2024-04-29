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
  i8 CURRENT_DUP_OP;

  char* __INDEXNAME__;
  IndexInfo __INDEX_INFO__[C_MAX_DUP_OPS];
	RowInfo __ROW_INFO_TO__[C_MAX_DUP_OPS];

	char* szStrArea;

  __L_LABEL_LOOP__:

  // marker-start
  struct __STRUCT_TO___CURRENT_OP* p__STRUCT_TO___CURRENT_OP;

  __L_LABEL__:

  p__STRUCT_TO___CURRENT_OP = (struct __STRUCT_TO___CURRENT_OP*)__ROW_INFO_TO__[CURRENT_DUP_OP].m_aRowCellArray;
	__ROW_INFO_TO__[CURRENT_DUP_OP].m_acStrArea = __ROW_INFO_TO__[CURRENT_DUP_OP].m_acStrAreaSav;
	szStrArea = __ROW_INFO_TO__[CURRENT_DUP_OP].m_acStrArea;

  paW = &aDynamicT->m_aW[0];

  #ifdef WITH_CLANG
		aDynamicT->m_fp_vExtern_IndexFind(&aDynamicT->m_xIndexInfoT[CURRENT_DUP_OP],"__TABLENAME__","__INDEXNAME__");
  #endif

    i_CurrentRow = -1;

  // marker-end

  L_Return:
  return;

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
