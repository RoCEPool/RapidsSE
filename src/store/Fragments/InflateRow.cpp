/*============================================================================*/
/* Code.cpp -- Code Fragments                                                 */
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
/*-- vInflateRowBegin --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vInflateRowBegin
void vInflateRowBegin(Combined_T* aCombinedT) {

  i8* paW;
  i8 i_CurrentRow;
  i8* piColumn;

  // marker-start


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // INFLATE-ROW
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  struct __STRUCT_TO__* p__STRUCT_TO__;

  L_InflateRowT:


  paW = &aCombinedT->m_aW[0];

  i_CurrentRow = -1;

  L_AdvanceNextRow:

   aCombinedT->m_iJoinedRows = 0;

   // process the row
   aCombinedT->m_iProcessRow = 1;

   i_CurrentRow++;
   if (i_CurrentRow  >= aCombinedT->m_nRows) {
     goto L_Return;
   }

   // get the next row (should be able to add rowsize)
#ifdef WITH_TINYC
   aCombinedT->m_xRowInfoT.m_aRowCellArray = aRowBuffer_GetCellArray(aCombinedT->m_aRowBuffer, i_CurrentRow);
#endif

   aCombinedT->m_xRowInfoT.m_racStrArea = aCombinedT->m_xRowInfoT.m_racStrAreaSav;

   piColumn = (i8*)aCombinedT->m_xRowInfoT.m_aRowCellArray;

  // marker-end
  L_Return:
    return;
}

/*----------------------------------------------------------------------------*/
/*-- vInflateRowCol ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vInflateRowCol
void vInflateRowCol(Combined_T* aCombinedT) {

  m8* amCellAddress;
  i8* piColumn;
  i8 iOffset;
  i8 i_Col;
  i8 __COLUMN_NO__;

  // marker-start
  // - - - - - - - - - - - -
  i_Col = __COLUMN_NO__;

  iOffset = piColumn[i_Col];
  if (iOffset != C_DATUM_NULL) {
    amCellAddress = (m8*)&piColumn[i_Col];
    piColumn[i_Col] = ADDR_ADD(i8, amCellAddress, iOffset);
  }

  // marker-end
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
