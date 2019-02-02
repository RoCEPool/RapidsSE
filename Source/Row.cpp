/*============================================================================*/
/* Row.cpp -- Row                                                             */
/*============================================================================*/

#include "Data.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
RowData* Data::_aRowData(void* aAddr)
{
  RowData* pRow = reinterpret_cast<RowData*>(aAddr);
  
  return pRow;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* RowData::operator new(size_t)
{
  vFatal("fatal: RowData class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowData::RowData(void)
{
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RowData::~RowData(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* convert row to binary string                                               */
/*----------------------------------------------------------------------------*/
void RowData::vAsBinaryStringVLQ(s48K& sDataString, ColumnVec* pColVec)
{
  Stz* aData;
  s17KB sDst;
  
  // get no of columns
  i8 nColumns = pColVec->nItems();
  
  // for each column
  for (i8 i_Column = 0; i_Column < nColumns - 1; i_Column++) {
    
    // get the column at index
    Column* pColumn = pColVec->pGet(i_Column);
    
    // not the first column
    if (i_Column != 0)
      sDataString.vConcat(","_Stz);
    
    // get the column data pointer
    aData = (Stz*)aColumnDatum(i_Column);
    
    // copy the data for the column into aDst
    DtmLib::vSprintQuotesVLQ(sDst, aData, pColumn);
    
    // now copy the data for the column from aDst to aDataString
    if (i_Column == 0)
      sDataString.vCopy(sDst);
    else
      sDataString.vConcat(sDst);
  }
  
}

/*----------------------------------------------------------------------------*/
/* convert row to escaped string                                              */
/*----------------------------------------------------------------------------*/
void RowData::vAsEscapedString(s48K& sDataString, ColumnVec* pColVec)
{
  Stz* aData;
  s17KB sDst;
  
  // get no of columns
  i8 nColumns = pColVec->nItems();
  
  // for each column
  for (i8 i_Column = 0; i_Column < nColumns - 1; i_Column++) {
    
    // get the column at index
    Column* pColumn = pColVec->pGet(i_Column);
    
    // not the first column
    if (i_Column != 0)
      sDataString.vConcat(","_Stz);
    
    // move to column data
    aData = (Stz*)aColumnDatum(i_Column);
    
    // copy the data for the column into aDst
    DtmLib::vSprintEscapedVLQ(pColumn->eDatumType(), sDst, aData, pColumn);
    
    // now copy the data for the column from aDst to aDataString
    if (i_Column == 0)
      sDataString.vCopy(sDst);
    else
      sDataString.vConcat(sDst);
  }
  
}

/*----------------------------------------------------------------------------*/
/* get address of a column datum                                              */
/*----------------------------------------------------------------------------*/
char* RowData::aColumnDatum(i8 iColumnNo)
{
  // get the column offset
  u2 iOffset = iDatumOffset(iColumnNo);
  
  // move to offset
  char* aCursor = ((char*)(this) + iOffset);
  
  return aCursor;
}

/*----------------------------------------------------------------------------*/
/* dump row                                                                   */
/*----------------------------------------------------------------------------*/
void RowData::vDump(ColumnVec* pColVec)
{
  vTrace("RowData: "_Stz);
  
  Stz* aData;
  s17KB sDst;
  
  // get no of columns
  i8 nColumns = pColVec->nItems();

  // for each column except hidden column, dump the offsets
  for (i8 i_Column = 0; i_Column < nColumns - 1; i_Column++) {
    u2 iOffset = iDatumOffset(i_Column);
    vTrace("%lld|"_Stz, iOffset);
  }

  // for each column except hidden column, dump the column data
  for (i8 i_Column = 0; i_Column < nColumns - 1; i_Column++) {

    // get the column
    Column* pColumn = pColVec->pGet(i_Column);

    // move to column data
    aData = (Stz*)aColumnDatum(i_Column);

    // dump the column value
    DtmLib::vSprintVLQ(pColumn->eDatumType(), sDst, aData, pColumn);
    vTrace("%s|"_Stz, sDst.aChar());
  }

  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* show row                                                                   */
/*----------------------------------------------------------------------------*/
void RowData::vShow(ColumnVec* pColVec, bool bCompressed)
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
/* get address of the byte vector                                             */
/*----------------------------------------------------------------------------*/
byte* RowData::yByteVec(void)
{
  byte* yByteVec = aAdd<byte*>(this,auOffsetVec()[0]);
  
  return yByteVec;
}

/*----------------------------------------------------------------------------*/
/* get offset of a column datum                                               */
/*----------------------------------------------------------------------------*/
u2 RowData::iDatumOffset(i8 iColumnNo)
{
  u2 iDatumOffset = auOffsetVec()[iColumnNo];
  
  return iDatumOffset;
}

/*----------------------------------------------------------------------------*/
/* get offset of a column value                                               */
/*----------------------------------------------------------------------------*/
u2* RowData::auOffsetVec(void)
{
  return (u2*)this;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
