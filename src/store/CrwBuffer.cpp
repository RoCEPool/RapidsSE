/*============================================================================*/
/* CrwBuffer.cpp -- Compact Row Buffer                                        */
/*============================================================================*/

#include "BASE.h"

#include "IndexDef.hpp"

#include "RowCoder.hpp"
#include "HeadLine.hpp"
#include "TailLine.hpp"

#include "Crw.hpp"
#include "CrwPacker.hpp"
#include "CrwBuffer.hpp"
#include "RowCompactor.hpp"
#include "RowInflator.hpp"

#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "CrvBuffer.hpp"
#include "TablePtn.hpp"
#include "RowOnce.hpp"
#include "IndexStream.hpp"
#include "Table.hpp"
#include "Pot.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,CrwBuffer,CN_CRWBUFFER)
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
  
  _checkInitialised_(CN_CRWBUFFER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  //vTrace("CrwBuffer\n"_Stz);
  
  AM::METRICS::vDerefMetrics((i8)this,CN_CRWBUFFER,DR_NEW);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CrwBuffer::~CrwBuffer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  AM::METRICS::vDerefMetrics((i8)this,CN_CRWBUFFER,DR_DELETE);
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* delete a crw                                                               */
/*----------------------------------------------------------------------------*/
bool CrwBuffer::bDeleteRefdCrw(Rrf xRef)
{
  // get the referenced row buffer
  CrwBuffer* pCrwBuffer = CrwBuffer::pRefdCrwBuffer(xRef);
  if (! pCrwBuffer)
    return false;
  
  // get the referenced row
  Crw* aCrw = pCrwBuffer->aCrw(xRef.iItem());
  
  pCrwBuffer->m_oRowTray.pNub()->vDelete(aCrw);
  
  if (pCrwBuffer->nRows() == 0) {
    return true;
  }
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* dump crw buffer                                                            */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  _static_
  UNUSED(eClass);
  UNUSED(aTab);

  CrwBuffer* pCrwBuffer = _pCrwBuffer(pObject);
  
  pCrwBuffer->vShow();
}

/*----------------------------------------------------------------------------*/
/* get crw given ref                                                          */
/*----------------------------------------------------------------------------*/
Crw* CrwBuffer::pRefdCrw(Rrf xRef)
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
/* get the crw buffer that given id references                                */
/*----------------------------------------------------------------------------*/
CrwBuffer* CrwBuffer::pRefdCrwBuffer(Rrf& xRef)
{
  _static_
  
  i8 i_PoolBuffer = xRef.iBuffer();
  
  HeapBuffer* pHeapBuffer =
  reinterpret_cast<HeapBuffer*>(FileMap::pToPoolBuffer(i_PoolBuffer));
  
  CrwBuffer* pCrwBuffer =
  reinterpret_cast<CrwBuffer*>(pHeapBuffer->aFirstChunk());
  
  ASSERT(pCrwBuffer);
  ASSERT(pCrwBuffer->eGetClassNo() == CN_CRWBUFFER);
  
  return pCrwBuffer;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

void CrwBuffer::vRepair(Object* pObject)
{
  _static_
  CrwBuffer* pCrwBuffer = _pCrwBuffer(pObject);

  AM::vDeleteOperand(pCrwBuffer,nullptr);
}

/*----------------------------------------------------------------------------*/
/* get the crw buffer that contains the given address                         */
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
  
  RASSERT(CN_CRWBUFFER == pCrwBuffer->eGetClassNo());
  
  return pCrwBuffer;
}

/*----------------------------------------------------------------------------*/
/* convert/inflate compact row into row                                       */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vToRow(RowSingle* pRowSingle, Crw* aCrw)
{
  _static_
  
  RowInflator::vInflate_AllColumns_into_RowSingle(pRowSingle,aCrw);
}



/*----------------------------------------------------------------------------*/
/* post recv function                                                         */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vPostRecv(void* aCrwBuffer)
{
  _static_
  
  CrwBuffer* pCrwBuffer = _pCrwBuffer(aCrwBuffer);
  
  pCrwBuffer->vIsPermanent(false);
  pCrwBuffer->vRestoreNub();
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set the table of the crw buffer                                            */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vStreamDef(StreamDef* pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  DataBuffer::vStreamDef_dbuff(pStreamDef);
  m_oRowTray.pNub()->vDesc(pStreamDef);
}

/*----------------------------------------------------------------------------*/
/* set the table of the crw buffer                                            */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vTable(Table* pTable)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  DataBuffer::vTable_dbuff(pTable);
  vStreamDef(pTable == nullptr ? nullptr : pTable->pStreamDef());
}

/*----------------------------------------------------------------------------*/
/* get row compactor                                                          */
/*----------------------------------------------------------------------------*/
RowCompactor* CrwBuffer::pRowCompactor(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  return m_pRowCompactor;
}

/*----------------------------------------------------------------------------*/
/* get row tray                                                               */
/*----------------------------------------------------------------------------*/
RowTray* CrwBuffer::pRowTray(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  return & m_oRowTray;
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
  
  StreamDef* pStreamDef = this->pStreamDef_dbuff();
  i8 jCrwSize = pStreamDef->jRowSizeBytes();

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
/* append a row into this crw buffer                                          */
/*----------------------------------------------------------------------------*/
bool CrwBuffer::bAppendRow(Row* aRowSrc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  bool bAppended = m_pRowCompactor->bCompactRow(aRowSrc);
  
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
/* copy given argument into this crw buffer                                   */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vCopy(CrwBuffer* pArgCrwBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  m_pTable_dbuff     = pArgCrwBuffer->m_pTable_dbuff;
  m_pStreamDef_dbuff = pArgCrwBuffer->m_pStreamDef_dbuff;
  
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
  
//  i8 iCachLinesUsed = m_oRowTray.pNub()->nHeads() - m_oRowTray.pNub()->nHeadsDeleted()
//                        + m_oRowTray.pNub()->nTails();
//  i8 iSpaceUsed = iCachLinesUsed * C_BYTES_PER_CACHE_LINE;
  
  i8 iSpaceUsed = m_oRowTray.pNub()->nAllotedBytes();
  i8 iDecile = IntLib::iDecile( iSpaceUsed , C_SIZE_POOL_BUFFER);
  
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
void CrwBuffer::vGetRow_single(RowSingle* pRowSingle, i8 i_Row)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  Crw* aCrw = (Crw*)aGetHead(i_Row);
  
  RowInflator::vInflate_AllColumns_into_RowSingle(pRowSingle,aCrw);
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
    if (nRows() >= AM::nIndexEntries())
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
/* inflate rows of the buffer                                                 */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vInflateRows(StreamDef* pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);

  HV_RowSingle hvRowSingle(pStreamDef);

  //m_oRowTray.pNub()->vDumpTails();

  i8 nCrws = m_oRowTray.pNub()->nHeads();

  for (i8 i_Crw = 0; i_Crw < nCrws; i_Crw++) {

    // get the compact row
    Crw* aCrw = (Crw*)aGetHead(i_Crw);

    // ignore deleted rows
    if (aCrw->bIsDeleted())
      continue;

    // inflate compact row into row
    RowInflator::vInflate_AllColumns_into_RowSingle(hvRowSingle,aCrw);

  }

}

/*----------------------------------------------------------------------------*/
/* inflate rows of the buffer                                                 */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vInflateRows(Tub* pTub)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  UNUSED(pTub);

//
//    #include "vFLAT_Structures.h"
//    #include "vFLAT_PreliminaryCrwOld.h"
//    #include "vFLAT_DrivingLoopCrw2.h"
//    #include "vFLAT_Filter.h"
//    #include "vFLAT_InflateColumn.h"
//    #include "vFLAT_SkipColumn.h"

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

         nRows(),
         
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
/* print rows of the buffer                                                   */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vPrintRows(StreamDef* pStreamDef)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  HV_RowSingle hvRowSingle(pStreamDef);
  
  //m_oRowTray.pNub()->vDumpTails();
  
  i8 nCrws = m_oRowTray.pNub()->nHeads();
  
  for (i8 i_Crw = 0; i_Crw < nCrws; i_Crw++) {
    
    // get the compact row
    Crw* aCrw = (Crw*)aGetHead(i_Crw);
    
    // ignore deleted rows
    if (aCrw->bIsDeleted())
      continue;
    
    // inflate compact row into row
    RowInflator::vInflate_AllColumns_into_RowSingle(hvRowSingle,aCrw);
    
    hvRowSingle->vPrintRow();
  }
  
}

/*----------------------------------------------------------------------------*/
/* get the no of rows in the buffer                                           */
/*----------------------------------------------------------------------------*/
i8 CrwBuffer::nRows(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  return m_oRowTray.pNub()->nHeads() - m_oRowTray.pNub()->nHeadsDeleted();
}

/*----------------------------------------------------------------------------*/
/* get the row size of the buffer                                             */
/*----------------------------------------------------------------------------*/
i8 CrwBuffer::nRowSize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);

  return m_oRowTray.pNub()->nHeadBytes();
}

