/*============================================================================*/
/* RowBuffer.cpp -- Row Buffer                                               */
/*============================================================================*/

#include "BASE.h"

#include "RowBuffer.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aNewFunction(RowBuffer,CN_ROWBUFFER)
__vDeleteFunction(RowBuffer,CN_ROWBUFFER)
__aFooterFunction(RowBuffer);
__pCastFunction(STORE, RowBuffer, CN_ROWBUFFER);

/*----------------------------------------------------------------------------*/
/* get address of cell array of row                                           */
/*----------------------------------------------------------------------------*/
void* aRowBuffer_GetCellArray(void* pRowBuffer, i8 iRow)
{
  return RowBuffer::aGetCellArray((RowBuffer*)pRowBuffer, iRow);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowBuffer::RowBuffer(void)
: DataBuffer(CN_ROWBUFFER, false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER);

  LogicUnit* pLogicUnit = MY::pOpLogicUnit();
  if (pLogicUnit)
    pLogicUnit->m_xpStats->m_nNewRowBuffer++;

  m_bDeletable = false;

#ifdef WITH_BUFFER_ID
#ifdef WITH_HASHMAP
  AM::pHashMap()->vInsertKeyValue((void *) m_oBufferId.m_pvAsAddress, (Object *) this);
#endif
#endif
  
  m_oDecider.vSetRemaining(1);
  
  AM::METRICS::vDerefMetrics((i8)this,CN_CRWBUFFER,DR_NEW);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowBuffer::RowBuffer(eCN eClassNo)
: DataBuffer(eClassNo, false)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);

  RASSERT(CN_ROWBUFFER != eClassNo);
  
  LogicUnit* pLogicUnit = MY::pOpLogicUnit();
  if (pLogicUnit)
    pLogicUnit->m_xpStats->m_nNewRowBuffer++;

  m_bDeletable = true;

#ifdef WITH_BUFFER_ID
#ifdef WITH_HASHMAP
  AM::pHashMap()->vInsertKeyValue((void *) m_oBufferId.m_pvAsAddress, (Object *) this);
#endif
#endif
  
  if (CN_CRWBUFFER == eClassNo)
    AM::METRICS::vDerefMetrics((i8)this,CN_CRWBUFFER,DR_NEW);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowBuffer::RowBuffer(eCN eClassNo, StreamDef* pStreamDef)
: DataBuffer(eClassNo, false)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo);
  
  RASSERT(CN_ROWBUFFER != eClassNo);

  vResolveRowPix(pStreamDef);

  LogicUnit* pLogicUnit = MY::pOpLogicUnit();
  if (pLogicUnit)
    pLogicUnit->m_xpStats->m_nNewRowBuffer++;

  m_bDeletable = true;

#ifdef WITH_BUFFER_ID
#ifdef WITH_HASHMAP
  AM::pHashMap()->vInsertKeyValue((void *) m_oBufferId.m_pvAsAddress, (Object *) this);
#endif
#endif
  
  if (CN_ROWBUFFER == eClassNo)
    AM::METRICS::vDerefMetrics((i8)this,CN_ROWBUFFER,DR_NEW);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RowBuffer::~RowBuffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  RASSERT(m_bDeletable);
  
  LogicUnit* pLogicUnit = MY::pOpLogicUnit();
  if(pLogicUnit)
    pLogicUnit->m_xpStats->m_nDelRowBuffer++;
  
  if (CN_ROWBUFFER == eGetClassNo())
    AM::METRICS::vDerefMetrics((i8)this,CN_ROWBUFFER,DR_NEW);
#ifdef WITH_BUFFER_ID
#ifdef WITH_HASHMAP
  AM::pHashMap()->vDeleteKey((void*)m_oBufferId.m_pvAsAddress);
#endif
#endif
}

