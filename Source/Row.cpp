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
void Row::vAsBinaryStringVLQ(s48K& sDataString, ColumnVec* pColVec)
{
  Stz* aData;
  
  i8 nColumns = pColVec->nItems();
  
  for (i8 i_Column = 0; i_Column < nColumns - 1; i_Column++) {
    
    Column* pColumn = pColVec->pGet(i_Column);
    
    if (i_Column != 0)
      sDataString.vConcat(","_Stz);
    
    // move to column data
    aData = (Stz*)aColumnDatum(i_Column);
    
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
void Row::vAsEscapedString(s48K& sDataString, ColumnVec* pColVec)
{
  Stz* aData;
  
  i8 nColumns = pColVec->nItems();
  
  for (i8 i_Column = 0; i_Column < nColumns - 1; i_Column++) {
    
    Column* pColumn = pColVec->pGet(i_Column);
    
    if (i_Column != 0)
      sDataString.vConcat(","_Stz);
    
    // move to column data
    aData = (Stz*)aColumnDatum(i_Column);
    
    s17KB s;
    DtmLib::vSprintEscapedVLQ(pColumn->eDatumType(), s, aData, pColumn);
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
  // get the column offset
  u2 iOffset = iColumnOffset(iColumnNo);
  
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
  
  Stz* aData;

  // get no of columns
  i8 nColumns = pColVec->nItems();

  // for each column except hidden column, dump teh offsets
  for (i8 i_Column = 0; i_Column < nColumns - 1; i_Column++) {

    u2 iOffset = iColumnOffset(i_Column);

    vTrace("%lld|"_Stz, iOffset);
  }

  // for each column except hidden column, dump the column data
  for (i8 i_Column = 0; i_Column < nColumns - 1; i_Column++) {

    // get the column
    Column* pColumn = pColVec->pGet(i_Column);

    // move to column data
    aData = (Stz*)aColumnDatum(i_Column);

    // dump the column value
    s17KB s;
    DtmLib::vSprintVLQ(pColumn->eDatumType(), s, aData, pColumn);
    vTrace("%s|"_Stz, s.aChar());
  }

  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* show row                                                                   */
/*----------------------------------------------------------------------------*/
void Row::vShow(ColumnVec* pColVec, bool bCompressed)
{
  Stz* aData;
  
  i8 nColumns = pColVec->nItems() - 1;
  
  for (i8 i_Column = 0; i_Column < nColumns; i_Column++) {
    
    Column* pColumn = pColVec->pGet(i_Column);
    
    // move to column data
    aData = (Stz*)aColumnDatum(i_Column);
    
    s17KB s;
    if (bCompressed)
      DtmLib::vSprintVLQ(pColumn->eDatumType(), s, aData, pColumn);
    else
      DtmLib::vSprint(pColumn->eDatumType(), s, aData, pColumn);
    vTrace("%s "_Stz, s.aChar());
  }
  
}
/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* get address of a column datum                                              */
/*----------------------------------------------------------------------------*/
u2 Row::iColumnOffset(i8 iColumnNo)
{
  // calculate the offset pointer for the column
  char* aOffset = (char*)(this) + iColumnNo*C_COLUMN_OFFSET_SIZE;
  
  // get the ogffset
  u2 iOffset = *((u2*)aOffset);
  
  return iOffset;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