/*----------------------------------------------------------------------------*/
/* get estimate for size of actual row                                        */
/*----------------------------------------------------------------------------*/
i8 CrwBuffer::jRowSizeEst(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);

  i8 nHeads = m_oRowTray.pNub()->nHeads();

  i8 jSpaceUsed = m_oRowTray.pNub()->nAllotedBytes();

  i8 jRowSize = jSpaceUsed / nHeads;

  return jRowSize;
}

/*----------------------------------------------------------------------------*/
/* get the row size of the buffer                                             */
/*----------------------------------------------------------------------------*/
i8 CrwBuffer::nAllotedBytes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);

  return m_oRowTray.pNub()->nAllotedBytes();
}


/*----------------------------------------------------------------------------*/
/* get the row size of the buffer                                             */
/*----------------------------------------------------------------------------*/
i8 CrwBuffer::nHeadSize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);

  return m_oRowTray.pNub()->nHeadSize();
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
/* show entry                                                                 */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vShowRow(i8 i_Row)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  vTrace("Rrf: %3lli.%03lli "_Stz,m_iBufferNo,i_Row);
  
  StreamDef* pStreamDef = pTable_dbuff()->pStreamDef();
  HV_Record hvRecord(pStreamDef);
  
  // create a row
  HV_RowSingle hvRowSingle(pStreamDef);
  
  // get compact row
  Crw* aCompactRow = (Crw*)aGetHead(i_Row);
  
  // inflate compact row into row
  RowInflator::vInflate_AllColumns_into_RowSingle(hvRowSingle,aCompactRow);
  
  // append the row to the record
  hvRecord->vToCsvRecord(hvRowSingle->pRowCurrent());
  
  i8 nOffset = nDifference(this,aCompactRow);
  
  vTrace("row: %04x (%s)\n"_Stz,nOffset,hvRecord->sz());
  
  if (0 == i_Row)
    Crw::vDumpCrw("  "_Stz,aCompactRow,pStreamDef);

}

