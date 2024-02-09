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

  struct __STRUCT_FROM__* p__STRUCT_FROM__;

  RowInfo __ROW_INFO_FROM__;
  RowInfo __ROW_INFO_TO__;

  char* szStrArea;
  i8 nAreaRemaining;

  __L_LABEL_LOOP__:

  // marker-start
  struct __STRUCT_TO__* p__STRUCT_TO__;

__L_LABEL__:

  // Process row ?
  if ( ! aDynamicT->m_bProcessRow ) {
		aDynamicT->m_bProcessRow = true;
    goto __L_LABEL_LOOP__;
  }

  // PROJECT FROM
  p__STRUCT_FROM__ = (struct __STRUCT_FROM__*)__ROW_INFO_FROM__.m_aRowCellArray;

  // PROJECT TO
  p__STRUCT_TO__ = (struct __STRUCT_TO__*)__ROW_INFO_TO__.m_aRowCellArray;
  __ROW_INFO_TO__.m_acStrArea = __ROW_INFO_TO__.m_acStrAreaSav;
  szStrArea = __ROW_INFO_TO__.m_acStrArea;
  nAreaRemaining = __ROW_INFO_TO__.m_nStringAreaSize;

  // marker-end

  return;

}

/*----------------------------------------------------------------------------*/
/*-- iProjectEnd -------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iProjectEnd
i8 iProjectEnd(Dynamic_T* aDynamicT) {

  struct __STRUCT_TO__*   p__STRUCT_TO__;

  char* szStrArea;
  i8 nAreaRemaining;

  // marker-start

  if (aDynamicT->m_iErrorCode != (i8)EC_NONE) {
    return 0;
  }

  p__STRUCT_TO__ = (struct __STRUCT_TO__*)aDynamicT->m_xRowInfoPT.m_aRowCellArray;
  szStrArea = aDynamicT->m_xRowInfoPT.m_acStrArea;
  nAreaRemaining = aDynamicT->m_xRowInfoPT.m_nStringAreaSize;

  // marker-end

  return 0;

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
