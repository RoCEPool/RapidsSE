/*============================================================================*/
/* CrwBuffer.cpp -- Compact Row Buffer                                        */
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
CrwBuffer* Data::_pCrwBuffer(void* aAddress)
{
  CrwBuffer* pCrwBuffer = reinterpret_cast<CrwBuffer*>(aAddress);
  
  RASSERT_TYPE(pCrwBuffer,CN_CRWBUFFER);
  
  return pCrwBuffer;
}

/*----------------------------------------------------------------------------*/
/* allocate a new row buffer from heap                                        */
/*----------------------------------------------------------------------------*/
void* CrwBuffer::operator new(size_t)
{
  CrwBuffer* pCrwBuffer =
    static_cast<CrwBuffer*>(HeapMem::pAllocate(CN_CRWBUFFER));
  
  return pCrwBuffer;
}

/*----------------------------------------------------------------------------*/
/* perform new of row buffer for given address                                */
/*----------------------------------------------------------------------------*/
void* CrwBuffer::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* free a row buffer back to the heap                                         */
/*----------------------------------------------------------------------------*/
void CrwBuffer::operator delete(void* aAddress)
{
  CrwBuffer* pCrwBuffer = static_cast<CrwBuffer*>(aAddress);
  
  HeapMem::vFree(pCrwBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CrwBuffer::CrwBuffer(void)
: DataBuffer(CN_CRWBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  _checkInitialised_(CN_CRWBUFFER);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CrwBuffer::~CrwBuffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump entry buffer                                                          */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  CrwBuffer* pCrwBuffer = _pCrwBuffer(pObject);
  
  pCrwBuffer->vShow();
}

/*----------------------------------------------------------------------------*/
/* get the entry buffer that given id references                              */
/*----------------------------------------------------------------------------*/
CrwBuffer* CrwBuffer::pRefdCrwBuffer(Id& xId)
{
  bn nBufferNo = xId.iBuffer();
  
  HeapBuffer* pHeapBuffer =
  reinterpret_cast<HeapBuffer*>(gpBufferPool->pToPoolBuffer(nBufferNo));
  
  CrwBuffer* pCrwBuffer =
  reinterpret_cast<CrwBuffer*>(pHeapBuffer->aFirstChunk());
  
  ASSERT(pCrwBuffer);
  ASSERT(pCrwBuffer->eClass() == CN_CRWBUFFER);
  
  return pCrwBuffer;
}

/*----------------------------------------------------------------------------*/
/* get the row buffer that contains the given address                         */
/*----------------------------------------------------------------------------*/
CrwBuffer* CrwBuffer::pToCrwBuffer(void* aAddr)
{
  PoolBuffer* pPoolBuffer =
    PoolBuffer::pGetPoolBuffer((Object*)aAddr);
  
  HeapBuffer* pHeapBuffer =
    reinterpret_cast<HeapBuffer*>(pPoolBuffer);

  CrwBuffer* pCrwBuffer =
    reinterpret_cast<CrwBuffer*>(pHeapBuffer->aFirstChunk());
  
  RASSERT(CN_CRWBUFFER == pCrwBuffer->eClass());
  
  return pCrwBuffer;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* analyze this buffer                                                        */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vAnalyze(s48K& sAnalysis, bool bDetailed,
                           i8& iBuffers, i8& iRows)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
    
  CrwBuffer* pNextBuffer = this;
  
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
/* append a row entry into this buffer                                        */
/*----------------------------------------------------------------------------*/
bool CrwBuffer::bAppendRow(RowCompactor* pRowCoder, Row* pRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
    
#ifdef WITH_TRACE_ENCODING
  vTrace("Partition:%lld\n"_Stz, m_iPartitionNo);
#endif
  
  bool bCopied = pRowCoder->bCopyRow(pRow);
  
  return bCopied;
}

/*----------------------------------------------------------------------------*/
/* assert valid row buffer                                                    */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vAssertCrwBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* return number of row buffers under this buffer                             */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vBufferCount(i8& nBuffers)
{
  CrwBuffer* pNextBuffer = nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // table ?
  
  RASSERT(pTable());
  
  pNextBuffer = this->pNextBuffer();
  nBuffers++;
  
  while (pNextBuffer) {
    pNextBuffer = pNextBuffer->pNextBuffer();
    nBuffers++;
  }
}

/*----------------------------------------------------------------------------*/
/* get the column vector                                                      */
/*----------------------------------------------------------------------------*/
ColumnVec* CrwBuffer::pColumnVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  return pTable()->pColumnVec();
}

/*----------------------------------------------------------------------------*/
/* get decile full of the buffer                                              */
/*----------------------------------------------------------------------------*/
i8 CrwBuffer::iDecile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  i8 iDecile;
  i8 iCachLinesUsed = m_oRowTray.pNub()->nHeads()
                        + m_oRowTray.pNub()->nTails();
  i8 iSpaceUsed = iCachLinesUsed * C_BYTES_PER_CACHE_LINE;
  iDecile = IntLib::iDecile( iSpaceUsed , C_SIZE_POOL_BUFFER);
  
  return iDecile;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  // show this buffer
  vShow();
}

/*----------------------------------------------------------------------------*/
/* get the compact row from the buffer                                        */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vGetRow(Row* pRow, i8 i_Row)
{
  HeadLine* aCompactRow = m_oRowTray.pNub()->aGetHead(i_Row);
  RowInflator::vInflate(pRow,aCompactRow);
}


/*----------------------------------------------------------------------------*/
/* does buffer have space for nBytes ?                                        */
/*----------------------------------------------------------------------------*/
bool CrwBuffer::bHasSpace(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  i8 nFreeBytes = m_oRowTray.pNub()->nFreeAllotableBytes();
  
  //! is this correct now that we have separated CrwBuffer from EntBuffer?
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
/* get length of list of buffers                                               */
/*----------------------------------------------------------------------------*/
i8 CrwBuffer::iLengthList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  i8 iLength = 0;
  for (CrwBuffer* pLn = this; pLn; pLn = pLn->pNextBuffer())
    iLength++;
  
  return iLength;
}

/*----------------------------------------------------------------------------*/
/* set the table of the row buffer                                            */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vTable(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  //RASSERT(pTable);
  m_oRowTray.pNub()->vDesc(pTable);
}

/*----------------------------------------------------------------------------*/
/* get the table of the index buffer                                          */
/*----------------------------------------------------------------------------*/
Table* CrwBuffer::pTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  return m_oRowTray.pNub()->pDesc();
}

