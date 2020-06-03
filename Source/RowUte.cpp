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
  
  if (! pRowSlab->bNewRow())
    return false;

  
  // copy values from row single into row slab
  
  i8 nCol = pStreamDef->nColumns(C_ALL);
  for (i8 i_Col = 0; i_Col < nCol; i_Col++) {
    
    Column* pCol = pStreamDef->pGetColumn(i_Col);
    
    // -- single word values --
    
    if (DM_VCH != pCol->eDatum()) {
      
      // copy word from row single to row slab
      pRowSlab->vDeposit(pRowSingle->m_waColVal[i_Col]);
      
      continue;
    }

    // -- vch string values --
    
    // check for null
    if (pRowSingle->m_waColVal[i_Col].bIsNull()) {
      pRowSlab->vDeposit(C_DATUM_NULL);
      continue;
    }
        
    // copy vch from row single to row slab
    char* azTgt = pRowSlab->azDepositString();
    
    char* azSrc = pRowSingle->m_waColVal[i_Col].m_azStr;
    i8 nLen = StrLib::nCopy(azTgt,azSrc);

    pRowSlab->vDepositedString(nLen + 1);
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
