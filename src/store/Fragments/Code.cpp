/*============================================================================*/
/* Code.cpp -- Code Fragments                                                 */
/*============================================================================*/

#include "BASE.h"
#include "Combined.hpp"

/*----------------------------------------------------------------------------*/
/*-- Code Fragments ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

// function-vFilterBegin
void vFilterBegin(Combined_T* aCombinedT, Stz* sLabelLoop) {

  char* szStrArea;

  RASSERT(sLabelLoop->bSame("L_AdvanceNextRow"_Stz));

  L_AdvanceNextRow:

  // marker-start

  szStrArea = aCombinedT->m_racStrAreaT;
  if (!aCombinedT->m_iProcessRow) {
    aCombinedT->m_iProcessRow = 1;
    goto L_AdvanceNextRow;
  }
  // marker-end
}

// function-iFilterEnd
i8 iFilterEnd(Combined_T* aCombinedT, bool bFilterResult) {

  // marker-start

  if (aCombinedT->m_iErrorCode != (i8)EC_NONE) {
   return 0;
  }

  if (bFilterResult && bFilterResult != C_DATUM_NULL) {
    aCombinedT->m_iProcessRow = 1;
  } else {
    aCombinedT->m_iProcessRow = 0;
  }
  // marker-end
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