/*----------------------------------------------------------------------------*/
/* show contents of buffer                                                    */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vShow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  i8 nRows = this->nRows();
  
  vTrace("CrwBuffer: %p (buffer %lld %s): rows: %lld headItems: %lld tailItems: %lld free: %lldB\n"_Stz,
         this,m_iBufferNo,
         m_bIsPermanent ? "P"_Stz : "T"_Stz,
         nRows,
         m_oRowTray.pNub()->nHeads(),
         m_oRowTray.pNub()->nTails(),
         m_oRowTray.pNub()->nFreeAllotableBytes());

  vTrace("  partition: %lld \n"_Stz, m_tLglPtn.iAsI8());
  
  vTrace("--------------------------------------------------"
         "--------------------------------------------------\n"_Stz);
  
  vTrace("\n"_Stz);
  vTrace("Rows (by cacheLine):\n"_Stz);
  
  for (i8 i_Row = 0; i_Row < nRows; i_Row++)
    vShowRow(i_Row);
  
  vTrace("\n"_Stz);
  vTrace("=================================================="
         "==================================================\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* convert this crw buffer into one or more csv tub buffers                   */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vToCsv(Tub* pTub)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  StreamDef* pStreamDef = this->pStreamDef_dbuff();
  
  HV_Record hvRecord(pStreamDef);
  HV_RowSingle hvRowSingle(pStreamDef);
  
  CsvBuffer* pCsvBuffer = _pCsvBuffer(pTub->pWorkingBuffer());
  if (! pCsvBuffer) {
    pCsvBuffer = new CsvBuffer;
    pTub->vAddBuffer(pCsvBuffer);
  }
  
  i8 nSize = 0;
  Stz* szRecord = nullptr;
  
  i8 nRows = this->nRows();
  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
    
    vGetRow_single(hvRowSingle, i_Row);
    
    hvRecord->vToCsvRecord(hvRowSingle->pRowCurrent());
    StrLib::vConcat(hvRecord->az(), hvRecord->iStrBufSize(), '\n');           //! sub-optimal
    nSize = hvRecord->sz()->nSize();
    
    if (pCsvBuffer->nFreeBytes() < nSize + 1) {
      pCsvBuffer = new CsvBuffer;
      pTub->vAddBuffer(pCsvBuffer);
    }
    
    szRecord = pCsvBuffer->szAllotRecord(nSize - 1);
    StrLib::vCopy(szRecord->azChar(), nSize, hvRecord->sz()->azChar());
  }
  
  if (szRecord)
    ((char*)szRecord)[nSize-1] = '\0';
}

