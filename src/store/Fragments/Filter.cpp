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

  char* szStrArea;
  i8 nAreaRemaining;

  struct __STRUCT_FROM__* p__STRUCT_FROM__;
	RowInfo __ROW_INFO_FROM__;

  __L_LABEL_LOOP__:

  // marker-start
__L_LABEL__:

	p__STRUCT_FROM__ = (struct __STRUCT_FROM__*)__ROW_INFO_FROM__.m_aRowCellArray;
	szStrArea = __ROW_INFO_FROM__.m_acStrArea;
  nAreaRemaining = __ROW_INFO_FROM__.m_nStringAreaSize;

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