/*----------------------------------------------------------------------------*/
/* get next buffer                                                            */
/*----------------------------------------------------------------------------*/
CrwBuffer* CrwBuffer::pNextBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  return _pCrwBuffer(m_pNextBuffer);
}

/*----------------------------------------------------------------------------*/
/* set next buffer                                                            */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vNextBuffer(CrwBuffer* pNextBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  m_pNextBuffer = pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* get prev buffer                                                            */
/*----------------------------------------------------------------------------*/
CrwBuffer* CrwBuffer::pPrevBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  return _pCrwBuffer(m_pPrevBuffer);
}

/*----------------------------------------------------------------------------*/
/* set prev buffer                                                            */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vPrevBuffer(CrwBuffer* pPrevBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  m_pPrevBuffer = pPrevBuffer;
}

/*----------------------------------------------------------------------------*/
/* get row                                                                    */
/*----------------------------------------------------------------------------*/
HeadLine* CrwBuffer::pRow(i8 iRowNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  return m_oRowTray.pNub()->aGetHead(iRowNo);
}

/*----------------------------------------------------------------------------*/
/* get the no of rows in the buffer                                           */
/*----------------------------------------------------------------------------*/
i8 CrwBuffer::nRowCount(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  return m_oRowTray.pNub()->nHeads();
}

