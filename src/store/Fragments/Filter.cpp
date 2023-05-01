/*============================================================================*/
/* Filter.cpp -- Code Fragments for Filter operations                         */
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
/*-- vFilterBegin ------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vFilterBegin
void vFilterBegin(Combined_T* aCombinedT) {

  char* szStrArea;
  struct __STRUCT_FROM__* p__STRUCT_FROM__;

  __L_LABEL_LOOP__:

  // marker-start

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // FILTER-OP
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

__L_LABEL__:

  if (!aCombinedT->m_iProcessRow) {
    aCombinedT->m_iProcessRow = 1;
    goto __L_LABEL_LOOP__;
  }
  // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- iFilterEnd --------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-iFilterEnd
i8 iFilterEnd(Combined_T* aCombinedT, bool bFilterResult) {

  // marker-start
  if (bFilterResult && bFilterResult != C_DATUM_NULL) {
    aCombinedT->m_iProcessRow = 1;
  } else {
    aCombinedT->m_iProcessRow = 0;
  }
  // marker-end
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
