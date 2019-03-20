/*============================================================================*/
/* RowBuffer.cpp -- Row Buffer                                                */
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
RowBuffer* Data::_pRowBuffer(void* aAddress)
{
  RowBuffer* pRowBuffer = reinterpret_cast<RowBuffer*>(aAddress);
  
  RASSERT_TYPE(pRowBuffer,CN_ROWBUFFER);
  
  return pRowBuffer;
}

/*----------------------------------------------------------------------------*/
/* allocate a new row buffer from heap                                        */
/*----------------------------------------------------------------------------*/
void* RowBuffer::operator new(size_t)
{
  RowBuffer* pRowBuffer =
    static_cast<RowBuffer*>(HeapMem::pAllocate(CN_ROWBUFFER));
  
  return pRowBuffer;
}

/*----------------------------------------------------------------------------*/
/* perform new of row buffer for given address                                */
/*----------------------------------------------------------------------------*/
void* RowBuffer::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free a row buffer back to the heap                                         */
/*----------------------------------------------------------------------------*/
void RowBuffer::operator delete(void* aAddress)
{
  RowBuffer* pRowBuffer = static_cast<RowBuffer*>(aAddress);
  
  HeapMem::vFree(pRowBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowBuffer::RowBuffer(void)
: Object(CN_ROWBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  m_okLock.vInitLock(LK_ROWBUFFER);
  
  m_iBufferNo     = *nPoolBufferNo();
  m_pIndex        = nullptr;
  m_iPartitionNo  = -1;
  m_nRows         = 0;
  m_iForwardFree  = 0;
  m_iBackwardFree = C_MAX_ROWS - 1;
  m_pNextBuffer   = nullptr;
  m_pPrevBuffer   = nullptr;
  
  for (i8 iIndex = 0 ; iIndex < C_MAX_ROWS; iIndex++)
    m_xRow[iIndex].vSetAll(0);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  _checkInitialised_(CN_ROWBUFFER);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RowBuffer::~RowBuffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump entry buffer                                                          */
/*----------------------------------------------------------------------------*/
void RowBuffer::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RowBuffer* pRowBuffer = _pRowBuffer(pObject);
  
  pRowBuffer->vShow();
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* analyze this buffer                                                        */
/*----------------------------------------------------------------------------*/
void RowBuffer::vAnalyze(s48K& sAnalysis, bool bDetailed,
                           i8& iBuffers, i8& iRows)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  if (m_pIndex->bIsHidden()) {
    
    RowBuffer* pNextBuffer = this;
    
    do {
      
      iBuffers++;
      iRows += (pNextBuffer->m_nRows);
      
      if (bDetailed)
        sAnalysis.vCatSprint("row: (%s, %lld, %5lld, %lld )\n"_Stz,
                             gpAttachMap->m_pGrid->pThisNode()->aNodeName(),
                             pNextBuffer->m_iPartitionNo,
                             iBuffers,
                             pNextBuffer->m_nRows);
      
      pNextBuffer = pNextBuffer->pNextBuffer();
    } while (pNextBuffer);
    
    return;
  }
  
}

/*----------------------------------------------------------------------------*/
/* allot a row in this buffer                                                 */
/*----------------------------------------------------------------------------*/
CacheLine* RowBuffer::aAllotRow(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // check if this buffer has cacheLine's for rows
  
  if ( ! bHasSpace(nBytes))
    return nullptr;
  
  i8 iRowNo = m_iForwardFree++;
  m_nRows++;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  return &m_xRow[iRowNo];
}

/*----------------------------------------------------------------------------*/
/* append a row entry into this buffer                                        */
/*----------------------------------------------------------------------------*/
CacheLine* RowBuffer::aAppendRow(CacheLine* aRowData)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // allot row entry in this buffer
  
  bn nBufferNo = this->nPoolBufferNo();

  // NOTE:: Assuming each row fits in a cacheline
  CacheLine* pRow = aAllotRow(C_BYTES_PER_CACHE_LINE);
  if (! pRow)
    return nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy row data into row entry
  
  MemLib::vCopy(pRow,aRowData,C_BYTES_PER_CACHE_LINE,CN_ROWBUFFER);
  //pRowEntry->vShow(0, m_pIndex, false);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  //vDumpItems();
  return pRow;
}

/*----------------------------------------------------------------------------*/
/* assert valid row buffer                                                    */
/*----------------------------------------------------------------------------*/
void RowBuffer::vAssertRowBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* drop lock for write                                                        */
/*----------------------------------------------------------------------------*/
void RowBuffer::vDropWrite(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  m_okLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void RowBuffer::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  // show this buffer
  vShow();
}

/*----------------------------------------------------------------------------*/
/* grab lock for write                                                        */
/*----------------------------------------------------------------------------*/
void RowBuffer::vGrabWrite(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  m_okLock.vGrabWrite();
}

/*----------------------------------------------------------------------------*/
/* does buffer have space for nBytes ?                                        */
/*----------------------------------------------------------------------------*/
bool RowBuffer::bHasSpace(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  i8 nFreeCacheLines = (C_MAX_ROWS - m_iForwardFree) + (C_MAX_ROWS - m_iBackwardFree - 1);
  i8 nFreeBytes = nFreeCacheLines * C_BYTES_PER_CACHE_LINE;
  
  if (gpAttachMap->m_nIndexEntries > 0) {
    // has space calculation based on no of entries
    if (m_nRows >= gpAttachMap->m_nIndexEntries)
      return false;
    else
      return true;
  }
  else {
    // has space calculation based on free space
    if (nFreeBytes < nBytes)
      return false;
    else
      return true;
  }
}

/*----------------------------------------------------------------------------*/
/* set the index of the index buffer                                          */
/*----------------------------------------------------------------------------*/
void RowBuffer::vIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  RASSERT(pIndex->bIsHidden());
  m_pIndex = pIndex;
}

