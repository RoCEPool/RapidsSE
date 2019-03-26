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
  
  // zero all the cachelines in the row tray
  for (i8 iIndex = 0 ; iIndex < C_MAX_ROWS; iIndex++)
    m_xRowTray.aGet(iIndex)->vSetAll(0);
  
  m_pNextBuffer   = nullptr;
  m_pPrevBuffer   = nullptr;
  
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
/* get the entry buffer that given id references                              */
/*----------------------------------------------------------------------------*/
RowBuffer* RowBuffer::pRefdRowBuffer(Id& xId)
{
  bn nBufferNo = xId.iBuffer();
  
  HeapBuffer* pHeapBuffer =
  reinterpret_cast<HeapBuffer*>(gpBufferPool->pToPoolBuffer(nBufferNo));
  
  RowBuffer* pRowBuffer =
  reinterpret_cast<RowBuffer*>(pHeapBuffer->aFirstChunk());
  
  ASSERT(pRowBuffer);
  ASSERT(pRowBuffer->eClass() == CN_ROWBUFFER);
  
  return pRowBuffer;
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
    
  RowBuffer* pNextBuffer = this;
  
  do {
    
    iBuffers++;
    iRows += (pNextBuffer->nRowCount());
    
    if (bDetailed)
      sAnalysis.vCatSprint("row: (%s, %lld, %5lld, %lld )\n"_Stz,
                           gpAttachMap->m_pGrid->pThisNode()->aNodeName(),
                           pNextBuffer->m_iPartitionNo,
                           iBuffers,
                           pNextBuffer->nRowCount());
    
    pNextBuffer = pNextBuffer->pNextBuffer();
    
  } while (pNextBuffer);
  
}

/*----------------------------------------------------------------------------*/
/* allot a row in this buffer                                                 */
/*----------------------------------------------------------------------------*/
CacheLine* RowBuffer::aAllotRow(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // check if this buffer has space
  
  if ( ! bHasSpace(nBytes))
    return nullptr;
  
  i8 iCacheLine = m_xRowTray.iAllotHead();
  
  return m_xRowTray.aGet(iCacheLine);
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
/* return number of row buffers under this buffer                             */
/*----------------------------------------------------------------------------*/
void RowBuffer::vBufferCount(i8& nBuffers)
{
  RowBuffer* pNextBuffer = nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // hidden index ?
  
  RASSERT(m_pIndex->bIsHidden());
  
  pNextBuffer = this->pNextBuffer();
  nBuffers++;
  
  while (pNextBuffer) {
    pNextBuffer = pNextBuffer->pNextBuffer();
    nBuffers++;
  }
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
  
  i8 nFreeCacheLines = m_xRowTray.nFree();
  i8 nFreeBytes = nFreeCacheLines * C_BYTES_PER_CACHE_LINE;
  
  if (gpAttachMap->m_nIndexEntries > 0) {
    // has space calculation based on no of entries
    if (nRowCount() >= gpAttachMap->m_nIndexEntries)
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
  
  return m_xRowTray.aGet(iRowNo);
}

/*----------------------------------------------------------------------------*/
/* get the no of rows in the buffer                                           */
/*----------------------------------------------------------------------------*/
i8 RowBuffer::nRowCount(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  return m_xRowTray.nHeads();
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
  
  // get the buffer no
  bn nBuffer = nPoolBufferNo();
  
  // restore the bufferNo
  m_iBufferNo = *nBuffer;
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
  
  ColumnVec* pColumnVec = m_pIndex->pTable()->pColumnVec();
  
  s48K sRowString;
  CacheLine* aRowCacheLine = m_xRowTray.aGet(iRowNo);
  RowEncoder::vAsString(sRowString, pColumnVec,aRowCacheLine);
  vTrace("row: (%s)\n"_Stz, sRowString.aChar());
}

/*----------------------------------------------------------------------------*/
/* show contents of buffer                                                    */
/*----------------------------------------------------------------------------*/
void RowBuffer::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  i8 nRows = nRowCount();
  
  vTrace("RowBuffer: %p (buffer %lld): rows: %lld headItems: %lld tailItems: %lld free: %lld\n"_Stz,
         this,m_iBufferNo,nRows,m_xRowTray.nHeads(),m_xRowTray.nTails(),
         m_xRowTray.nFree());
  
  vTrace("  prevBuffer %p nextBuffer %p\n"_Stz,
         m_pPrevBuffer,m_pNextBuffer);
  
  vTrace("  partition: %lld \n"_Stz, m_iPartitionNo);
  
  vTrace("--------------------------------------------------"
         "--------------------------------------------------\n"_Stz);
  
  vTrace("\n"_Stz);
  vTrace("Rows (by cacheLine):\n"_Stz);
  
  for (i8 iIndex = 0; iIndex < nRows; iIndex++)
    vShowRow(iIndex);
  
  vTrace("\n"_Stz);
  vTrace("=================================================="
         "==================================================\n"_Stz);
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
/* test for valid row buffer                                                  */
/*----------------------------------------------------------------------------*/
bool RowBuffer::bValidRowBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  if (! bValidMagic()) {
    vTrace("Not valid magic\n"_Stz);
    return false;
  }
  
  if (CN_ROWBUFFER != eClass()) {
    vTrace("Not valid class\n"_Stz);
    return false;
  }
  
  if (! aFooter()->bValidFooter()) {
    vTrace("Not valid footer\n"_Stz);
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify the structure of this row buffer                                    */
/*----------------------------------------------------------------------------*/
bool RowBuffer::bVerify(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // verify the contents of this buffer
  
  bool bValid;
  
  bValid = bVerifyNextPrev();
  if (! bValid)
    return false;
  
  bValid = bVerifyRows();
  if (! bValid)
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify has detected a fault                                                */
/*----------------------------------------------------------------------------*/
void RowBuffer::vVerifyFault(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  vTrace("VerifyFault\n"_Stz);
  RASSERT(0);
  return;
}

/*----------------------------------------------------------------------------*/
/* verify the rows of this buffer                                             */
/*----------------------------------------------------------------------------*/
bool RowBuffer::bVerifyRows(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  i8 nRows = nRowCount();
  
  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
    CacheLine* aRowCacheLine = m_xRowTray.aGet(i_Row);
    RASSERT(aRowCacheLine);
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify the next, previous pointers                                         */
/*----------------------------------------------------------------------------*/
bool RowBuffer::bVerifyNextPrev(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBUFFER,_FOOTER_);
  
  /*
   Any buffer should have a next whose prev point back to it; any prev should
   point forward to it; a child buffer should point back to its parent
   */
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (m_pPrevBuffer) {
    RowBuffer* pPrevBuffer = _pRowBuffer(m_pPrevBuffer);
    if (pPrevBuffer->pNextBuffer() != this) {
      vVerifyFault();
      return false;
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (m_pNextBuffer) {
    RowBuffer* pNextBuffer = _pRowBuffer(m_pNextBuffer);
    if (pNextBuffer->pPrevBuffer() != this) {
      vVerifyFault();
      return false;
    }
  }
  
  return true;
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
