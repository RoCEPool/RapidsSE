/*============================================================================*/
/* Limit.cpp -- Code Fragments for limit operations                           */
/*============================================================================*/

#include "BASE.h"
//#include "Combined.hpp"
#include "IndexDef.hpp"
#include "CodeSynthCommon.h"
#include "ClangStuff.h"
#include "ClangDynamic.h"
#include "Pay.hpp"
#include "Key.hpp"
#include "Ent.hpp"
#include "Ent_int.h"

/*----------------------------------------------------------------------------*/
/*-- Code Fragments ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- vLimit ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vLimit
void vLimit(Dynamic_T* aDynamicT) {

  char* szStrArea;
  i8 nAreaRemaining;
  struct __STRUCT_FROM___PREV_OP* p__STRUCT_FROM___PREV_OP;
	RowInfo __ROW_INFO_FROM__[C_MAX_DUP_OPS];

  i8 CURRENT_DUP_OP;
  i8 PREV_DUP_OP;
  i8 PREV_OP_NO;

	i8 __LIMIT_ROWS__;

  __L_LABEL_LOOP__:
  __L_LABEL_NEXT__:

  // marker-start

  struct __STRUCT_TO___CURRENT_OP* p__STRUCT_TO___CURRENT_OP;

  __L_LABEL__:

	p__STRUCT_FROM___PREV_OP = (struct __STRUCT_FROM___PREV_OP*)__ROW_INFO_FROM__[PREV_DUP_OP].m_aRowCellArray;
	szStrArea = __ROW_INFO_FROM__[PREV_DUP_OP].m_acStrArea;
    nAreaRemaining = __ROW_INFO_FROM__[PREV_DUP_OP].m_nStringAreaSize;

  p__STRUCT_TO___CURRENT_OP = (struct __STRUCT_TO___CURRENT_OP*)p__STRUCT_FROM___PREV_OP;

  if (!aDynamicT->m_bProcessRow) {
		aDynamicT->m_bProcessRow = true;
    goto __L_LABEL_LOOP__;
  }

  if (aDynamicT->m_iLimitRows < __LIMIT_ROWS__) {
    aDynamicT->m_iLimitRows++;
    goto __L_LABEL_NEXT__;
  }
  else
    goto L_Return;

  // marker-end

  L_Return:
    return;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