/*----------------------------------------------------------------------------*/
/* post recv function                                                         */
/*----------------------------------------------------------------------------*/
void RowBuffer::vPostRecv(void* aRowBuffer)
{
  _static_
  
  RowBuffer* pRowBuffer = _pRowBuffer(aRowBuffer);
  
  pRowBuffer->vRestorePointers();
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get address of cell array of row                                           */
/*----------------------------------------------------------------------------*/
void* RowBuffer::aGetCellArray(RowBuffer* pRowBuffer, i8 iRow)
{
  _static_

  Row* pRow = pRowBuffer->pRowAddress(iRow);
  m8* aArray = (m8*) & pRow->m_wxaColumnCell[0];

  return (void*)aArray;
}

/*----------------------------------------------------------------------------*/
/* repair                                                                     */
/*----------------------------------------------------------------------------*/
void RowBuffer::vRepairList(Object* pObject)
{
  _static_
  RowBuffer* pRowBuffer = _pRowBuffer(pObject);
  RowBuffer* pNextRowBuffer;

  while (pRowBuffer) {
    pNextRowBuffer = pRowBuffer->pNextRowBuffer();
    AM::vDeleteOperand(pRowBuffer);
    pRowBuffer = pNextRowBuffer;
  }
}

/*----------------------------------------------------------------------------*/
/* get the row buffer that contains the given address                         */
/*----------------------------------------------------------------------------*/
RowBuffer* RowBuffer::pToRowBuffer(void* aAddr)
{
  _static_
  
  DataBuffer* pDataBuffer = pToDataBuffer(aAddr);
  
  RowBuffer* pRowBuffer =
    reinterpret_cast<RowBuffer*>(pDataBuffer);
    
  return pRowBuffer;
}

/*----------------------------------------------------------------------------*/
/* get the row buffer that given id references                                */
/*----------------------------------------------------------------------------*/
RowBuffer* RowBuffer::pRefdRowBuffer(Rrf& xRef)
{
  _static_

  i8 i_PoolBuffer = xRef.iBuffer();

  HeapBuffer* pHeapBuffer =
      reinterpret_cast<HeapBuffer*>(FileMap::pToPoolBuffer(i_PoolBuffer));

  RowBuffer* pRowBuffer =
      reinterpret_cast<RowBuffer*>(pHeapBuffer->aFirstChunk());

  ASSERT(pRowBuffer);
  ASSERT(pRowBuffer->eGetClassNo() == CN_ROWBUFFER);

  return pRowBuffer;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot a ref set in this buffer (may return nulltpr)                        */
/*----------------------------------------------------------------------------*/
Row* RowBuffer::pAllotCopyRow(RowSingle* pRowSingle)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  RASSERT(m_pRowPix);

  StreamDef* pStreamDef = pRowSingle->m_pStreamDef;
  
  //i8 nHeadSize = Row::nSizeBytes(pStreamDef);
  i8 nVchSize = pRowSingle->nSumVchSizes();
  
  if (! bHasSpaceFor(nVchSize + m_nRowSizeProper))
    return nullptr;
  
  Row* pRow = pAllotRow();
  
  new(pRow) Row;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy single row into this buffer
  
  for (i8 i_Col = 0; i_Col < pStreamDef->nColumns(); i_Col++) {
    
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
      Stz* acDstString = aAllotHead(nSrcLength);
      i8 nCopyBytes = StrLib::nCopy((char*)acDstString, acSrcString);
      ASSERT(nCopyBytes + 1 == nSrcLength);
      UNUSED(nCopyBytes);
      // set offset of tail string
      //pRow->vColVal_ofs(i_Col,iToOffset(acDstString));
      pRow->vColVal_ofs(i_Col,acDstString);
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
/* allot a new row                                                            */
/*----------------------------------------------------------------------------*/
Row* RowBuffer::pAllotRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  // allot a row
  Row* pRow = RowBox_t::pAllotRow();
  
  return pRow;
}

/*----------------------------------------------------------------------------*/
/* allot a row copying the rowSingle into it                                  */
/*----------------------------------------------------------------------------*/
Row* RowBuffer::pAllotRow(i8 iVArgsBegin, ...)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  ASSERT(C_VARGS_BEGIN == iVArgsBegin);

  Row* pRow = RowBuffer::pAllotRow();
  if (! pRow)
    return nullptr;

  va_list xVarArgs;
  va_start(xVarArgs,iVArgsBegin);

  bool b = bDepositColumns(pRow,xVarArgs);
  RASSERT(b);

  va_end(xVarArgs);

  return pRow;
}

/*----------------------------------------------------------------------------*/
/* allot a row copying the rowSingle into it                                  */
/*----------------------------------------------------------------------------*/
Row* RowBuffer::pAllotRow(StreamDef* pStreamDef, i8 iVArgsBegin, ...)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  // allot a row
  Row* pRow = RowBuffer::pAllotRow();
  
  Stz* szDst;
  
  if ( ! pRow )
    return nullptr;
  
  ASSERT(C_VARGS_BEGIN == iVArgsBegin);
  
  va_list xVarArgs;
  
  va_start(xVarArgs,iVArgsBegin);
  
  for (i8 i_Col = 0; i_Col < pStreamDef->nColumns(); i_Col++) {
  
    Column* pCol = pStreamDef->pGetColumn(i_Col);
    
    // STRING_DEPOSIT
    if (DM_VCH == pCol->eDatum()) {
      Stz* szVal = va_arg(xVarArgs,Stz*);
      if (szVal) {
        szDst = (Stz*)aAllotHead(StrLib::nSize(szVal->azChar()));
        
        // return early if no more space for the string
        if ( ! szDst )
          return nullptr;
        
        StrLib::vCopy((char*)szDst, szVal->azChar());

        pRow->vColVal_ofs(i_Col,szDst);
      }
      else {
        pRow->vSetColNull(i_Col);
      }
    }
    else {
      i8 iVal = va_arg(xVarArgs,i8);
      pRow->vColVal_int(i_Col,iVal);
    }
    // END STRING_DEPOSIT

  }
  
  i8 iVArgsEnd = va_arg(xVarArgs,i8);
  ASSERT(C_VARGS_END == iVArgsEnd);
  UNUSED(iVArgsEnd);

  va_end(xVarArgs);
  
  return pRow;
}

