/*============================================================================*/
/* Filter.cpp -- Code Fragments for Filter operations                         */
/*============================================================================*/

#include "BASE.h"
#include "Dynamic.hpp"
#include "IndexDef.hpp"
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
/*-- vFilterBegin ------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vFilterBegin
void vFilterBegin(Dynamic_T* aDynamicT) {

  i8 PREV_DUP_OP;
  i8 PREV_OP_NO;

  char* szStrArea;
  i8 nAreaRemaining;
  struct __STRUCT_FROM___PREV_OP* p__STRUCT_FROM___PREV_OP;
	RowInfo __ROW_INFO_FROM__[C_MAX_DUP_OPS];

  __L_LABEL_LOOP__:

  // marker-start

__L_LABEL__:

	p__STRUCT_FROM___PREV_OP = (struct __STRUCT_FROM___PREV_OP*)__ROW_INFO_FROM__[PREV_DUP_OP].m_aRowCellArray;
	szStrArea = __ROW_INFO_FROM__[PREV_DUP_OP].m_acStrArea;
    nAreaRemaining = __ROW_INFO_FROM__[PREV_DUP_OP].m_nStringAreaSize;

  if ( ! aDynamicT->m_bProcessRow ) {
    aDynamicT->m_bProcessRow = true;
    goto __L_LABEL_LOOP__;
  }
  // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- vFilterEnd --------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vFilterEnd
void vFilterEnd(Dynamic_T* aDynamicT, bool bFilterResult) {

  // marker-start
  if (bFilterResult && bFilterResult != C_DATUM_NULL) {
    aDynamicT->m_bProcessRow = true;
  } else {
    aDynamicT->m_bProcessRow = false;
  }
  // marker-end

}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
