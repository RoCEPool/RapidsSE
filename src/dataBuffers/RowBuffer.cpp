/*============================================================================*/
/* RowBufferr.hpp -- Row Buffer                                               */
/*============================================================================*/

#include "DataBuffers.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
RowBuffer* DataBuffers::_pRowBuffer(void* aAddress)
{
  RowBuffer* pRowBuffer = reinterpret_cast<RowBuffer*>(aAddress);
  
  return pRowBuffer;
}

__aNewFunction(RowBuffer,CN_ROWBUFFER)
__vDeleteFunction(RowBuffer,CN_ROWBUFFER)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowBuffer::RowBuffer(void)
: DataBuffer(CN_ROWBUFFER, false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER);
  
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowBuffer::RowBuffer(eCN eClassNo)
: DataBuffer(eClassNo, false)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);
  

}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowBuffer::RowBuffer(eCN eClassNo, StreamDef* pStreamDef)
: DataBuffer(eClassNo, false)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);
  
  vResolveRowPix(pStreamDef);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RowBuffer::~RowBuffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot a ref set in this buffer (may return nulltpr)                        */
/*----------------------------------------------------------------------------*/
Row* RowBuffer::aAllotCopyRow(RowSingle* pRowSingle)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  RASSERT(m_oRowPix.m_bInitialised);

  StreamDef* pStreamDef = pRowSingle->m_pStreamDef;
  
  //i8 nHeadSize = Row::nSizeBytes(pStreamDef);
  i8 nVchSize = pRowSingle->nSumVchSizes();
  
  if (! bHasSpaceFor(nVchSize + m_nRowSizeProper))
    return nullptr;
  
  Row* pRow = aAllotNewRow();
  
  new(pRow) Row;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy single row into this buffer
  
  for (i8 i_Col = 0; i_Col < pStreamDef->nColumns(C_ALL); i_Col++) {
    
    Column* pCol = pStreamDef->pGetColumn(i_Col);
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // vch
    
    if (DM_VCH == pCol->eDatum()) {
      
      // check for null
      if (pRowSingle->bIsColNull(i_Col)) {
        pRow->vSetColNull(i_Col);
        continue;
      }
      
      // get src string
      char* acSrcString = (char*)pRowSingle->szColVal(i_Col);
      i8 nSrcLength = StrLib::nSize(acSrcString);
      
      // STRING_DEPOSIT
      // allocate a string from the tail
      Stz* acDstString = ayAllotHead(nSrcLength);
      i8 nCopyBytes = StrLib::nCopy((char*)acDstString, acSrcString);
      ASSERT(nCopyBytes + 1 == nSrcLength);
      // set offset of tail string
      pRow->vColVal_ofs(i_Col,iToOffset(acDstString));
      // END STRING_DEPOSIT

      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // all others
    
    pRow->vColVal_int(i_Col,pRowSingle->iColVal(i_Col));
  }
  
  //pRow->vDump(pStreamDef);
  
  return pRow;
}

/*----------------------------------------------------------------------------*/
/* print rows                                                                 */
/*----------------------------------------------------------------------------*/
void RowBuffer::vPrintRows(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  HV_Record hvRecord(aRowPix());

  i8 nRows = this->nRows();

  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
    hvRecord->vToCsvRecord(pRowAddress(i_Row));
    hvRecord->vPrintAsRow();
  }
}

/*----------------------------------------------------------------------------*/
/* print rows                                                                 */
/*----------------------------------------------------------------------------*/
void RowBuffer::vPrintRows(StreamDef* pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  HV_Record hvRecord(pStreamDef->aRowPix());
  
  // NOTE: We need to print only nItem no of rows
  // as the limitOp may have truncated the rows via setting the nItems()
  
  // get the first row
  Row* pRow = aFirstRow();
  HV_RowSingle hvRowSingle(pStreamDef);
  
  for (i8 i_Row = 0; i_Row < nItems(); i_Row++) {
    hvRowSingle->vCopyRow(pRow);
    hvRecord->vToCsvRecord(hvRowSingle->pRowCurrent());
    hvRecord->vPrintAsRow();
    
    // get the next row
    pRow = aNextRow(pRow);
  }
}

/*----------------------------------------------------------------------------*/
/* show contents of buffer                                                    */
/*----------------------------------------------------------------------------*/
void RowBuffer::vShow(StreamDef* pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  i8 nRows = nItems();
  
  vTrace("RowBuffer: %p (buffer %lld): rows: %lld\n"_Stz,
         this,m_iBufferNo,nRows);
  
  vTrace("  prevBuffer %p nextBuffer %p\n"_Stz,
         m_pPrevBuffer,m_pNextBuffer);
  
  vTrace("  partition: %lld \n"_Stz, m_tPtnNo);
  
  Row* pRow = aFirstRow();
  HV_RowSingle hvRowSingle(pStreamDef);
  
  HV_Record hvRecord(pStreamDef->aRowPix());
  while (pRow) {
    hvRowSingle->vCopyRow(pRow);
    hvRecord->vToCsvRecord(hvRowSingle->pRowCurrent());
    pRow = aNextRow(pRow);
    hvRecord->vPrintAsRow();
  }
  
  vTrace("--------------------------------------------------"
         "--------------------------------------------------\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* convert this row buffer into one or more csv buffers                       */
/*----------------------------------------------------------------------------*/
CsvBuffer* RowBuffer::pToCsvBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  SP_RowPix spRowPix(this->nColumns(),axaColPix());
  
  CsvBuffer* pFirstCsvBuffer = new CsvBuffer;
  CsvBuffer* pCsvBuffer = pFirstCsvBuffer;
  
  Stz* szRecord = nullptr;
  i8 nSize = 1;
  
  i8 nRows = this->nRows();
  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
    
    Row* aRow = pRowAddress(i_Row);
    
    Stz* szRow = spRowPix.sz(aRow,true);

    nSize = szRow->nSize();
    
    if (pCsvBuffer->nFreeBytes() < nSize + 1) {
      CsvBuffer* pNewCsvBuffer = new CsvBuffer;
      pCsvBuffer->vNextBuffer(pNewCsvBuffer);
      pCsvBuffer = pNewCsvBuffer;
    }

    szRecord = pCsvBuffer->szAllotRecord(nSize - 1);

    StrLib::vCopyLen(szRecord->azChar(),szRow->azChar(),nSize - 1);
  }
  
  if (szRecord)
    ((char*)szRecord)[nSize-1] = '\0';
  
  return pFirstCsvBuffer;
}

/*----------------------------------------------------------------------------*/
/* calculate the absolute offset within buffer for given item                 */
/*----------------------------------------------------------------------------*/

// I THINK THIS NEEDS TO GO

i8 RowBuffer::iToOffset(Stz* szStr)
{
  i4 iOffset = static_cast<i4>(nDifference(szStr,this));
  
  return iOffset;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