/*----------------------------------------------------------------------------*/
/* allot a row and deposit given row into it                                  */
/*----------------------------------------------------------------------------*/
Row* RowBuffer::pAllotRow(Row* pRow)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  RowPix* pPix = pRowPix();

  // allot a row
  Row* pNewRow = RowBuffer::pAllotRow();

  Stz* szDst;

  if ( ! pNewRow )
    return nullptr;

  for (i8 i_Col = 0; i_Col < pPix->m_nColumns; i_Col++) {

    ColPix xColPix = pPix->m_xaColPix[i_Col];

    // STRING_DEPOSIT
    if (DM_VCH == xColPix.m_uDatum) {
      Stz* szVal = pRow->szColVal_ofs(i_Col);
      if (szVal) {
        szDst = (Stz*)aAllotHead(StrLib::nSize(szVal->azChar()));

        // return early if no more space for the string
        if ( ! szDst )
          return nullptr;

        StrLib::vCopy((char*)szDst, szVal->azChar());

        pNewRow->vColVal_ofs(i_Col,szDst);
      }
      else {
        pNewRow->vSetColNull(i_Col);
      }
    }
    else {
      pNewRow->vColVal_int(i_Col,pRow->iColVal_int(i_Col));
    }
    // END STRING_DEPOSIT

  }

  return pNewRow;
}

/*----------------------------------------------------------------------------*/
/* get decile full of the buffer                                              */
/*----------------------------------------------------------------------------*/
i8 RowBuffer::iDecile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  i8 iSpaceUsed = nUsedBytes();
  i8 iDecile = IntLib::iDecile( iSpaceUsed , C_SIZE_POOL_BUFFER);
  
  return iDecile;
}

/*----------------------------------------------------------------------------*/
/* delete this row buffer                                                     */
/*----------------------------------------------------------------------------*/
void RowBuffer::vDeciderCopies(i8 nCopies)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  m_oDecider.vResetRemaining(nCopies);
}

/*----------------------------------------------------------------------------*/
/* delete this row buffer                                                     */
/*----------------------------------------------------------------------------*/
void RowBuffer::vDecidedDelete(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  i8 nAcksWereRemaining;
  
  if (m_oDecider.bDecAndDecide(nAcksWereRemaining)) {
    
    m_bDeletable = true;
  
    delete this;
    // dont touch this pointer
  }
}

