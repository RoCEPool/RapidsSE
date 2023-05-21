/*============================================================================*/
/* Code.cpp -- Code Fragments                                                 */
/*============================================================================*/

#include "BASE.h"
#include "Combined.hpp"
#include "IndexDef.hpp"
#include "CodeSynthCommon.h"
#include "ClangStuff.h"
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
void vInflateRowBegin(Dynamic_T* aDynamicT) {

  i8* paW;
  i8 i_CurrentRow;
  i8* piColumn;

  // marker-start


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // INFLATE-ROW
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  struct __STRUCT_TO__* p__STRUCT_TO__;

  L_InflateRowT:

  printf("INFLATE-ROW\n");

  paW = &aDynamicT->m_aW[0];

  i_CurrentRow = -1;

  __L_LABEL_LOOP__:

   aDynamicT->m_iJoinedRows = 0;

   // process the row
   aDynamicT->m_iProcessRow = 1;

   i_CurrentRow++;
   if (i_CurrentRow  >= aDynamicT->m_nRows) {
     goto L_Return;
   }

   // get the next row (should be able to add rowsize)
#ifdef WITH_TINYC
   aDynamicT->m_xRowInfoIT.m_aRowCellArray = aRowBuffer_GetCellArray(aDynamicT->m_aRowBuffer, i_CurrentRow);
#endif

   aDynamicT->m_xRowInfoIT.m_acStrArea = aDynamicT->m_xRowInfoIT.m_acStrAreaSav;

   piColumn = (i8*)aDynamicT->m_xRowInfoIT.m_aRowCellArray;

  // marker-end
  L_Return:
    return;
}

/*----------------------------------------------------------------------------*/
/*-- vInflateRowCol ----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
// function-vInflateRowCol
void vInflateRowCol(Dynamic_T* aDynamicT) {

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
