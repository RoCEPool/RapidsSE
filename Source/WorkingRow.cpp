/*============================================================================*/
/* WorkingRow.cpp -- result row                                                */
/*============================================================================*/

#include "DBobjects.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
WorkingRow* DBobjects::_pWorkingRow(void* aAddr)
{
  WorkingRow* pWorkingRow = reinterpret_cast<WorkingRow*>(aAddr);
  
  return pWorkingRow;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* WorkingRow::operator new(size_t)
{
  vFatal("fatal: WorkingRow class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* WorkingRow::operator new(size_t, WorkingRow* pWorkingRow)
{
  return pWorkingRow;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
WorkingRow::WorkingRow(Row* aSrcRow, ColumnVec *pColumnVec, i8 nWorkingRowSize, i8 nRowSize, bool bCompressed)
{
  _poolAddress_(this);
  
  m_nWorkingRowSize = nWorkingRowSize;
  Row* aDstRow = this->aRow();
  
  //----------------------------------------------------------------------------
  // To copy a compressed row as is
  if (bCompressed) {
    MemLib::vCopy(aDstRow,aSrcRow,nRowSize,CN_WORKINGBUFFER);
    return;
  }
  
  //----------------------------------------------------------------------------
  // To copy a compressed row as uncompressed row
  
  i8 nColumns = pColumnVec->nItems() - 1;
  Stz* pOffset = (Stz*)aDstRow;
  Stz* aDst = pOffset + C_COLUMN_OFFSET_SIZE * nColumns;
  
  i8 iRowLength = C_COLUMN_OFFSET_SIZE * nColumns;
  
  for (i8 i_Column = 0; i_Column < nColumns; i_Column++) {
    
    Column* pColumn = pColumnVec->pGet(i_Column);
    eDM eDatumType = pColumn->eDatumType();
    
    char* pSrcOffset = (char*)(aSrcRow) + i_Column*C_COLUMN_OFFSET_SIZE;
    u2 iSrcOffset = *((u2*)pSrcOffset);
    
    // move to offset
    Stz* aSrcData = (Stz*)((unsigned char*)(aSrcRow) + iSrcOffset);
    
    // copy column data (nCopyVLQ will convert VLQ to non-VLQ
    i8 iColumnLength = DtmLib::nCopyVLQ(eDatumType, aDst, aSrcData);
    
    // save the offset
    *((u2*)pOffset->aChar()) = iRowLength;
    
    // move to next offset location
    pOffset += C_COLUMN_OFFSET_SIZE;
    
    // increment row length
    iRowLength += iColumnLength;
    
    // move to next column location
    aDst += iColumnLength;
    
  }
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
WorkingRow::~WorkingRow(void) noexcept(false)
{
  _poolAddress_(this);
  
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get bytes required for uncompressed row given the compressed size          */
/*----------------------------------------------------------------------------*/
i8 WorkingRow::nUncompressedBytesRequired(Row* aRow, ColumnVec *pColumnVec, i8 nCompressedRowSize)
{
  i8 i_VlqAdjustment = 0;
  u2 iOffset = -1;
  u2 iOffsetPrev = -1;
  
  i8 nColumns = pColumnVec->nItems();
  bool bVlqColumn = false;
  
  for (i8 i_Column = 0; i_Column < nColumns - 1; i_Column++) {
    
    Column* pColumn = pColumnVec->pGet(i_Column);
    
    char* pOffset = (char*)(aRow) + i_Column*C_COLUMN_OFFSET_SIZE;
    iOffsetPrev = iOffset;
    iOffset = *((u2*)pOffset);
    
    // was the previous column vlq column ?
    if (bVlqColumn) {
      bVlqColumn = false;
      i_VlqAdjustment += 8 - (iOffset - iOffsetPrev);
    }
    
    // check if vlq column
    if (pColumn->eDatumType() == DM_INT || pColumn->eDatumType() == DM_BCD)
      bVlqColumn = true;
  }
  
  // if last column was a vlq column, do the vlq adjustment
  if (bVlqColumn) {
    i_VlqAdjustment += 8 - (nCompressedRowSize - iOffset);
  }
  
  i8 nBytes =  nBytesRequired( nCompressedRowSize + i_VlqAdjustment);
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get bytes required for row incl rowSize                                    */
/*----------------------------------------------------------------------------*/
i8 WorkingRow::nBytesRequired(i8 nRowSize)
{
  i8 nBytes =   //sizeof(Item)   // item
  nRowSize      // row proper
  + sizeof(i8)  // m_nWorkingRowSize
  ;
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get row address                                                            */
/*----------------------------------------------------------------------------*/
Row* WorkingRow::aRow(void)
{
  _poolAddress_(this);
  
  Row* aRow = pAdd<Row*>(this,sizeof(i8));

  return aRow;
}

/*----------------------------------------------------------------------------*/
/* show the result row                                                        */
/*----------------------------------------------------------------------------*/
void WorkingRow::vShow(ColumnVec* pColVec, bool bCompressed)
{
  _poolAddress_(this);
  
  Row* aRow = this->aRow();
  
  aRow->vShow(pColVec, bCompressed);
}

/*----------------------------------------------------------------------------*/
/* get size in bytes of this result row                                       */
/*----------------------------------------------------------------------------*/
i8 WorkingRow::nRowSize(void)
{
  _poolAddress_(this);
  
  return m_nWorkingRowSize;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
