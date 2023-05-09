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
void vProjectBegin(Dynamic_T* aDynamicT) {

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

  if (!aDynamicT->m_iProcessRow) {
    aDynamicT->m_iProcessRow = 1;
    goto __L_LABEL_LOOP__;
  }

  p__STRUCT_FROM__ = (struct __STRUCT_FROM__*)aDynamicT->m_xRowInfoIT.m_aRowCellArray;

  aDynamicT->m_xRowInfoPT.m_acStrArea = aDynamicT->m_xRowInfoPT.m_acStrAreaSav;
  p__STRUCT_TO__ = (struct __STRUCT_TO__*)aDynamicT->m_xRowInfoPT.m_aRowCellArray;
  szStrArea = aDynamicT->m_xRowInfoPT.m_acStrArea;
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

  // marker-start

  if (aDynamicT->m_iErrorCode != (i8)EC_NONE) {
    return 0;
  }

  p__STRUCT_TO__ = (struct __STRUCT_TO__*)aDynamicT->m_xRowInfoPT.m_aRowCellArray;
  szStrArea = aDynamicT->m_xRowInfoPT.m_acStrArea;

  // marker-end

  return 0;

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
