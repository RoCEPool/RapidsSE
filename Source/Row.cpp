/*============================================================================*/
/* Row.cpp -- Row                                                             */
/*============================================================================*/

#include "DBobjects.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Row* DBobjects::_pRow(void* aAddr)
{
  Row* pRow = reinterpret_cast<Row*>(aAddr);
  
  return pRow;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Row::operator new(size_t iSize)
{
  RASSERT(iSize);
  vFatal("fatal: Row class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Row::Row(void)
{
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Row::~Row(void)
{
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* extract row from given raw record                                          */
/*----------------------------------------------------------------------------*/
bool Row::bFormatRow(ColumnVec* pColVec,
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
      if ( ! gpAttachMap->m_bMetadata ) {
        vWarn("Skipping row - invalid field %s in record: "_Stz,
               pColumn->sColumnName().aChar());
        for (char* a = (char*)aRawData; *a && '\n' != *a && '\r' != *a; a++)
          vTrace("%c"_Stz,*a);
        vTrace("\n"_Stz);
      }
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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert row to binary string                                               */
/*----------------------------------------------------------------------------*/
void Row::vAsBinaryString(s48K& sDataString, ColumnVec* pColVec)
{
  i8 i_RowLength = 0;
  char* pOffset;
  Stz* aData;
  
  i8 nColumns = pColVec->nItems();
  
  for (i8 i_Column = 0; i_Column < nColumns - 1; i_Column++) {
    
    Column* pColumn = pColVec->pGet(i_Column);
    
    if (i_Column != 0)
      sDataString.vConcat(","_Stz);
    
    unsigned char* pOffset = (unsigned char*)(this) + i_Column*C_COLUMN_OFFSET_SIZE;
    unsigned char iOffset = *(pOffset);
    
    // move to offset
    aData = (Stz*)((unsigned char*)(this) + iOffset);
    
    s256 s;
    DtmLib::vSprint(pColumn->eDatumType(), s, aData, pColumn);
    if (i_Column == 0)
      sDataString.vCopy(s);
    else
      sDataString.vConcat(s);
  }
  
  
}

/*----------------------------------------------------------------------------*/
/* convert row to escaped string                                              */
/*----------------------------------------------------------------------------*/
void Row::vAsEscapedString(s48K& sDataString, ColumnVec* pColVec)
{  
  i8 i_RowLength = 0;
  char* pOffset;
  Stz* aData;
  
  i8 nColumns = pColVec->nItems();
  
  for (i8 i_Column = 0; i_Column < nColumns - 1; i_Column++) {
    
    Column* pColumn = pColVec->pGet(i_Column);
    
    if (i_Column != 0)
      sDataString.vConcat(","_Stz);
    
    unsigned char* pOffset = (unsigned char*)(this) + i_Column*C_COLUMN_OFFSET_SIZE;
    unsigned char iOffset = *(pOffset);
    
    // move to offset
    aData = (Stz*)((unsigned char*)(this) + iOffset);
    
    s256 s;
    DtmLib::vSprintEscaped(pColumn->eDatumType(), s, aData, pColumn);
    if (i_Column == 0)
      sDataString.vCopy(s);
    else
      sDataString.vConcat(s);
  }
  
}

/*----------------------------------------------------------------------------*/
/* get address of a column datum                                              */
/*----------------------------------------------------------------------------*/
char* Row::aColumnDatum(i8 iColumnNo)
{
  i8 iCurrentColumn = 0;
  
  char* aOffset = (char*)(this) + iColumnNo*C_COLUMN_OFFSET_SIZE;
  unsigned char iOffset = *(aOffset);
  
  // move to offset
  char* aCursor = ((char*)(this) + iOffset);
  
  return aCursor;
}

/*----------------------------------------------------------------------------*/
/* dump row                                                                   */
/*----------------------------------------------------------------------------*/
void Row::vDump(ColumnVec* pColVec)
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
    char* pOffset = (char*)(this) + i_Column*C_COLUMN_OFFSET_SIZE;

    // get the column offset
    unsigned char iOffset = *(pOffset);

    vTrace("%lld|"_Stz, iOffset);
  }

  // for each column except hidden column, dump the column data
  for (i8 i_Column = 0; i_Column < nColumns - 1; i_Column++) {

    // get the column
    Column* pColumn = pColVec->pGet(i_Column);

    // calulate the offset pointer for column
    char* pOffset = (char*)(this) + i_Column*C_COLUMN_OFFSET_SIZE;

    // get the column offset
    unsigned char iOffset = *(pOffset);

    // let aData point to column data
    aData = (Stz*)((unsigned char*)(this) + iOffset);

    // dump the column value
    s256 s;
    DtmLib::vSprint(pColumn->eDatumType(), s, aData, pColumn);
    vTrace("%s|"_Stz, s.aChar());
  }

  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* show row                                                                   */
/*----------------------------------------------------------------------------*/
void Row::vShow(ColumnVec* pColVec)
{
  i8 i_RowLength = 0;
  char* pOffset;
  Stz* aData;
  
  i8 nColumns = pColVec->nItems();
  
  for (i8 i_Column = 0; i_Column < nColumns - 1; i_Column++) {
    
    Column* pColumn = pColVec->pGet(i_Column);
    
    char* pOffset = (char*)(this) + i_Column*C_COLUMN_OFFSET_SIZE;
    unsigned char iOffset = *(pOffset);
    
    // move to offset
    aData = (Stz*)((unsigned char*)(this) + iOffset);
    
    s256 s;
    DtmLib::vSprint(pColumn->eDatumType(), s, aData, pColumn);
    vTrace("%s "_Stz, s.aChar());
  }
  
  //vTrace("\n"_Stz);
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