/*----------------------------------------------------------------------------*/
/* get the index of the index buffer                                          */
/*----------------------------------------------------------------------------*/
Index* RowBuffer::pIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  return m_pIndex;
}

/*----------------------------------------------------------------------------*/
/* get next buffer                                                            */
/*----------------------------------------------------------------------------*/
RowBuffer* RowBuffer::pNextBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  return _pRowBuffer(m_pNextBuffer);
}

/*----------------------------------------------------------------------------*/
/* set next buffer                                                            */
/*----------------------------------------------------------------------------*/
void RowBuffer::vNextBuffer(RowBuffer* pNextBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  m_pNextBuffer = pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* set partition no                                                           */
/*----------------------------------------------------------------------------*/
void RowBuffer::vPartitionNo(i8 iPartitionNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  m_iPartitionNo = iPartitionNo;
}

/*----------------------------------------------------------------------------*/
/* get partition no                                                           */
/*----------------------------------------------------------------------------*/
i8 RowBuffer::iPartitionNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  return m_iPartitionNo;
}

/*----------------------------------------------------------------------------*/
/* get prev buffer                                                            */
/*----------------------------------------------------------------------------*/
RowBuffer* RowBuffer::pPrevBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  return _pRowBuffer(m_pPrevBuffer);
}

/*----------------------------------------------------------------------------*/
/* set prev buffer                                                            */
/*----------------------------------------------------------------------------*/
void RowBuffer::vPrevBuffer(RowBuffer* pPrevBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  m_pPrevBuffer = pPrevBuffer;
}

/*----------------------------------------------------------------------------*/
/* get row                                                                    */
/*----------------------------------------------------------------------------*/
CacheLine* RowBuffer::pRow(i8 iRowNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  return &m_xRow[iRowNo];
}

/*----------------------------------------------------------------------------*/
/* get the no of rows in the buffer                                           */
/*----------------------------------------------------------------------------*/
i8 RowBuffer::nRowCount(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  return m_nRows;
}

/*----------------------------------------------------------------------------*/
/* restore the buffer after being received over a socket                      */
/*----------------------------------------------------------------------------*/
void RowBuffer::vRestore(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  RASSERT( ! m_pNextBuffer );
  RASSERT( ! m_pPrevBuffer );
  
  // set the index to nullptr (this pointer is not valid any more)
  m_pIndex = nullptr;
  
  // get the original buffer address
  //  nb: the given address has to be an address in the orginal buffer
  //PoolBuffer* pOriginalBuffer =
  //pBitAnd<PoolBuffer*>(m_i4aItemOffsets,C_OBJECT_TO_BUFFER_MASK);
  
  // get this buffer address
  PoolBuffer* pThisBuffer =
  pBitAnd<PoolBuffer*>(this,C_OBJECT_TO_BUFFER_MASK);
  
  // get the buffer no
  bn nBuffer = nPoolBufferNo();
  
  // restore the bufferNo
  m_iBufferNo = *nBuffer;
  
  // now restore the row id's to point to this buffer no
//  for (Item* aItem = aLeadEntry();
//       aItem != nullptr;
//       aItem = aNextEntry(aItem, 0)) {
//
//    Id xId(*nBuffer, aItem->iItemNo(), aItem->nLevels());
//    aItem->vId(xId);
//  }
  
}

