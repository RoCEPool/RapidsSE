/*============================================================================*/
/* RowUte.cpp -- Row Ute helper                                               */
/*============================================================================*/

#include "Rows.h"

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add a row single to row slab                                               */
/*----------------------------------------------------------------------------*/
bool RowUte::bAddSingleToSlab(RowSlab* pRowSlab, RowSingle* pRowSingle,
                              StreamDef* pStreamDef)
{
  _static_
  
  // begin a new slab row, return false if no space
  
  if (! pRowSlab->aAllotNewRow())
    return false;

  
  // copy values from row single into row slab
  
  i8 nCol = pStreamDef->nColumns(C_ALL);
  for (i8 i_Col = 0; i_Col < nCol; i_Col++) {
    
    Column* pCol = pStreamDef->pGetColumn(i_Col);
    
    // -- single word values --
    
    if (DM_VCH != pCol->eDatum()) {
      
      // copy word from row single to row slab
      pRowSlab->vDeposit(pRowSingle->iColVal(i_Col));
      
      continue;
    }

    // -- vch string values --
    
    // check for null
    if (pRowSingle->bIsColNull(i_Col)) {
      pRowSlab->vDeposit(C_DATUM_NULL);
      continue;
    }
        
    // copy vch from row single to row slab
    char* azTgt = pRowSlab->azDepositString_begin();
    
    char* azSrc = pRowSingle->azColVal(i_Col);
    i8 nLen = StrLib::nCopy(azTgt,azSrc);

    pRowSlab->vDepositString_end(nLen + 1);
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