/*----------------------------------------------------------------------------*/
/* convert this crw buffer into one or more rowRef tub buffers with given key */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vToRowRef(StreamDef* pStreamDefOut, Tub* pTub, i8 iKeyCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);

  Rrf xRowRef;
  xRowRef.vBuffer(iBufferNo());

  StreamDef* pStreamDef = this->pStreamDef_dbuff();

  HV_RowSingle hvRowSingle(pStreamDef);

  Pds* pPdsBuffer = _pPds(pTub->pWorkingBuffer());
  if (! pPdsBuffer) {
    pPdsBuffer = new Pds(pStreamDefOut);
    pTub->vAddBuffer(pPdsBuffer);
  }

  i8 nRows = this->nRows();
  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {

    xRowRef.vItem(i_Row);
    vGetRow_single(hvRowSingle, i_Row);

    //Column* pColumn = pStreamDefLft->pGetColumn(iKeyCol);

    i8 iKeyValue = hvRowSingle->iColVal(iKeyCol);

    if (pPdsBuffer->nFreeBytes() < 24) {
      pPdsBuffer = new Pds(pStreamDefOut);
      pTub->vAddBuffer(pPdsBuffer);
    }

    Row* pRow = pPdsBuffer->pAllotRow(pStreamDefOut,
                                    C_VARGS_BEGIN,
                                    iKeyValue,
                                    m_tLglPtn.iAsI8(),
                                    xRowRef.iAsI8(),
                                    C_VARGS_END);
    ASSERT(pRow);
    UNUSED(pRow);
  }

}

/*----------------------------------------------------------------------------*/
/* test for valid row buffer                                                  */
/*----------------------------------------------------------------------------*/
bool CrwBuffer::bValidCrwBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  if (! bIsMagicValid()) {
    vTrace("Not valid magic\n"_Stz);
    return false;
  }
  
  if (CN_CRWBUFFER != eGetClassNo()) {
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
/* verify the rows of this buffer                                             */
/*----------------------------------------------------------------------------*/
bool CrwBuffer::bVerifyRows(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);

  i8 nRows = this->nRows();

  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
    Crw* aRowCacheLine = (Crw*)aGetHead(i_Row);
    RASSERT(aRowCacheLine);
  }

  return true;
}

/*----------------------------------------------------------------------------*/
/* verify has detected a fault                                                */
/*----------------------------------------------------------------------------*/
void CrwBuffer::vVerifyFault(const char* azFunc, const char* azFilePath, i8 iLineNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CRWBUFFER,_FOOTER_);
  
  vTrace("CrwBuffer::VerifyFault: %s %s lld\n"_Stz,azFunc,azFilePath,iLineNo);
  RASSERT(0);
  return;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
