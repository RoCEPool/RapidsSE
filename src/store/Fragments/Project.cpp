/*============================================================================*/
/* Project.cpp -- Code Fragments for project operations                       */
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
/*-- vProjectBegin -----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vProjectBegin
void vProjectBegin(Dynamic_T* aDynamicT) {

  struct __STRUCT_FROM___PREV_OP* p__STRUCT_FROM___PREV_OP;

  RowInfo __ROW_INFO_FROM__[C_MAX_DUP_OPS];
  RowInfo __ROW_INFO_TO__[C_MAX_DUP_OPS];

  char* szStrArea;
  i8 nAreaRemaining;
  i8 CURRENT_DUP_OP;
  i8 PREV_DUP_OP;

  __L_LABEL_LOOP__:

  // marker-start
  struct __STRUCT_TO___CURRENT_OP* p__STRUCT_TO___CURRENT_OP;

__L_LABEL__:

  // Process row ?
  if ( ! aDynamicT->m_bProcessRow ) {
		aDynamicT->m_bProcessRow = true;
    goto __L_LABEL_LOOP__;
  }

  // PROJECT FROM
  p__STRUCT_FROM___PREV_OP = (struct __STRUCT_FROM___PREV_OP*)__ROW_INFO_FROM__[PREV_DUP_OP].m_aRowCellArray;

  // PROJECT TO
  p__STRUCT_TO___CURRENT_OP = (struct __STRUCT_TO___CURRENT_OP*)__ROW_INFO_TO__[CURRENT_DUP_OP].m_aRowCellArray;
  __ROW_INFO_TO__[CURRENT_DUP_OP].m_acStrArea = __ROW_INFO_TO__[CURRENT_DUP_OP].m_acStrAreaSav;
  szStrArea = __ROW_INFO_TO__[CURRENT_DUP_OP].m_acStrArea;
  nAreaRemaining = __ROW_INFO_TO__[CURRENT_DUP_OP].m_nStringAreaSize;

  // marker-end

  return;

}

/*----------------------------------------------------------------------------*/
/*-- iProjectEnd -------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iProjectEnd
i8 iProjectEnd(Dynamic_T* aDynamicT) {

  i8 CURRENT_DUP_OP;
  struct __STRUCT_TO___CURRENT_OP*   p__STRUCT_TO___CURRENT_OP;

  char* szStrArea;
  i8 nAreaRemaining;

  // marker-start

  if (aDynamicT->m_iErrorCode != (i8)EC_NONE) {
    return 0;
  }

  p__STRUCT_TO___CURRENT_OP = (struct __STRUCT_TO___CURRENT_OP*)aDynamicT->m_xRowInfoPT[CURRENT_DUP_OP].m_aRowCellArray;
  szStrArea = aDynamicT->m_xRowInfoPT[CURRENT_DUP_OP].m_acStrArea;
  nAreaRemaining = aDynamicT->m_xRowInfoPT[CURRENT_DUP_OP].m_nStringAreaSize;

  // marker-end

  return 0;

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
