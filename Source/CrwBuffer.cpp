/*============================================================================*/
/* CrwBuffer.cpp -- Compact Row Buffer                                        */
/*============================================================================*/

#include "CompactBuffers.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(CompactBuffers,CrwBuffer,CN_CRWBUFFER)
__aNewFunction(CrwBuffer,CN_CRWBUFFER)
__vDeleteFunction(CrwBuffer,CN_CRWBUFFER)
__aFooterFunction(CrwBuffer)

/*----------------------------------------------------------------------------*/
/* perform new of row buffer for given address                                */
/*----------------------------------------------------------------------------*/
void* CrwBuffer::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CrwBuffer::CrwBuffer(void)
: DataBuffer(CN_CRWBUFFER, false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  ASSERT(CacheLine::bOnCacheLineBoundary(this));

  m_pRowCompactor = nullptr;
  
  m_pTable = nullptr;
  m_pStreamDef = nullptr;
  
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
/* delete a crw                                                               */
/*----------------------------------------------------------------------------*/
bool CrwBuffer::bDeleteRefdCrw(Ref xRef)
{
  // get the referenced row buffer
  CrwBuffer* pCrwBuffer = CrwBuffer::pRefdCrwBuffer(xRef);
  if (! pCrwBuffer)
    return false;
  
  // get the referenced row
  Crw* aCrw = pCrwBuffer->aCrw(xRef.iItem());
  
  pCrwBuffer->m_oRowTray.pNub()->vDelete(aCrw);
  
  if (pCrwBuffer->nRowCount() == 0) {
    return true;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* dump entry buffer                                                          */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  _static_
  
  CrwBuffer* pCrwBuffer = _pCrwBuffer(pObject);
  
  pCrwBuffer->vShow();
}

/*----------------------------------------------------------------------------*/
/* get crw given ref                                                          */
/*----------------------------------------------------------------------------*/
Crw* CrwBuffer::pRefdCrw(Ref xRef)
{
  _static_
  
  // get the referenced row buffer
  CrwBuffer* pCrwBuffer = CrwBuffer::pRefdCrwBuffer(xRef);
  if (! pCrwBuffer)
    return nullptr;
  
  // get the referenced row
  Crw* aCrw = pCrwBuffer->aCrw(xRef.iItem());
  
  return aCrw;
}

/*----------------------------------------------------------------------------*/
/* get the entry buffer that given id references                              */
/*----------------------------------------------------------------------------*/
CrwBuffer* CrwBuffer::pRefdCrwBuffer(Ref& xRef)
{
  _static_
  
  i8 i_Buffer = xRef.iBuffer();
  
  HeapBuffer* pHeapBuffer =
  reinterpret_cast<HeapBuffer*>(gpBufferPool->pToPoolBuffer(i_Buffer));
  
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
  _static_
  
  PoolBuffer* pPoolBuffer =
    PoolBuffer::pWithinPoolBuffer((Object*)aAddr);
  
  HeapBuffer* pHeapBuffer =
    reinterpret_cast<HeapBuffer*>(pPoolBuffer);

  CrwBuffer* pCrwBuffer =
    reinterpret_cast<CrwBuffer*>(pHeapBuffer->aFirstChunk());
  
  RASSERT(CN_CRWBUFFER == pCrwBuffer->eClass());
  
  return pCrwBuffer;
}

/*----------------------------------------------------------------------------*/
/* convert/inflate compact row into row                                       */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vToRow(RowSingle* pRow, Crw* aCrw)
{
  _static_
  
  RowInflator::vInflateRow(pRow,aCrw);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* analyze this buffer                                                        */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vAnalyze(i8& nBuffers, i8& nRows)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
    
  CrwBuffer* pNextBuffer = this;
  
  do {
    
    nBuffers++;
    nRows += (pNextBuffer->nRowCount());
    
//    if (bDetailed)
//      sAnalysis.vCatSprint("row: (%s, %lld, %5lld, %lld )\n"_Stz,
//                           AM::szNodeNm(),
//                           pNextBuffer->m_iPartitionNo,
//                           iBuffers,
//                           pNextBuffer->nRowCount());
    
    pNextBuffer = pNextBuffer->pNextCrwBuffer();
    
  } while (pNextBuffer);
  
}

/*----------------------------------------------------------------------------*/
/* close this buffer for append                                               */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vAppendClose(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  if (m_pRowCompactor)
    m_pRowCompactor->vCompactEnd();

  m_pRowCompactor = nullptr;
}

/*----------------------------------------------------------------------------*/
/* append given crw into this buffer                                          */
/*----------------------------------------------------------------------------*/
bool CrwBuffer::bAppendCrw(Crw* aCrwSrc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  StreamDef* pStreamDef = this->pStreamDef();
  i8 jCrwSize = pStreamDef->m_nFixedSized;

  RowNub* pRowNub = m_oRowTray.pNub();
  i8 i_Head = pRowNub->iAllotHead(jCrwSize);
  if (C_UNKNOWN == i_Head)
    return false;
  
  Crw* aCrwDst = _aCrw(pRowNub->aGetHead(i_Head));
  
  MemLib::vCopy(aCrwDst,aCrwSrc,jCrwSize,CN_CRWBUFFER);

  return true;
}

/*----------------------------------------------------------------------------*/
/* open this buffer for append                                                */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vAppendOpen(RowCompactor* pRowCompactor, bool bCollectSamples)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
 
  ASSERT(! m_pRowCompactor);
  
  m_pRowCompactor = pRowCompactor;
  
  m_pRowCompactor->vCompactBegin(this, bCollectSamples);
}

/*----------------------------------------------------------------------------*/
/* append an rsb into this crw buffer                                         */
/*----------------------------------------------------------------------------*/
bool CrwBuffer::bAppendRsb(Rsb* aRsbSrc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  bool bAppended = m_pRowCompactor->bCompactRsb(aRsbSrc);
  
  return bAppended;
}

/*----------------------------------------------------------------------------*/
/* append a row entry into this buffer                                        */
/*----------------------------------------------------------------------------*/
bool CrwBuffer::bAppendSrw(RowSingle* pSrwSrc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
    
#ifdef WITH_ENCODING_TRACE
  vTrace("Partition:%lld\n"_Stz, m_iPartitionNo);
#endif
  
  bool bAppended = m_pRowCompactor->bCompactSrw(pSrwSrc);
  
  return bAppended;
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  CrwBuffer* pNextBuffer = nullptr;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // table ?
  
  RASSERT(pTable());
  
  pNextBuffer = this->pNextCrwBuffer();
  nBuffers++;
  
  while (pNextBuffer) {
    pNextBuffer = pNextBuffer->pNextCrwBuffer();
    nBuffers++;
  }
}

/*----------------------------------------------------------------------------*/
/* copy given argument into this crw buffer                                   */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vCopy(CrwBuffer* pArgCrwBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  m_pTable     = pArgCrwBuffer->m_pTable;
  m_pStreamDef = pArgCrwBuffer->m_pStreamDef;
  
  MemLib::vCopy(&m_oRowTray,
                &pArgCrwBuffer->m_oRowTray,
                sizeof(RowTray),
                CN_CRWBUFFER);
  
  //vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* get compact row                                                            */
/*----------------------------------------------------------------------------*/
Crw* CrwBuffer::aCrw(i8 i_Row)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  return (Crw*)aGetHead(i_Row);
}

/*----------------------------------------------------------------------------*/
/* get decile full of the buffer                                              */
/*----------------------------------------------------------------------------*/
i8 CrwBuffer::iDecile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  i8 iDecile;
  i8 iCachLinesUsed = m_oRowTray.pNub()->nHeads() - m_oRowTray.pNub()->nHeadsDeleted()
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
/* get the head line for given row                                            */
/*----------------------------------------------------------------------------*/
HeadLine* CrwBuffer::aGetHead(i8 i_Row)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  HeadLine* aHeadLine = m_oRowTray.pNub()->aGetHead(i_Row);
  
  return aHeadLine;
}