/*----------------------------------------------------------------------------*/
/* deposit var-arg list of column values into given row                       */
/*----------------------------------------------------------------------------*/
bool RowBuffer::bDepositColumns(Row* pRow, va_list xVarArgs)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  ColPix* axaColPix = RowBox_t::axaColPix();

  for (i8 i_Col = 0; i_Col < nColumns(); i_Col++) {

    ColPix* pColPix = &axaColPix[i_Col];

    if (DM_VCH == pColPix->m_uDatum) {
      Stz* szVal = va_arg(xVarArgs,Stz*);
      if (szVal) {
        Stz* szDst = (Stz*)aAllotHead(StrLib::nSize(szVal->azChar()));

        // return early if no more space for the string
        if ( ! szDst )
          return false;

        StrLib::vCopy((char*)szDst, szVal->azChar());

        pRow->vColVal_ofs(i_Col,szDst);
      }
      else {
        pRow->vSetColNull(i_Col);
      }
    }
    else {
      i8 iVal = va_arg(xVarArgs,i8);
      pRow->vColVal_int(i_Col,iVal);
    }

  }

  i8 iVArgsEnd = va_arg(xVarArgs,i8);
  ASSERT(C_VARGS_END == iVArgsEnd);
  UNUSED(iVArgsEnd);

  return true;
}

/*----------------------------------------------------------------------------*/
/* deposit var-arg list of column values into given row                       */
/*----------------------------------------------------------------------------*/
void RowBuffer::vDepositRow(Row* pRow, i8 iVargsBegin, ...)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  ASSERT(C_VARGS_BEGIN == iVargsBegin);

  va_list xVarArgs;
  va_start(xVarArgs,iVargsBegin);

  bool b = bDepositColumns(pRow,xVarArgs);
  RASSERT(b);
  
  va_end(xVarArgs);
}

/*----------------------------------------------------------------------------*/
/* get next buffer                                                            */
/*----------------------------------------------------------------------------*/
RowBuffer* RowBuffer::pNextRowBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  return _pRowBuffer(this->pNextLinkedBuffer());
}

/*----------------------------------------------------------------------------*/
/* set next buffer                                                            */
/*----------------------------------------------------------------------------*/
void RowBuffer::vNextRowBuffer(RowBuffer* pNextBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  LinkedBuffer::vNextLinkedBuffer(pNextBuffer);
}

/*----------------------------------------------------------------------------*/
/* get prev buffer                                                            */
/*----------------------------------------------------------------------------*/
RowBuffer* RowBuffer::pPrevRowBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  return _pRowBuffer(this->pPrevLinkedBuffer());
}

/*----------------------------------------------------------------------------*/
/* set prev buffer                                                            */
/*----------------------------------------------------------------------------*/
void RowBuffer::vPrevRowBuffer(RowBuffer* pPrevBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  vPrevLinkedBuffer(pPrevBuffer);
}

/*----------------------------------------------------------------------------*/
/* print rows                                                                 */
/*----------------------------------------------------------------------------*/
void RowBuffer::vPrintRows(File* pFilePtr, Stz* szLabel)
{
  _precondition_(_ADDRESS_,_MAGIC_);

  RowPix* pPix = pRowPix();
  
  //pPix->vPrint();

  HV_Record hvRecord(pPix);

  i8 nRows = this->nRows();

  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
    hvRecord->vToCsvRecord(pRowAddress(i_Row));
    hvRecord->vPrintAsRow(szLabel);
    
    if (pFilePtr)
      hvRecord->vWriteAsCsv(pFilePtr);
  }
}

/*----------------------------------------------------------------------------*/
/* print rows                                                                 */
/*----------------------------------------------------------------------------*/
void RowBuffer::vPrintRows(StreamDef* pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  RowPix* pPix = pStreamDef->pRowPix();
  
  //pPix->vPrint();
  
  HV_Record hvRecord(pPix);
  
  // NOTE: We need to print only nItem no of rows
  // as the limitOp may have truncated the rows via setting the nItems()
  
  // get the first row
  Row* pRow = pFirstRow();
  HV_RowSingle hvRowSingle(pStreamDef);
  
  for (i8 i_Row = 0; i_Row < nItems(); i_Row++) {
    hvRowSingle->vCopyRow(pRow);
    hvRecord->vToCsvRecord(hvRowSingle->pRowCurrent());
    hvRecord->vPrintAsRow();
    
    // get the next row
    pRow = pNextRow(pRow);
  }
}

