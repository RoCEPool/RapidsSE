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
  struct __STRUCT_FROM__* p__STRUCT_FROM__;
	RowInfo __ROW_INFO_FROM__;

	i8 __LIMIT_ROWS__;

  __L_LABEL_LOOP__:
  __L_LABEL_NEXT__:

  // marker-start
  __L_LABEL__:

	p__STRUCT_FROM__ = (struct __STRUCT_FROM__*)__ROW_INFO_FROM__.m_aRowCellArray;
	szStrArea = __ROW_INFO_FROM__.m_acStrArea;
  nAreaRemaining = __ROW_INFO_FROM__.m_nStringAreaSize;

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