/*----------------------------------------------------------------------------*/
/* restore the buffer after being received over a socket                      */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vRestore(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  RASSERT( ! m_pNextBuffer );
  RASSERT( ! m_pPrevBuffer );
  
  // set the table to nullptr (this pointer is not valid any more)
  vTable(nullptr);
  
  // get the buffer no
  bn nBuffer = nPoolBufferNo();
  
  // restore the bufferNo
  m_iBufferNo = *nBuffer;
}

/*----------------------------------------------------------------------------*/
/* restore the table of the row buffer                                        */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vRestoreTable(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  //RASSERT( ! m_pTable );
  vTable(pTable);
}

/*----------------------------------------------------------------------------*/
/* show entry                                                                 */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vShowRow(i8 i_Row)
{
  vTrace("Id: %3lli.%03lli "_Stz,m_iBufferNo,i_Row);
  
  ColumnVec* pColVec = pTable()->pColumnVec();
  HV_Rec hvRec(pColVec);
  
  // create a row
  HV_Row hRow(pColVec);
  
  // get compact row
  HeadLine* aCompactRow = m_oRowTray.pNub()->aGetHead(i_Row);

  // inflate compact row into row
  RowInflator::vInflate(hRow,aCompactRow);
  
  // append the row to the record
  hvRec->vAppend(hRow);
  
  vTrace("row: (%s)\n"_Stz,hvRec->szRecStr());
}

/*----------------------------------------------------------------------------*/
/* show contents of buffer                                                    */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  i8 nRows = nRowCount();
  
  vTrace("CrwBuffer: %p (buffer %lld): rows: %lld headItems: %lld tailItems: %lld free: %lldB\n"_Stz,
         this,m_iBufferNo,nRows,
         m_oRowTray.pNub()->nHeads(),
         m_oRowTray.pNub()->nTails(),
         m_oRowTray.pNub()->nFreeAllotableBytes());
  
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
void CrwBuffer::vTruncate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  CrwBuffer* pNextBuffer;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pNextBuffer = this->pNextBuffer();
  
  while (pNextBuffer) {
    CrwBuffer* pThisBuffer = pNextBuffer;
    pNextBuffer = pNextBuffer->pNextBuffer();
    delete pThisBuffer;
  }
  delete this;
  
}

/*----------------------------------------------------------------------------*/
/* test for valid row buffer                                                  */
/*----------------------------------------------------------------------------*/
bool CrwBuffer::bValidCrwBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  if (! bValidMagic()) {
    vTrace("Not valid magic\n"_Stz);
    return false;
  }
  
  if (CN_CRWBUFFER != eClass()) {
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
bool CrwBuffer::bVerify(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
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
void CrwBuffer::vVerifyFault(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  vTrace("VerifyFault\n"_Stz);
  RASSERT(0);
  return;
}

/*----------------------------------------------------------------------------*/
/* verify the rows of this buffer                                             */
/*----------------------------------------------------------------------------*/
bool CrwBuffer::bVerifyRows(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  i8 nRows = nRowCount();
  
  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
    HeadLine* aRowCacheLine = m_oRowTray.pNub()->aGetHead(i_Row);
    RASSERT(aRowCacheLine);
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* verify the next, previous pointers                                         */
/*----------------------------------------------------------------------------*/
bool CrwBuffer::bVerifyNextPrev(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  /*
   Any buffer should have a next whose prev point back to it; any prev should
   point forward to it; a child buffer should point back to its parent
   */
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (m_pPrevBuffer) {
    CrwBuffer* pPrevBuffer = _pCrwBuffer(m_pPrevBuffer);
    if (pPrevBuffer->pNextBuffer() != this) {
      vVerifyFault();
      return false;
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (m_pNextBuffer) {
    CrwBuffer* pNextBuffer = _pCrwBuffer(m_pNextBuffer);
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
Footer* CrwBuffer::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