/*----------------------------------------------------------------------------*/
/* restore pointers are recv()                                                */
/*----------------------------------------------------------------------------*/
void RowBuffer::vRestorePointers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_);
    
  RowBox_t<C_WSP_ROWBUFFER>::vResetRowPix();
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
         pPrevLinkedBuffer(),pNextLinkedBuffer());

  vTrace("  partition: %lld \n"_Stz, m_tLglPtn.iAsI8());
  
  Row* pRow = pFirstRow();
  HV_RowSingle hvRowSingle(pStreamDef);
  
  HV_Record hvRecord(pStreamDef->pRowPix());
  while (pRow) {
    hvRowSingle->vCopyRow(pRow);
    hvRecord->vToCsvRecord(hvRowSingle->pRowCurrent());
    pRow = pNextRow(pRow);
    hvRecord->vPrintAsRow();
  }
  
  vTrace("--------------------------------------------------"
         "--------------------------------------------------\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* get string area                                                            */
/*----------------------------------------------------------------------------*/
char* RowBuffer::azStrArea(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  char* azArea = ((char*)ayLatterHead()) + 1;

  return azArea;
}

/*----------------------------------------------------------------------------*/
/* convert this row buffer into one or more csv tub buffers                   */
/*----------------------------------------------------------------------------*/
void RowBuffer::vToCsv(Tub* pTub)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  HV_Record hvRecord(pRowPix());
    
  CsvBuffer* pCsvBuffer = _pCsvBuffer(pTub->pWorkingBuffer());
  if (! pCsvBuffer) {
    pCsvBuffer = new CsvBuffer;
    pTub->vAddBuffer(pCsvBuffer);
  }

  i8 nSize = 0;
  Stz* szRecord = nullptr;
  
  i8 nRows = this->nRows();
  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
    
    Row* aRow = pRowAddress(i_Row);
    Stz* szRow = hvRecord->sz(aRow,true);
    nSize = szRow->nSize();
    
    if (pCsvBuffer->nFreeBytes() < nSize + 1) {
      pCsvBuffer = new CsvBuffer;
      pTub->vAddBuffer(pCsvBuffer);
    }

    szRecord = pCsvBuffer->szAllotRecord(nSize - 1);
    StrLib::vCopyLen(szRecord->azChar(),szRow->azChar(),nSize - 1);
    
    //vTrace("At %lld : %s\n"_Stz,i_Row,pCsvBuffer->szRecords());
    //vDebugPoint();
  }
  
  
  if (szRecord)
    ((char*)szRecord)[nSize-1] = '\0';

  //vTrace("End: %s\n"_Stz,pCsvBuffer->szRecords());
  //vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* convert this row buffer into one or more csv buffers                       */
/*----------------------------------------------------------------------------*/
CsvBuffer* RowBuffer::pToCsvBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);

  HV_Record hvRecord(pRowPix());
  
  CsvBuffer* pFirstCsvBuffer = new CsvBuffer;
  CsvBuffer* pCsvBuffer = pFirstCsvBuffer;
  
  Stz* szRecord = nullptr;
  i8 nSize = 1;
  
  i8 nRows = this->nRows();
  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
    
    Row* aRow = pRowAddress(i_Row);
    
    Stz* szRow = hvRecord->sz(aRow,true);

    nSize = szRow->nSize();
    
    if (pCsvBuffer->nFreeBytes() < nSize + 1) {
      CsvBuffer* pNewCsvBuffer = new CsvBuffer;
      pCsvBuffer->vNextLinkedBuffer(pNewCsvBuffer);
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
/* truncate this buffer by removing all entries                               */
/*----------------------------------------------------------------------------*/
void RowBuffer::vTruncate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* verify this row buffer                                                 TBD */
/*----------------------------------------------------------------------------*/
bool RowBuffer::bVerify(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* calculate the absolute offset within buffer for given item                 */
/*----------------------------------------------------------------------------*/

// I THINK THIS NEEDS TO GO

//i8 RowBuffer::iToOffset(Stz* szStr)
//{
//  i4 iOffset = static_cast<i4>(nDifference(szStr,this));
//
//  return iOffset;
//}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
