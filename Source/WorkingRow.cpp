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
void* WorkingRow::operator new(size_t iSize)
{
  RASSERT(iSize);
  vFatal("fatal: WorkingRow class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* WorkingRow::operator new(size_t iSize, WorkingRow* pWorkingRow)
{
  RASSERT(iSize);
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
  
  if (bCompressed) {
    MemLib::vCopy(aDstRow,aSrcRow,nRowSize,CN_WORKINGBUFFER);
  }
  else {
    
    i8 nColumns = pColumnVec->nItems() - 1;
    Stz* pOffset = (Stz*)aDstRow;
    Stz* aDst = pOffset + C_COLUMN_OFFSET_SIZE * nColumns;
    
    i8 iRowLength = C_COLUMN_OFFSET_SIZE * nColumns;
    
    for (i8 i_Column = 0; i_Column < nColumns; i_Column++) {
      
      Column* pColumn = pColumnVec->pGet(i_Column);
      eDM eDatumType = pColumn->eDatumType();
      
      char* pSrcOffset = (char*)(aSrcRow) + i_Column*C_COLUMN_OFFSET_SIZE;
      unsigned char iSrcOffset = *(pSrcOffset);
      
      // move to offset
      Stz* aSrcData = (Stz*)((unsigned char*)(aSrcRow) + iSrcOffset);
      i8 iColumnLength = DtmLib::nCopyVLQ(eDatumType, aDst, aSrcData);
      
      // save the offset
      *(pOffset->aChar()) = iRowLength;
      
      // move to next offset location
      pOffset += C_COLUMN_OFFSET_SIZE;
      
      // increment row length
      iRowLength += iColumnLength;
      
      // move to next column location
      aDst += iColumnLength;
      
    }
  }
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
WorkingRow::~WorkingRow(void)
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
  i8 iVlqColumns = 0;
  
  i8 i_VlqLength = 0;
  unsigned char iOffset = -1;
  unsigned char iOffsetPrev = -1;
  char* pOffset;
  Stz* aData;
  
  i8 nColumns = pColumnVec->nItems();
  bool bVlqColumn = false;
  
  for (i8 i_Column = 0; i_Column < nColumns - 1; i_Column++) {
    
    Column* pColumn = pColumnVec->pGet(i_Column);
    
    char* pOffset = (char*)(aRow) + i_Column*C_COLUMN_OFFSET_SIZE;
    iOffsetPrev = iOffset;
    iOffset = *(pOffset);
    
    if (bVlqColumn) {
      bVlqColumn = false;
      i_VlqLength += 8 - (iOffset - iOffsetPrev);
    }
    
    if (pColumn->eDatumType() == DM_INT || pColumn->eDatumType() == DM_BCD)
      bVlqColumn = true;
  }
  
  if (bVlqColumn) {
    i_VlqLength += 8 - (nCompressedRowSize - iOffset);
  }
  
  i8 nBytes =  nBytesRequired( nCompressedRowSize + i_VlqLength);
  
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