/*----------------------------------------------------------------------------*/
/* restore the index of the index buffer                                      */
/*----------------------------------------------------------------------------*/
void RowBuffer::vRestoreIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  //RASSERT( ! m_pIndex );
  m_pIndex = pIndex;
}

/*----------------------------------------------------------------------------*/
/* show entry                                                                 */
/*----------------------------------------------------------------------------*/
void RowBuffer::vShowRow(i8 iRowNo)
{
  vTrace("Id: %3lli.%03lli "_Stz,
         m_iBufferNo,
         iRowNo
         );

  vTrace("RowData: "_Stz);
  
  ColumnVec* pColumnVec = m_pIndex->pTable()->pColumnVec();
  
#ifdef WITH_ROWBUFFER
  for (i8 iCol = 0; iCol < pColumnVec->nItems() - 1; iCol++) {
    CacheLine* aRowCacheLine = &m_xRow[iRowNo];
    //w8 wVal = RowEncoding::wDecode(pColumnVec->pGet(iCol), aRowCacheLine);
    //vTrace("%lld "_Stz, wVal);
    RowEncoding::vPrint(pColumnVec->pGet(iCol),aRowCacheLine);
  }
#endif
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* show contents of buffer                                                    */
/*----------------------------------------------------------------------------*/
void RowBuffer::vShow(Item* aMarkEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  vTrace("RowBuffer: %p (buffer %lld): rows: %lld forwardFree: %lld backwardFree: %lld free: %lld\n"_Stz,
         this,m_iBufferNo,m_nRows,m_iForwardFree,m_iBackwardFree,
         (C_MAX_ROWS - m_iForwardFree) + (C_MAX_ROWS - m_iBackwardFree - 1));
  
  vTrace("  prevBuffer %p nextBuffer %p\n"_Stz,
         m_pPrevBuffer,m_pNextBuffer);
  
  vTrace("  index: %s partition: %lld \n"_Stz,
         m_pIndex ? m_pIndex->aIndexName() : "NULL"_Stz,m_iPartitionNo);
  
  vTrace("--------------------------------------------------"
         "--------------------------------------------------\n"_Stz);
  
  vTrace("\n"_Stz);
  vTrace("Rows (by cacheLine):\n"_Stz);
  
  for (i8 iIndex = 0; iIndex < m_nRows; iIndex++)
    vShowRow(iIndex);
  
  vTrace("\n"_Stz);
  vTrace("=================================================="
         "==================================================\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* show short form of buffer                                                  */
/*----------------------------------------------------------------------------*/
void RowBuffer::vShowShort(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  vTrace("[t%lld: "_Stz, *nPoolBufferNo());
  
//  i8 iOffset = aSkipRoots()->iNEO(0);
//  while (iOffset > -1) {
//    Item* aItem = aGetEntryOffset(iOffset);
//
//    KeyData* aKey = _aKeyData(aItem->aDataArea())
//    ;
//    i8 iVal = aKey->iGetInt();
//
//    if (m_iBufferHeight)
//      vTrace("%lld(%lld)"_Stz,iVal,*EntryBuffer::pRefdEntryBuffer(aItem)->nPoolBufferNo());
//    else
//      vTrace("%lld|%lld|"_Stz,iVal,*EntryBuffer::pRefdEntryBuffer(aItem)->nPoolBufferNo());
//
//    iOffset = aItem->iNEO(0);
//    if (iOffset > -1)
//      vTrace(","_Stz);
//  }
  vTrace("]"_Stz);
}


/*----------------------------------------------------------------------------*/
/* truncate this buffer by removing all entries                               */
/*----------------------------------------------------------------------------*/
void RowBuffer::vTruncate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  RowBuffer* pNextBuffer;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pNextBuffer = this->pNextBuffer();
  
  while (pNextBuffer) {
    RowBuffer* pThisBuffer = pNextBuffer;
    pNextBuffer = pNextBuffer->pNextBuffer();
    delete pThisBuffer;
  }
  delete this;
  
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* RowBuffer::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