/*----------------------------------------------------------------------------*/
/* get the compact row from the buffer                                        */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vGetRow(RowSingle* pRow, i8 i_Row)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  Crw* aCrw = (Crw*)aGetHead(i_Row);
  
  RowInflator::vInflateRow(pRow,aCrw);
}

/*----------------------------------------------------------------------------*/
/* get the compact row from the buffer                                        */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vGetRow(RowSlab* pRowSlab, i8 i_Row)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);

  Crw* aCrw = (Crw*)aGetHead(i_Row);

  RowInflator::vInflateRow(pRowSlab,aCrw);
}

/*----------------------------------------------------------------------------*/
/* does buffer have space for nBytes ?                                        */
/*----------------------------------------------------------------------------*/
bool CrwBuffer::bHasSpace(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  i8 nFreeBytes = m_oRowTray.pNub()->nFreeAllotableBytes();
  
  //! is this correct now that we have separated CrwBuffer from EntBuffer?
  if (AM::nIndexEntries() > 0) {
    // has space calculation based on no of entries
    if (nRowCount() >= AM::nIndexEntries())
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
  for (CrwBuffer* pLn = this; pLn; pLn = pLn->pNextCrwBuffer())
    iLength++;
  
  return iLength;
}

/*----------------------------------------------------------------------------*/
/* get next buffer                                                            */
/*----------------------------------------------------------------------------*/
CrwBuffer* CrwBuffer::pNextCrwBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  return _pCrwBuffer(m_pNextBuffer);
}

