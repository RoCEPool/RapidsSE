/*============================================================================*/
/* RowData.cpp -- Row Data Buffer                                             */
/*============================================================================*/

#include "DBobjects.h"

extern BufferPool* gpBufferPool;

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
/* fetch next record from buffer, read next chunk if necessary                */
/*----------------------------------------------------------------------------*/
bool RowData::bFormatData(ColumnVec* pColVec, Stz* aRawData, i8 iRawLength, Stz* aRowData, i8& iRowLength)
{
  char* aSrc = (char*)aRawData->aChar();
  i8 nColumns = pColVec->nItems() - 1;
  
  Stz* pOffset = aRowData;
  Stz* aDst = pOffset + C_COLUMN_OFFSET_SIZE * nColumns;
  iRowLength = C_COLUMN_OFFSET_SIZE * nColumns;
  
  
  i8 iColumnLength = 0;
  
  for (i8 i_Column = 0; i_Column < nColumns; i_Column++) {
    Column* pColumn = pColVec->pGet(i_Column);
    
    // extract datum type
    bool bValid = DtmLib::bExtract(pColumn->eDatumType(), aDst, &aSrc, pColumn);
    
    // is the datum valid ?
    if (!bValid) {
      vTrace("Skipping row - invalid field %lld in record: "_Stz, i_Column);
      for (char* a = (char*)aRawData; *a && '\n' != *a; a++)
        vTrace("%c"_Stz,*a);
      vTrace("\n"_Stz);
      return false;
    }

    // save offset
    *(pOffset->aChar()) = iRowLength;
    
    iColumnLength = DtmLib::iSize(pColumn->eDatumType(), aDst->aChar());
    
    // move to next offset location
    pOffset += C_COLUMN_OFFSET_SIZE;
    
    // increment row length
    iRowLength += iColumnLength;
    
    // move to next column location
    aDst += iColumnLength;
  }
  
  //vDump(aRowData, pColVec);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get pointer to where column starts                                         */
/*----------------------------------------------------------------------------*/
char* RowData::pColumn(char* aRowData, i8 iColumnNo)
{

  i8 iCurrentColumn = 0;
  
  char* pOffset = (char*)(aRowData) + iColumnNo*C_COLUMN_OFFSET_SIZE;
  unsigned char iOffset = *(pOffset);
  
  // move to offset
  char* sCursor = aRowData + iOffset;

  return sCursor;
}

/*----------------------------------------------------------------------------*/
/* show RowData                                                               */
/*----------------------------------------------------------------------------*/
void RowData::vRowAsString(s256& sDataString, Stz* aRowData, ColumnVec* pColVec)
{
  i8 i_RowLength = 0;
  char* pOffset;
  Stz* aData;
  
  i8 nColumns = pColVec->nItems();
  
  for (i8 i_Column = 0; i_Column < nColumns - 1; i_Column++) {
    
    Column* pColumn = pColVec->pGet(i_Column);
    
    if (i_Column != 0)
      sDataString.vConcat(","_Stz);
      
    char* pOffset = (char*)(aRowData) + i_Column*C_COLUMN_OFFSET_SIZE;
    unsigned char iOffset = *(pOffset);
    
    // move to offset
    aData = aRowData + iOffset;
    
    s256 s;
    DtmLib::vSprint(pColumn->eDatumType(), s, aData, pColumn);
    if (i_Column == 0)
      sDataString.vCopy(s);
    else
      sDataString.vConcat(s);
  }

}

/*----------------------------------------------------------------------------*/
/* show RowData                                                               */
/*----------------------------------------------------------------------------*/
void RowData::vShow(Stz* aRowData, ColumnVec* pColVec)
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
