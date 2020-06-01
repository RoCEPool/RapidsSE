/*============================================================================*/
/* CsvRecord.cpp -- Csv Record                                                */
/*============================================================================*/

#include "Load.h"

/*----------------------------------------------------------------------------*/
/*-- Public Static -----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* extract row from record (returning first column which has invalid field)   */
/*----------------------------------------------------------------------------*/
Column* CsvRecord::pExtractRow(RowSingle* pRow, StreamDef* pStreamDef,
                               char** aazCrs, Format* pFmt)
{
  Datum oDatum;
  
  i8 nCols = pStreamDef->nColumns(C_ALL);
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    
    Column* pCol = pStreamDef->pGetColumn(i_Col);
    
    if (! CsvField::bExtractDtm(&oDatum,pCol,aazCrs,pFmt))
      return pCol;
    
    pRow->vSetValue(&oDatum,pCol);
  }
  
  return nullptr;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
