/*============================================================================*/
/* RowData.cpp -- Row Data Buffer                                             */
/*============================================================================*/

#include "DBobjects.h"

/*----------------------------------------------------------------------------*/
/* dump RowData                                                               */
/*----------------------------------------------------------------------------*/
void RowData::vDump(Stz* aRowData, ColumnVec* pColVec)
{
  vTrace("RowData: "_Stz);
  i8 i_RowLength = 0;
  char* pOffset;
  Stz* aData;
  
  // get no of columns
  i8 nColumns = pColVec->nItems();
  
  // for each column except hidden column, dump teh offsets
  for (i8 i_Column = 0; i_Column < nColumns - 1; i_Column++) {
    
    // calulate the offset pointer for column
    char* pOffset = (char*)(aRowData) + i_Column*C_COLUMN_OFFSET_SIZE;
    
    // get the column offset
    unsigned char iOffset = *(pOffset);
    
    vTrace("%lld|"_Stz, iOffset);
  }

  // for each column except hidden column, dump the column data
  for (i8 i_Column = 0; i_Column < nColumns - 1; i_Column++) {
    
    // get the column
    Column* pColumn = pColVec->pGet(i_Column);
  
    // calulate the offset pointer for column
    char* pOffset = (char*)(aRowData) + i_Column*C_COLUMN_OFFSET_SIZE;
    
    // get the column offset
    unsigned char iOffset = *(pOffset);
    
    // let aData point to column data
    aData = aRowData + iOffset;
  
    // dump the column value
    s256 s;
    DtmLib::vSprint(pColumn->eDatumType(), s, aData, pColumn);
    vTrace("%s|"_Stz, s.aChar());
  }

  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* extract row from given raw record                                          */
/*----------------------------------------------------------------------------*/
bool RowData::bFormatData(ColumnVec* pColVec,
                          Formatters* pFormatters,
                          Stz* aRawData, i8 iRawLength,
                          Stz* aRowData, i8& iRowLength,
                          bool &bBlankLine)
{
  char* aSrc = aRawData->aChar();
  bBlankLine = false;
  i8 nColumns = pColVec->nItems() - 1;
  
  Stz* pOffset = aRowData;
  Stz* aDst = pOffset + C_COLUMN_OFFSET_SIZE * nColumns;
  iRowLength = C_COLUMN_OFFSET_SIZE * nColumns;
  
  char* aStart = aSrc;
  
  // skip leading spaces
  StrLib::vSkipSpaces(&aSrc);
  
  // skip blank lines
  if (! DtmLib::bIsThereLineData(*aSrc,pFormatters) ) {
    bBlankLine = true;
    return false;
  }
  
  // reset back to start
  aSrc = aStart;
  
  //vTrace("RawData: ]%256s[\n"_Stz,aRawData);
  
  //MemLib::vHexDump(aRawData,256);
  for (i8 i_Column = 0; i_Column < nColumns; i_Column++) {
    
    Column* pColumn = pColVec->pGet(i_Column);
    if (13 == pColumn->iColumnNo())
      int iDebug = 1;
    
    // extract datum type
    eDM eDatumType = pColumn->eDatumType();
    bool bValid = DtmLib::bExtract(eDatumType, aDst, &aSrc,
                                   pColumn, pFormatters);
    
    // is the datum valid ?
    if ( ! bValid ) {
      vWarn("Skipping row - invalid field %s in record: "_Stz,
             pColumn->sColumnName().aChar());
      for (char* a = (char*)aRawData; *a && '\n' != *a && '\r' != *a; a++)
        vTrace("%c"_Stz,*a);
      vTrace("\n"_Stz);
      return false;
    }

    // save offset
    *(pOffset->aChar()) = iRowLength;
    
    i8 iColumnLength = DtmLib::iSize(eDatumType, aDst->aChar());
    
    // move to next offset location
    pOffset += C_COLUMN_OFFSET_SIZE;
    
    // increment row length
    iRowLength += iColumnLength;
    
    // move to next column location
    aDst += iColumnLength;
  }
  
  //vDump(aRowData, pColVec); // keep as debug
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get address to where column datum starts                                   */
/*----------------------------------------------------------------------------*/
char* RowData::aColumnDatum(char* aRowData, i8 iColumnNo)
{
  i8 iCurrentColumn = 0;
  
  char* aOffset = (char*)(aRowData) + iColumnNo*C_COLUMN_OFFSET_SIZE;
  unsigned char iOffset = *(aOffset);
  
  // move to offset
  char* aCursor = aRowData + iOffset;

  return aCursor;
}

/*----------------------------------------------------------------------------*/
/* show RowData                                                               */
/*----------------------------------------------------------------------------*/
void RowData::vShow_TBR(Stz* aRowData, ColumnVec* pColVec)
{
  
  i8 i_RowLength = 0;
  char* pOffset;
  Stz* aData;
  
  i8 nColumns = pColVec->nItems();
  
  for (i8 i_Column = 0; i_Column < nColumns - 1; i_Column++) {
    
    Column* pColumn = pColVec->pGet(i_Column);
    
    char* pOffset = (char*)(aRowData) + i_Column*C_COLUMN_OFFSET_SIZE;
    unsigned char iOffset = *(pOffset);
    
    // move to offset
    aData = aRowData + iOffset;
    
    s256 s;
    DtmLib::vSprint(pColumn->eDatumType(), s, aData, pColumn);
    vTrace("%s "_Stz, s.aChar());
  }
  
  //vTrace("\n"_Stz);
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
