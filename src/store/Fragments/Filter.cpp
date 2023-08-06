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
  struct __STRUCT_FROM__* p__STRUCT_FROM__;

  __L_LABEL_LOOP__:

  // marker-start
__L_LABEL__:

  if (!aDynamicT->m_iProcessRow) {
    aDynamicT->m_iProcessRow = 1;
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
    aDynamicT->m_iProcessRow = 1;
  } else {
    aDynamicT->m_iProcessRow = 0;
  }
  // marker-end

}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
