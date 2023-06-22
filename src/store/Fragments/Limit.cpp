/*============================================================================*/
/* Limit.cpp -- Code Fragments for limit operations                           */
/*============================================================================*/

#include "BASE.h"
#include "Combined.hpp"
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

  i8 __LIMIT_ROWS__;

  __L_LABEL_LOOP__:
  __L_LABEL_NEXT__:

  // marker-start

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // LIMIT-OP
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  __L_LABEL__:

  printf("LIMIT-OP\n");

  if (!aDynamicT->m_iProcessRow) {
    aDynamicT->m_iProcessRow = 1;
    goto __L_LABEL_LOOP__;
  }

  if (aDynamicT->m_iLimitRows <= __LIMIT_ROWS__) {
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