/*----------------------------------------------------------------------------*/
/* set next buffer                                                            */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vNextCrwBuffer(CrwBuffer* pNextBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  m_pNextBuffer = pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* operand dump of this crw buffer                                            */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vOperandDump(i8 iDetail)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  vTrace("CrwBuffer: rows: %lld "
         "headItems: %lld tailItems: %lld strItems: %lld "
         "free: %lld(B)\n"_Stz,

         nRowCount(),
         
         m_oRowTray.pNub()->nHeads(),
         m_oRowTray.pNub()->nTails(),
         m_oRowTray.pNub()->nStrs(),
         
         m_oRowTray.pNub()->nFreeAllotableBytes());
  
  
  if (iDetail >= 1)
    m_oRowTray.pNub()->vDumpStrings();

  if (iDetail >= 2)
    vShow();
}

/*----------------------------------------------------------------------------*/
/* get prev buffer                                                            */
/*----------------------------------------------------------------------------*/
CrwBuffer* CrwBuffer::pPrevCrwBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  return _pCrwBuffer(m_pPrevBuffer);
}

/*----------------------------------------------------------------------------*/
/* set prev buffer                                                            */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vPrevCrwBuffer(CrwBuffer* pPrevBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  m_pPrevBuffer = pPrevBuffer;
}

/*----------------------------------------------------------------------------*/
/* print rows of the buffer                                                   */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vPrintRows(StreamDef* pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  HV_Row hRow(pStreamDef,C_ALL);
  HV_Rec hvRec(pStreamDef,C_ALL);
  
  //m_oRowTray.pNub()->vDumpTails();
  
  i8 nRows = m_oRowTray.pNub()->nHeads();
  
  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
    
    // get the compact row
    Crw* aCrw = (Crw*)aGetHead(i_Row);
    
    // ignore deleted rows
    if (aCrw->bIsDeleted())
      continue;
    
    // inflate compact row into row
    RowInflator::vInflateRow(hRow,aCrw);
    
    hvRec->vAppend(hRow);
    hvRec->vPrintRow();
  }
  
}

/*----------------------------------------------------------------------------*/
/* get the no of rows in the buffer                                           */
/*----------------------------------------------------------------------------*/
i8 CrwBuffer::nRowCount(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  return m_oRowTray.pNub()->nHeads() - m_oRowTray.pNub()->nHeadsDeleted();
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
  i8 i_Buffer = iBufferNo();
  
  // restore the bufferNo
  m_iBufferNo = i_Buffer;
}

/*----------------------------------------------------------------------------*/
/* restore the nub the row buffer                                             */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vRestoreNub(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  m_oRowTray.vRestoreNub();
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  vTrace("Ref: %3lli.%03lli"_Stz,m_iBufferNo,i_Row);
  
  StreamDef* pStreamDef = pTable()->pStreamDef();
  HV_Rec hvRec(pStreamDef,C_ALL);
  
  // create a row
  HV_Row hRow(pStreamDef,C_ALL);
  
  // get compact row
  Crw* aCompactRow = (Crw*)aGetHead(i_Row);
  
  // inflate compact row into row
  RowInflator::vInflateRow(hRow,aCompactRow);
  
  // append the row to the record
  hvRec->vAppend(hRow);
  
  i8 nOffset = nDifference(this,aCompactRow);
  
  vTrace("row: %04x (%s)\n"_Stz,nOffset,hvRec->szRecStr());
  
  if (0 == i_Row)
    Crw::vDumpCrw("  "_Stz,aCompactRow,pStreamDef);

}

