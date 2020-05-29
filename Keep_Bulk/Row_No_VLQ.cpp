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
void* Row::operator new(size_t)
{
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
Row::~Row(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/




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
    
    char* pOffset = (char*)(this) + i_Column*C_COLUMN_OFFSET_SIZE;
    u2 iOffset = *((u2*)pOffset);
    
    // move to offset
    aData = (Stz*)((unsigned char*)(this) + iOffset);
    
    s17KB s;
    DtmLib::vSprintQuotes(pColumn->eDatumType(), s, aData, pColumn);
    if (i_Column == 0)
      sDataString.vCopy(s);
    else
      sDataString.vConcat(s);
  }
  
}

/*----------------------------------------------------------------------------*/
/* convert row to binary string                                               */
/*----------------------------------------------------------------------------*/
void Row::vAsBinaryStringVLQ(s48K& sDataString, ColumnVec* pColVec)
{
  i8 i_RowLength = 0;
  char* pOffset;
  Stz* aData;
  
  i8 nColumns = pColVec->nItems();
  
  for (i8 i_Column = 0; i_Column < nColumns - 1; i_Column++) {
    
    Column* pColumn = pColVec->pGet(i_Column);
    
    if (i_Column != 0)
      sDataString.vConcat(","_Stz);
    
    char* pOffset = (char*)(this) + i_Column*C_COLUMN_OFFSET_SIZE;
    u2 iOffset = *((u2*)pOffset);
    
    // move to offset
    aData = (Stz*)((unsigned char*)(this) + iOffset);
    
    s17KB s;
    DtmLib::vSprintQuotesVLQ(pColumn->eDatumType(), s, aData, pColumn);
    if (i_Column == 0)
      sDataString.vCopy(s);
    else
      sDataString.vConcat(s);
  }
  
  
}

/*----------------------------------------------------------------------------*/
/* convert row to escaped string                                              */
/*----------------------------------------------------------------------------*/
#ifdef _VLQ
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
    
    char* pOffset = (char*)(this) + i_Column*C_COLUMN_OFFSET_SIZE;
    u2 iOffset = *((u2*)pOffset);
    
    // move to offset
    aData = (Stz*)((unsigned char*)(this) + iOffset);
    
    s17KB s;
    DtmLib::vSprintEscapedVLQ(pColumn->eDatumType(), s, aData, pColumn);
    if (i_Column == 0)
      sDataString.vCopy(s);
    else
      sDataString.vConcat(s);
  }
  
}
#else
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
    
    char* pOffset = (char*)(this) + i_Column*C_COLUMN_OFFSET_SIZE;
    u2 iOffset = *((u2*)pOffset);
    
    // move to offset
    aData = (Stz*)((unsigned char*)(this) + iOffset);
    
    s17KB s;
    DtmLib::vSprintEscaped(pColumn->eDatumType(), s, aData, pColumn);
    if (i_Column == 0)
      sDataString.vCopy(s);
    else
      sDataString.vConcat(s);
  }
  
}
#endif
/*----------------------------------------------------------------------------*/
/* get address of a column datum                                              */
/*----------------------------------------------------------------------------*/
char* Row::aColumnDatum(i8 iColumnNo)
{
  i8 iCurrentColumn = 0;
  
  char* aOffset = (char*)(this) + iColumnNo*C_COLUMN_OFFSET_SIZE;
  u2 iOffset = *((u2*)aOffset);
  
  // move to offset
  char* aCursor = ((char*)(this) + iOffset);
  
  return aCursor;
}

/*----------------------------------------------------------------------------*/
/* dump row                                                                   */
/*----------------------------------------------------------------------------*/
#ifdef _VLQ
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
    u2 iOffset = *((u2*)pOffset);

    vTrace("%lld|"_Stz, iOffset);
  }

  // for each column except hidden column, dump the column data
  for (i8 i_Column = 0; i_Column < nColumns - 1; i_Column++) {

    // get the column
    Column* pColumn = pColVec->pGet(i_Column);

    // calulate the offset pointer for column
    char* pOffset = (char*)(this) + i_Column*C_COLUMN_OFFSET_SIZE;

    // get the column offset
    u2 iOffset = *((u2*)pOffset);

    // let aData point to column data
    aData = (Stz*)((char*)(this) + iOffset);

    // dump the column value
    s17KB s;
    DtmLib::vSprintVLQ(pColumn->eDatumType(), s, aData, pColumn);
    vTrace("%s|"_Stz, s.aChar());
  }

  vTrace("\n"_Stz);
}
#else
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
    u2 iOffset = *((u2*)pOffset);
    
    vTrace("%lld|"_Stz, iOffset);
  }
  
  // for each column except hidden column, dump the column data
  for (i8 i_Column = 0; i_Column < nColumns - 1; i_Column++) {
    
    // get the column
    Column* pColumn = pColVec->pGet(i_Column);
    
    // calulate the offset pointer for column
    char* pOffset = (char*)(this) + i_Column*C_COLUMN_OFFSET_SIZE;
    
    // get the column offset
    u2 iOffset = *((u2*)pOffset);
    
    // let aData point to column data
    aData = (Stz*)((unsigned char*)(this) + iOffset);
    
    // dump the column value
    s17KB s;
    DtmLib::vSprint(pColumn->eDatumType(), s, aData, pColumn);
    vTrace("%s|"_Stz, s.aChar());
  }
  
  vTrace("\n"_Stz);
}
#endif

/*----------------------------------------------------------------------------*/
/* show row                                                                   */
/*----------------------------------------------------------------------------*/
void Row::vShow(ColumnVec* pColVec, bool bCompressed)
{
  i8 i_RowLength = 0;
  char* pOffset;
  Stz* aData;
  
  i8 nColumns = pColVec->nItems() - 1;
  
  for (i8 i_Column = 0; i_Column < nColumns; i_Column++) {
    
    Column* pColumn = pColVec->pGet(i_Column);
    
    char* pOffset = (char*)(this) + i_Column*C_COLUMN_OFFSET_SIZE;
    u2 iOffset = *((u2*)pOffset);
    
    // move to offset
    aData = (Stz*)((unsigned char*)(this) + iOffset);
    
    s17KB s;
    if (bCompressed)
      DtmLib::vSprintVLQ(pColumn->eDatumType(), s, aData, pColumn);
    else
      DtmLib::vSprint(pColumn->eDatumType(), s, aData, pColumn);
    vTrace("%s "_Stz, s.aChar());
  }
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
