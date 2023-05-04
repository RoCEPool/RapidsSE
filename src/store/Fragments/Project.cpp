/*============================================================================*/
/* Project.cpp -- Code Fragments for project operations                       */
/*============================================================================*/

#include "BASE.h"
#include "Combined.hpp"
#include "IndexDef.hpp"
#include "TccStuff.h"
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
void vProjectBegin(Combined_T* aCombinedT) {

  struct __STRUCT_FROM__ {
    i8 N_NATIONKEY;
  };

  struct __STRUCT_FROM__* p__STRUCT_FROM__;

  char* szStrArea;

  __L_LABEL_LOOP__:

  // marker-start



  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // PROJECT-OP
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  struct __STRUCT_FROM__* __STRUCT_FROM__;
  struct __STRUCT_TO__* p__STRUCT_TO__;

  __L_LABEL__:

  if (!aCombinedT->m_iProcessRow) {
    aCombinedT->m_iProcessRow = 1;
    goto __L_LABEL_LOOP__;
  }

  p__STRUCT_FROM__ = (struct __STRUCT_FROM__*)aCombinedT->m_xRowInfoT.m_aRowCellArray;

  aCombinedT->m_racStrAreaP = aCombinedT->m_racStrAreaPSav;
  p__STRUCT_TO__ = (struct __STRUCT_TO__*)aCombinedT->m_aRowCellArrayP;
  szStrArea = aCombinedT->m_racStrAreaP;
  // marker-end

  return;

}

/*----------------------------------------------------------------------------*/
/*-- iProjectEnd -------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iProjectEnd
i8 iProjectEnd(Combined_T* aCombinedT) {

  struct __STRUCT_TO__*   p__STRUCT_TO__;

  char* szStrArea;

  // marker-start

  if (aCombinedT->m_iErrorCode != (i8)EC_NONE) {
    return 0;
  }

  p__STRUCT_TO__ = (struct __STRUCT_TO__*)aCombinedT->m_aRowCellArrayP;
  szStrArea = aCombinedT->m_racStrAreaP;

  // marker-end

  return 0;

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