/*----------------------------------------------------------------------------*/
/* show contents of buffer                                                    */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  i8 nRows = nRowCount();
  
  vTrace("CrwBuffer: %p (buffer %lld %s): rows: %lld headItems: %lld tailItems: %lld free: %lldB\n"_Stz,
         this,m_iBufferNo,
         m_bIsPermanent ? "P"_Stz : "T"_Stz,
         nRows,
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
/* get the stream def                                                         */
/*----------------------------------------------------------------------------*/
StreamDef* CrwBuffer::pStreamDef(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  return m_oRowTray.pNub()->pDesc();
}

/*----------------------------------------------------------------------------*/
/* set the table of the row buffer                                            */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vStreamDef(StreamDef* pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  //RASSERT(pTable);
  m_pStreamDef = pStreamDef;
  m_oRowTray.pNub()->vDesc(pStreamDef);
}

/*----------------------------------------------------------------------------*/
/* set the table of the row buffer                                            */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vTable(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  //RASSERT(pTable);
  m_pTable = pTable;
  m_oRowTray.pNub()->vDesc(pTable->pStreamDef());
}

/*----------------------------------------------------------------------------*/
/* get the table of the index buffer                                          */
/*----------------------------------------------------------------------------*/
Table* CrwBuffer::pTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);

  return m_pTable;
}

/*----------------------------------------------------------------------------*/
/* generate a CsvBuffer from a crw buffer with a csv row                      */
/*----------------------------------------------------------------------------*/
CsvBuffer* CrwBuffer::pToCsvBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  StreamDef* pStreamDef = this->pStreamDef();
  
  CsvBuffer* pFirstCsvBuffer = new CsvBuffer;
  CsvBuffer* pCsvBuffer = pFirstCsvBuffer;
  
  HV_Rec hvRec(pStreamDef,C_ALL);
  HV_Row hRow(pStreamDef,C_ALL);
  
  i8 nRows = nRowCount();
  i8 nBytes = 1;
  Stz* szRecord = nullptr;
  
  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
    
    // get the inflated compact row
    vGetRow(hRow, i_Row);
    
    // append the row to the record
    hvRec->vAppend(hRow);
    
    StrLib::vConcat(hvRec->szRecStr()->azChar(),'\n');           //! sub-optimal
    
    nBytes = hvRec->szRecStr()->nSize();
    
    if (pCsvBuffer->nFreeBytes() < nBytes + 1) {
      CsvBuffer* pNewCsvBuffer = new CsvBuffer;
      pCsvBuffer->vNextBuffer(pNewCsvBuffer);
      pCsvBuffer = pNewCsvBuffer;
    }
    
    szRecord = pCsvBuffer->szAllotRecord(nBytes - 1);
    StrLib::vCopyLen(szRecord->azChar(),hvRec->szRecStr()->azChar(),nBytes - 1);
  }
  
  if (szRecord)
    ((char*)szRecord)[nBytes-1] = '\0';
  
  return pFirstCsvBuffer;
}

/*----------------------------------------------------------------------------*/
/* generate a SrtBuffer from entry buffer                                     */
/*----------------------------------------------------------------------------*/
//SrtBuffer* CrwBuffer::pToSrtBuffer(i8 iKeyCol)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
//  
//  i8 nRowRefs = 1;
//  
//  HV_Row hRow(pTable()->pStreamDef(),C_ALL);
//  SrtBuffer* pSrtBuffer = new SrtBuffer(nRowRefs, m_iPartitionNo);
//  
//  i8 nRows = nRowCount();
//  
//  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
//    
//    SrtSet* pSrtSet = pSrtBuffer->aAllotSrtSet();
//    
//    if ( ! pSrtSet ) {
//      
//      // create a new SrtBuffer
//      SrtBuffer* pNewSrtBuffer = new SrtBuffer(nRowRefs, m_iPartitionNo);
//      pNewSrtBuffer->vNextBuffer(pSrtBuffer);
//      pSrtBuffer = pNewSrtBuffer;
//      pSrtSet = pSrtBuffer->aAllotSrtSet();
//      
//    }
//    RASSERT(pSrtSet);
//    
//    // get compact row
//    Crw* aCrw = (Crw*)aGetHead(i_Row);
//    
//    // set the rowData in the srtSet
//    pSrtSet->vRowRef(0, (i8)aCrw);
//    
//    // get the key from the row
//    RowInflator::vInflateColumn(hRow, aCrw, iKeyCol);
//    
//    // set the key in the srtSet
//    pSrtSet->vKey(hRow->m_waColVal[iKeyCol].m_iWrd);
//  }
//  
//  return pSrtBuffer;
//}

/*----------------------------------------------------------------------------*/
/* truncate this buffer by removing all entries                               */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vTruncate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  CrwBuffer* pNextBuffer;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pNextBuffer = this->pNextCrwBuffer();
  
  while (pNextBuffer) {
    CrwBuffer* pThisBuffer = pNextBuffer;
    pNextBuffer = pNextBuffer->pNextCrwBuffer();
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
    Crw* aRowCacheLine = (Crw*)aGetHead(i_Row);
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
