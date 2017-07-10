/*============================================================================*/
/* EntryBuffer.cpp -- Entry Buffer                                            */
/*============================================================================*/

#include "DBobjects.h"

#include "StorageEngine.h"

extern bool gbIndexSpace;
extern i8   gnIndexEntries;
extern i8   gnIndexLevels;
extern i8   gnIndexOffsets;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
EntryBuffer* DBobjects::_pEntryBuffer(void* aAddr)
{
  EntryBuffer* pEntryBuffer = reinterpret_cast<EntryBuffer*>(aAddr);
  
  RASSERT( !pEntryBuffer || CN_ENTRYBUFFER == pEntryBuffer->eClass());
  
  return pEntryBuffer;
}

/*----------------------------------------------------------------------------*/
/* fetch a new buffer from buffer pool                                        */
/*----------------------------------------------------------------------------*/
void* EntryBuffer::operator new(size_t iSize)
{
  RASSERT(iSize);
  Buffer* pBuffer = (Buffer*)Heap::pAllocate(CN_ENTRYBUFFER);
  
  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* fetch a new buffer from buffer pool                                        */
/*----------------------------------------------------------------------------*/
void* EntryBuffer::operator new(size_t iSize, void* aAddr)
{
  RASSERT(iSize);
  
  return aAddr;
}

/*----------------------------------------------------------------------------*/
/* return a row buffer to the buffer pool                                     */
/*----------------------------------------------------------------------------*/
void EntryBuffer::operator delete(void* pRowBufferAddress)
{
  Buffer* pBuffer = static_cast<Buffer*>(pRowBufferAddress);
  
  Heap::vFree(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EntryBuffer::EntryBuffer(void)
: Buffer(CN_ENTRYBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_pPrevBuffer = nullptr;
  m_pNextBuffer = nullptr;
  m_bIsLast     = false;
  //m_xWsp
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  _checkInitialised_(CN_ENTRYBUFFER);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EntryBuffer::EntryBuffer(i8 iDataBytes)
: Buffer(CN_ENTRYBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_xWsp.vUsed(iDataBytes);
  
  m_pPrevBuffer = nullptr;
  m_pNextBuffer = nullptr;
  m_bIsLast     = false;
  
  //m_xWsp
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  _checkInitialised_(CN_ENTRYBUFFER);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EntryBuffer::EntryBuffer(Index* pIndex, i8 nBufferLevel)
: EntryBuffer(CN_ENTRYBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_pIndex       = pIndex;
  m_nBufferLevel = nBufferLevel;
  m_bIsLast     = false;
  
  m_xWsp.vZero(CN_ENTRYBUFFER);
  
  // new the head
  vAllotHead();
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  _checkInitialised_(CN_ENTRYBUFFER);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EntryBuffer::EntryBuffer(eCN eClassNo)
: Buffer(eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo,_FOOTER_);
  
  m_eDurability = BD_TEMPORARY;
  m_nEntries    = 0;
  m_bIsLast     = false;
  
  m_pPrevBuffer  = nullptr;
  m_pNextBuffer  = nullptr;
  m_pHigherLevel = nullptr;
  
  m_nBufferLevel = 0;
  m_pIndex       = nullptr;
  
  _checkInitialised_(eClassNo);
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
EntryBuffer::~EntryBuffer(void)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* row buffer factory                                                         */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pNewEntryBuffer(eCN eClassNo)
{
  if (gpBufferPool->iFreeCount() <= C_BUFFER_THRESHOLD)
    return nullptr;
  
  EntryBuffer* pEntryBuffer =
    reinterpret_cast<EntryBuffer*>(Heap::pAllocate(CN_ENTRYBUFFER));
  
  if (! pEntryBuffer)
    return nullptr;
  
  new(pEntryBuffer) EntryBuffer(eClassNo);
  
  return pEntryBuffer;
}

/*----------------------------------------------------------------------------*/
/* get durability                                                             */
/*----------------------------------------------------------------------------*/
eBD EntryBuffer::eDurability(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_eDurability;
}

/*----------------------------------------------------------------------------*/
/* set durability                                                             */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vDurability(eBD eDurability)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_eDurability = eDurability;
}

/*----------------------------------------------------------------------------*/
/* get number of enteries in the buffer                                       */
/*----------------------------------------------------------------------------*/
i8 EntryBuffer::nEntries(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  return m_nEntries;
}

/*----------------------------------------------------------------------------*/
/* get next buffer                                                            */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pNextBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  return m_pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* set next buffer                                                            */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vNextBuffer(EntryBuffer* pNextBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_pNextBuffer = pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* get prev buffer                                                            */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pPrevBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_pPrevBuffer;
}

/*----------------------------------------------------------------------------*/
/* set prev buffer                                                            */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vPrevBuffer(EntryBuffer* pPrevBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_pPrevBuffer = pPrevBuffer;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump rowbuffer                                                             */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vDump(eCN eClass, Object* pObject, Str* aTab)
{
  ASSERT(eClass == CN_ENTRYBUFFER);
  ASSERT(pObject->eClass() == CN_ENTRYBUFFER);
  
  EntryBuffer* pRowBuffer = reinterpret_cast<EntryBuffer*>(pObject);
  
  vTrace("rows %lld used %lld free %lld prevBuffer %p nextBuffer %p\n"_Str,
         pRowBuffer->m_nEntries,
         pRowBuffer->m_xWsp.nUsed(),
         pRowBuffer->m_xWsp.nFree(),
         pRowBuffer->m_pPrevBuffer,
         pRowBuffer->m_pNextBuffer);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot an entry for a row in this buffer                                    */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::pAllotEntry(i8 iStrLength)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  i8 nBytesRequired = Entry::nBytesRequired(0,0,iStrLength);
  
  // test  sufficient freespace
  if (nFreeBytes() < nBytesRequired) {
    return nullptr;
  }
  
  Entry* pRow = reinterpret_cast<Entry*>(m_xWsp.aAllot(nBytesRequired));
  new(pRow) Entry(CN_ROW,nBytesRequired,0);
  
  m_nEntries++;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return pRow;
}

/*----------------------------------------------------------------------------*/
/* append rows of arg buffer into this buffer                                 */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vAppendBuffer(EntryBuffer* pSrcBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // how many bytes to be copied from arg buffer
  i8 nBytes  = pSrcBuffer->nUsedBytes();
  byte* aSrc = pSrcBuffer->aDataArea();
  
  // allot space in this buffer
  void* pDst = m_xWsp.aAllot(nBytes);
  
  // copy arg rows into this buffer
  MemLib::vCopy(pDst,aSrc,nBytes,CN_ENTRYBUFFER);
  
  // inc the number of rows in this buffer
  i8 nSrcRows = pSrcBuffer->m_nEntries;
  m_nEntries += nSrcRows;
}

/*----------------------------------------------------------------------------*/
/* copy given buffer                                                          */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vCopyBuffer(EntryBuffer* pSrcBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // how many bytes to be copied from src buffer
  i8 nBytes  = pSrcBuffer->nUsedBytes();
  byte* aSrc = pSrcBuffer->aDataArea();
  
  // reset and allot space in this buffer
  m_xWsp.vUsed(0);
  void* pDst = m_xWsp.aAllot(nBytes);
  
  // copy from rows into this buffer
  MemLib::vCopy(pDst,aSrc,nBytes,CN_ENTRYBUFFER);
  
  // set the number of rows in this buffer
  m_nEntries = pSrcBuffer->nEntries();
}

/*----------------------------------------------------------------------------*/
/* get address of data area                                                   */
/*----------------------------------------------------------------------------*/
byte* EntryBuffer::aDataArea(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  byte* pStart = reinterpret_cast<byte*>(m_xWsp.aArea());
  
  return pStart;
}

/*----------------------------------------------------------------------------*/
/* first row                                                                  */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::pFirstRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  if ( ! m_xWsp.nSize() )
    return nullptr;
  
  Entry* pFirst = reinterpret_cast<Entry*>(m_xWsp.aArea());
  
  return pFirst;
}

/*----------------------------------------------------------------------------*/
/* how much free space (in bytes) left in segment?                            */
/*----------------------------------------------------------------------------*/
i8 EntryBuffer::nFreeBytes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 nBytes = m_xWsp.nFree();
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* insert row into this buffer                                                */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bInsertRow(EntWrk& xRowWrk, Entry** ppRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 nBytes = Entry::nBytesRequired(xRowWrk);
  
  Entry* pRowEntry = pAllotEntry(nBytes);
  if ( ! pRowEntry )
    return false;
  
  pRowEntry->vSetEntry(xRowWrk);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  *ppRow = pRowEntry;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* insert row into this buffer                                                */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bInsertRow(s256& sRow, Entry** ppRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // create working row
  ValWrk xValWrk;
  
  //----------------------------------------------------------------------------
  // form the rowId
  bn nBufferNo = m_nBufferNo;
  i8 nRowNo = nEntries();
  Id rowId(*nBufferNo, nRowNo);
  
  //----------------------------------------------------------------------------
  // form the key
  xValWrk = nRowNo;
  
  KeyWrk xKeyWrk;
  Column* pColumn = m_pIndex->pIdxColVec()->pGet(0);
  ASSERT(pColumn->iColumnNo() == -1);
  ASSERT(pColumn->eColumnType() == CT_INTEGER);
  xKeyWrk.vAddDatum(xValWrk, pColumn);
    
  i8 nBytes = Entry::nBytesRequired(0, 8, StrVal::nBytesRequired(sRow.iLength()));
  
  Entry* pRowEntry = pAllotEntry(nBytes);
  if ( ! pRowEntry )
    return false;
  
  //----------------------------------------------------------------------------
  // add the id and key to EntWrk
  pRowEntry->vSetEntry(rowId, nRowNo, sRow);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  *ppRow = pRowEntry;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* set buffer is last                                                         */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vIsLast(bool bIsLast)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_bIsLast = bIsLast;
}

/*----------------------------------------------------------------------------*/
/* get if buffer is last                                                      */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bIsLast(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_bIsLast;
}

/*----------------------------------------------------------------------------*/
/* get next row                                                               */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::pNextRow(Entry* pCurr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 nBytes = pCurr->nSizeEntry();
  
  Entry* pNext = pAdd<Entry*>(pCurr,nBytes);
  
  if ((char*)pNext >= (char*)m_xWsp.aLatter())
    return nullptr;
  
  return pNext;
}

/*----------------------------------------------------------------------------*/
/* set row count                                                              */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vRowCount(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_nEntries = 0;
  for (Entry* pRow = pFirstRow(); pRow; pRow = pNextRow(pRow))
    m_nEntries++;
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_,CN_ENTRYBUFFER);
}

/*----------------------------------------------------------------------------*/
/* get bytes used                                                             */
/*----------------------------------------------------------------------------*/
i8 EntryBuffer::nUsedBytes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 nBytes = m_xWsp.nUsed();
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* analyze                                                                    */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vAnalyze(s48K& sAnalysis, bool bDetailed, i8& iBuffers, i8& iEntries)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  if (m_pIndex->bIsHidden()) {
    
    ASSERT(m_nBufferLevel == 0);
    EntryBuffer* pNextBuffer = this;
    
    do {
      ASSERT(pNextBuffer->nBufferLevel() == 0);
      
      iBuffers++;
      iEntries += (pNextBuffer->m_nEntries - 1);
      
      if (bDetailed)
      sAnalysis.vCatSprint("     buffer %5lld (L%lld) : %lld entries\n"_Str,
                           iBuffers,
                           pNextBuffer->m_nBufferLevel,
                           pNextBuffer->m_nEntries - 1);

      EntryBuffer* pThisBuffer = pNextBuffer;
      pNextBuffer = pNextBuffer->pNextBuffer();
    } while (pNextBuffer);
    
    return;
  }

  iBuffers++;
  iEntries += (m_nEntries - 1);
  
  if (bDetailed)
    sAnalysis.vCatSprint("     buffer %5lld (L%lld) : %lld entries\n"_Str,
                         iBuffers,
                         m_nBufferLevel,
                         m_nEntries - 1);
  
  // recursively dump the lower level buffers
  if (m_nBufferLevel != 0) {
    
    for (Entry* pEntry = pNextEntry(pHead()); pEntry != nullptr;
         pEntry = pNextEntry(pEntry)) {
      
      // get the index buffer from the entry
      EntryBuffer* pEntryBuffer = pEntry->pIndexBuffer();;
      
      ASSERT(pEntryBuffer);
      ASSERT(pEntryBuffer->eClass() == CN_ENTRYBUFFER);
      
      pEntryBuffer->vAnalyze(sAnalysis, bDetailed, iBuffers, iEntries);
      
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* get the buffer level                                                       */
/*----------------------------------------------------------------------------*/
i8 EntryBuffer::nBufferLevel(void)
{
  return m_nBufferLevel;
}

/*----------------------------------------------------------------------------*/
/* get decile full of the buffer                                              */
/*----------------------------------------------------------------------------*/
i8 EntryBuffer::iDecile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 iDecile;
  
  if (gbIndexSpace)
    iDecile = IntLib::iDecile(m_xWsp.nUsed(), C_SIZE_POOL_BUFFER);
  else
    iDecile = IntLib::iDecile(m_nEntries, gnIndexEntries);
  
  
  return iDecile;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // show this buffer
  vShow();
  
  // recursively dump the lower level buffers
  if (m_nBufferLevel != 0) {
    
    for (Entry* pEntry = pNextEntry(pHead()); pEntry != nullptr;
         pEntry = pNextEntry(pEntry)) {
      
      // get the index buffer from the entry
      EntryBuffer* pEntryBuffer = pEntry->pIndexBuffer();;
      
      ASSERT(pEntryBuffer);
      ASSERT(pEntryBuffer->eClass() == CN_ENTRYBUFFER);
      
      pEntryBuffer->vDump();
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* get first entry (after the head) of skip list in this index buffer         */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::pFirstEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // the head entry will have been alloted during constructor
  ASSERT(m_xWsp.nUsed());
  
  // first get the head
  Entry* pHead = _pEntry(m_xWsp.aArea());
  
  // return the next entry
  return pNextEntry(pHead);
}

/*----------------------------------------------------------------------------*/
/* does the buffer have space for nBytes ?                                    */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bHasSpace(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 iFree = m_xWsp.nFree();
  
  // free space calculation based on no of enteries
  if (gbIndexSpace) {
    if (iFree < nBytes)
      return false;
  }
  else {
    if (m_nEntries > gnIndexEntries)
      return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get head entry of skip list in this index buffer                           */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::pHead(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // the head entry will have been alloted during constructor
  ASSERT(m_xWsp.nUsed());
  
  // the head entry is the first entry in wsp
  Entry* pHead = _pEntry(m_xWsp.aArea());
  
  return pHead;
}

/*----------------------------------------------------------------------------*/
/* get the higher level index buffer                                          */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pHigherLevel(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return (EntryBuffer*)m_pHigherLevel;
}

/*----------------------------------------------------------------------------*/
/* set the higher level index buffer                                          */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vHigherLevel(EntryBuffer* pHigherLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  //ASSERT( ! m_pHigherLevel);
  
  m_pHigherLevel = pHigherLevel;
}

/*----------------------------------------------------------------------------*/
/* get the index of the index buffer                                          */
/*----------------------------------------------------------------------------*/
Index* EntryBuffer::pIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_pIndex;
}

/*----------------------------------------------------------------------------*/
/* return no of index buffers under this buffer                               */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vBufferCount(i8& nBuffers)
{
  EntryBuffer* pNextBuffer = nullptr;
  
  if (m_pIndex->bIsHidden()) {
    ASSERT(m_nBufferLevel == 0);
    pNextBuffer = this->pNextBuffer();
    nBuffers++;
    
    while (pNextBuffer) {
      ASSERT(pNextBuffer->nBufferLevel() == 0);
      
      EntryBuffer* pThisBuffer = pNextBuffer;
      pNextBuffer = pNextBuffer->pNextBuffer();
      nBuffers++;
    }

  }
  else {
    // count this buffer
    nBuffers++;
    
    // recursively dump the lower level buffers
    if (m_nBufferLevel != 0) {
      
      for (Entry* pEntry = pNextEntry(pHead()); pEntry != nullptr;
           pEntry = pNextEntry(pEntry)) {
        
        // get the index buffer from the entry
        pNextBuffer = pEntry->pIndexBuffer();;
        
        ASSERT(pNextBuffer);
        ASSERT(pNextBuffer->eClass() == CN_ENTRYBUFFER);
        
        pNextBuffer->vBufferCount(nBuffers);
        
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/* insert an entry for {key,id} into an index buffer at given level           */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bInsertEntry(i8 iTxId, Str* pRow, i8 iLength)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  Entry* pEntry;
  
  ASSERT(nBufferLevel() == 0);
  
  KeyWrk xKeyWrk;
  ValWrk xValWrk;
  
  //----------------------------------------------------------------------------
  // form the rowId
  bn nBufferNo = m_nBufferNo;
  i8 nRowNo = nEntries();
  Id rowId(*nBufferNo, nRowNo);
  
  //----------------------------------------------------------------------------
  // form the key
  xValWrk = nRowNo;
  Column* pColumn = m_pIndex->pIdxColVec()->pGet(0);
  ASSERT(pColumn->iColumnNo() == -1);
  xKeyWrk.vAddDatum(xValWrk, pColumn);
  
  //----------------------------------------------------------------------------
  // add the id and key to EntWrk
  EntWrk xEntWrk(rowId, xKeyWrk);
  
  //----------------------------------------------------------------------------
  // add the row data to the EntWrk
  xEntWrk.vData(pRow, iLength);
  
  //----------------------------------------------------------------------------
  // check if current index buffer has enough space
  i8 nEntryBytes = Entry::nBytesRequired(xEntWrk);
  
  if ( ! bHasSpace(nEntryBytes) ) {
    return false;
  }
  else {
    
    // insert entry into current index buffer
    pEntry = pInsert(xEntWrk);
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* insert new entry into this index buffer                                    */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::pInsert(EntWrk& xEntWrk)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  //vTrace("Inserting key:\n"_Str);
  //xKeyWrk.vShow();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // insert a new entry into the skip list
  
  // find the entry after which the new entry.key will be inserted
  EntryVec xPriorVec;
  bool bIsEqual = false;
  Entry* pAfterEntry = pPosition(xEntWrk.aKey(), JC_AFTER, &xPriorVec, bIsEqual);
    
  if (m_pIndex->bIsUnique() and bIsEqual) {
    vTrace("Warning: Duplicate key in unique index %s "_Str, m_pIndex->aIndexName());
    xEntWrk.aKey()->vShow(m_pIndex->pIdxColVec());
    vTrace("\n"_Str);
  }
  
  // create a new entry after the prior entry and set it
  //EntWrk xEntWrk(xId,xKeyWrk);
  i8 nEntryBytes = Entry::nBytesRequired(xEntWrk);
  Entry* pNewEntry = pAllotEntry(nEntryBytes, xEntWrk.nOffsets(), pAfterEntry);
  pNewEntry->vSetEntry(xEntWrk);
  
  //vTrace("\n\n---------INDEX BUFFER AFTER MOVING --------------\n"_Str);
  //vShow();
  //vTrace("-----------------------\n\n"_Str);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // revise the offsets of the prior entries from the prior vector
  
  // fix the prior vector after the move
  vFixPriorOffset(&xPriorVec, pNewEntry);
  
  // get the absolute offset of the new entry
  i8 iNewEntryOffsetAbs = iToOffsetAbs(pNewEntry);
  
  //pNewEntry->vShow(pNewEntry->nOffsets(), m_pIndex);
  
  // step thru each offset to be revised
  for (on n_OffsetNo = pNewEntry->nHighestOffset(); n_OffsetNo.bInRange(); n_OffsetNo--) {
    
    // get prior entry being revised
    ln nLevelNo = n_OffsetNo.nToLevelNo();
    Entry* aPriorEntry = xPriorVec[nLevelNo];
    
    i8 iPriorsRightOffsetAbs = iToRightOffsetAbs(aPriorEntry,n_OffsetNo);
    
    // is the prior's right offset after new entry : [PURPLE] (grey)-> [GREEN]
    if (iPriorsRightOffsetAbs > iNewEntryOffsetAbs) {
      
      // get post entry : [GREEN]
      Entry* aPostEntry = pRightEntry(aPriorEntry,nLevelNo);
      
      // calculate new's right offset : [RED ARROW]
      i8 iNewRightOffset = uOffsetBetween(pNewEntry,aPostEntry);
      
      // set right offset on new : [RED.hasoffset]
      pNewEntry->vRightOffset(n_OffsetNo,iNewRightOffset);
      
      // calculate prior's revised offset : [PURPLE ARROW]
      i8 iPriorRightOffset = uOffsetBetween(aPriorEntry,pNewEntry);
      
      aPriorEntry->vRightOffset(n_OffsetNo,iPriorRightOffset);
    }
    else {
      
      // this is the case where the prior entry now needs to point to the new
      // prior was at C_NIL_OFFSET and now needs to point to the newEntry
      ASSERT(aPriorEntry->iRightOffset(n_OffsetNo) == C_NIL_OFFSET);
      ASSERT(pNewEntry->iRightOffset(n_OffsetNo) == C_NIL_OFFSET);
      
      // calculate new right offset
      i8 iNewRightOffset = uOffsetBetween(pNewEntry,aPriorEntry);
      
      // set the right offset in the prior entry to offset of new entry
      aPriorEntry->vRightOffset(n_OffsetNo, iNewRightOffset);
    }
    
  } // for
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // fix higher level buffers
  // this is needed in case we inserted a new first key in the buffer
  //vShow();
  vFixHigherLevel(pNewEntry);
  
  //pNewEntry->vShow(pNewEntry->nOffsets(), m_pIndex);
  
  return pNewEntry;
}

/*----------------------------------------------------------------------------*/
/* get next entry (eq to right L0) after given entry (null if no next)        */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::pNextEntry(Entry* pEntry)                           //! tune
{
  // get the size of given entry
  i8 nBytes = pEntry->nSizeEntry();              // inline this
  
  // compute the address of possible next entry
  Entry* pNext = pAdd<Entry*>(pEntry,nBytes);    // inline
  
  // check for valid entry
  Entry* pLatter = _pEntry(m_xWsp.aLatter());    // inline
  if (pNext < pLatter)
    return pNext;
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* position at entry with lower bound at key                                  */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::pPosition(KeyWrk& xKeyWrk, eJC eJuxtaCompare)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  Entry* pEntry;
  bool bIsEqual = false;
  
  pEntry = pPosition(xKeyWrk.aKey(), eJuxtaCompare, nullptr, bIsEqual);
  
  if (pEntry == pHead())
    pEntry = pNextEntry(pEntry);
  
  return pEntry;
}

/*----------------------------------------------------------------------------*/
/* position at an index buffer at level iLevel starting from this buffer      */
/*----------------------------------------------------------------------------*/
EntryBuffer* EntryBuffer::pPositionLevel(KeyWrk& xKeyWrk, eJC eJuxtaCompare, i8 iLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  EntryBuffer* pEntryBuffer = this;
  
  i8 iBufferLevel;
  Entry* pEntry;
  
  while (true) {
    iBufferLevel = pEntryBuffer->nBufferLevel();
    
    if (iBufferLevel == iLevel)
      break;
    
    // get the entry
    pEntry = pEntryBuffer->pPosition(xKeyWrk, eJuxtaCompare);
    ASSERT(pEntry);
    
    // get the index buffer of the entry
    pEntryBuffer = pEntry->pIndexBuffer();
    ASSERT(pEntryBuffer);
    ASSERT(pEntryBuffer->eClass() == CN_ENTRYBUFFER);
  }
  
  return pEntryBuffer;
}

/*----------------------------------------------------------------------------*/
/* remove existing entry                                                      */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bRemove(Key* aKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vShow(void)
{
  s256 sHigherLevel;
  if (m_pHigherLevel)
    StrLib::wToString(((EntryBuffer*)m_pHigherLevel)->m_nBufferLevel, sHigherLevel.s());
  else
    sHigherLevel = "..."_Str;
  
  vTrace("EntryBuffer %p (buffer %lld): level L%lld HigerLevel %p (L%s) "
         "entries %lld prevBuffer %p nextBuffer %p \n"_Str,
         this,
         *nBufferNo(),
         m_nBufferLevel,
         m_pHigherLevel,
         sHigherLevel.aStr(),
         m_nEntries,
         m_pPrevBuffer,
         m_pNextBuffer);
  
  for (Entry* pEntry = pHead(); pEntry != nullptr; pEntry = pNextEntry(pEntry)) {
    vTrace("%8lld: "_Str, nSub(pEntry, m_xWsp.aArea()));
    pEntry->vShow(iToOffsetAbs(pEntry), m_pIndex);
  }
  
  vTrace("\n"_Str);
  
}

/*----------------------------------------------------------------------------*/
/* split this buffer into two: this original buffer plus a new buffer         */
/*----------------------------------------------------------------------------*/
#define NEW_SPLIT
#ifndef NEW_SPLIT
EntryBuffer* EntryBuffer::pSplitBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  EntryVec xPriorVec;
  
  // get split entry to remain in this buffer and
  // move entry (and those that follow) which will be moved into new buffer
  i8 iSplitAtEntry   = m_nEntries/2;
  Entry* pSplitEntry = pGetEntry(iSplitAtEntry);
  Entry* aMoveEntry  = pNextEntry(pSplitEntry);
  //aSplitEntry->vShow(0,m_pIndex->pIdxColVec());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set offsets to entries after the split entry in this buffer to nil
  
  // get vector of prior entries which have offsets to the split entry
  // show this buffer
  //vShow();
  
  Key* aSplitKey= pSplitEntry->aKey();
  Entry* pCrossCheck = pPosition(aSplitKey, JC_AFTER, &xPriorVec);
  
  // Note this ASSERT fails if we have dulplicates in the table     //ALKA: TODO
  // Need to fix when we cater for non-unique indexes
  //aCrossCheck->vShow(0,m_pIndex->pIdxColVec());
  ASSERT(pCrossCheck == pSplitEntry);
  
  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
  
  EntryVec xPrevVec;
  vPriorVec(aMoveEntry,&xPrevVec);
  
  //  vTrace("Split %p\n"_Str,pSplitEntry);
  //  vTrace("Move  %p\n"_Str,aMoveEntry);
  //  for (on n_OffsetNo; n_OffsetNo.bInRange(); n_OffsetNo++) {
  //    ln n_Level = n_OffsetNo.nToLevelNo();
  //    vTrace("%p v %p\n"_Str,xPrevVec[nLevel],xPriorVec[n_Level]);
  //    ASSERT(xPrevVec[n_Level] == xPriorVec[n_Level]);
  //  }
  
  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
  
  // set any prior offset to nil if its an offset to an entry in split half
  i8 iSplitEntryOffsetAbs = iToOffsetAbs(pSplitEntry);
  
  for (on n_OffsetNo; n_OffsetNo.bInRange(); n_OffsetNo++) {
    
    // calculate level into prior vector for this offset and get entry
    ln n_Level = n_OffsetNo.nToLevelNo();
    Entry* pEntry = xPriorVec[n_Level];
    
    if (iToRightOffsetAbs(pEntry,n_OffsetNo) > iSplitEntryOffsetAbs)
      pEntry->vRightOffset(n_OffsetNo, C_NIL_OFFSET);
  }
  
  // set all right offsets in the split entry to C_NIL_OFFSET
  for (on nOffsetNo = pSplitEntry->nHighestOffset(); nOffsetNo.bInRange(); nOffsetNo--)
    pSplitEntry->vRightOffset(nOffsetNo, C_NIL_OFFSET);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create new buffer and move the entries into it
  
  // create new buffer at the same index level and link the two buffers
  EntryBuffer* pNewBuffer = new EntryBuffer(m_pIndex, m_nBufferLevel);
  pNewBuffer->m_pNextBuffer = pNextBuffer();
  pNewBuffer->m_pPrevBuffer = this;
  m_pNextBuffer = pNewBuffer;
  
  // copy entries from this buffer starting at splitEntry to the new buffer
  m_nEntries -= pNewBuffer->iCopyEntries(this, aMoveEntry);
  
  // give copied bytes back to wsp of this original buffer
  i8 nBytesToGive = nSub(m_xWsp.aLatter(), aMoveEntry);
  m_xWsp.vGive(nBytesToGive);
  ASSERT(m_xWsp.aLatter() == pAdd<void*>((void*)pSplitEntry,pSplitEntry->nSize()));
  
  return pNewBuffer;
}
#else //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
EntryBuffer* EntryBuffer::pSplitBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  EntryVec xPriorVec;
  
  // get split entry to remain in this buffer and
  // move entry (and those that follow) which will be moved into new buffer
  i8 iSplitAtEntry   = m_nEntries/2;
  Entry* pSplitEntry = pGetEntry(iSplitAtEntry);
  Entry* aMoveEntry  = pNextEntry(pSplitEntry);
  //aSplitEntry->vShow(0,m_pIndex->pIdxColVec());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set offsets to entries after the split entry in this buffer to nil
  
  // get vector of prior entries which have offsets to the split entry
  // show this buffer
  //vShow();
  
  Key* aSplitKey= pSplitEntry->aKey();
  //Entry* pCrossCheck = pPosition(aSplitKey, JC_AFTER, &xPriorVec);
  
  // Note this ASSERT fails if we have dulplicates in the table     //ALKA: TODO
  // Need to fix when we cater for non-unique indexes
  //aCrossCheck->vShow(0,m_pIndex->pIdxColVec());
  //ASSERT(pCrossCheck == pSplitEntry);
  
  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
  
  //EntryVec xPrevVec;
  vPriorVec(aMoveEntry,&xPriorVec);
  
  //  vTrace("Split %p\n"_Str,pSplitEntry);
  //  vTrace("Move  %p\n"_Str,aMoveEntry);
  //  for (on nOffsetNo; nOffsetNo.bInRange(); nOffsetNo++) {
  //    ln nLevel = nOffsetNo.nToLevelNo();
  //    vTrace("%p v %p\n"_Str,xPrevVec[nLevel],xPriorVec[nLevel]);
  //    ASSERT(xPrevVec[nLevel] == xPriorVec[nLevel]);
  //  }
  
  // + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + + +
  
  // set any prior offset to nil if its an offset to an entry in split half
  i8 iSplitEntryOffsetAbs = iToOffsetAbs(pSplitEntry);
  
  for (on nOffsetNo; nOffsetNo.bInRange(); nOffsetNo++) {
    
    // calculate level into prior vector for this offset and get entry
    ln nLevel = nOffsetNo.nToLevelNo();
    Entry* pEntry = xPriorVec[nLevel];
    
    if (iToRightOffsetAbs(pEntry,nOffsetNo) > iSplitEntryOffsetAbs)
      pEntry->vRightOffset(nOffsetNo, C_NIL_OFFSET);
  }
  
  // set all right offsets in the split entry to C_NIL_OFFSET
  for (on nOffsetNo = pSplitEntry->nHighestOffset(); nOffsetNo.bInRange(); nOffsetNo--)
    pSplitEntry->vRightOffset(nOffsetNo, C_NIL_OFFSET);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create new buffer and move the entries into it
  
  // create new buffer at the same index level and link the two buffers
  EntryBuffer* pNewBuffer = new EntryBuffer(m_pIndex, m_nBufferLevel);
  pNewBuffer->m_pNextBuffer = pNextBuffer();
  pNewBuffer->m_pPrevBuffer = this;
  m_pNextBuffer = pNewBuffer;
  
  // copy entries from this buffer starting at splitEntry to the new buffer
  m_nEntries -= pNewBuffer->iCopyEntries(this, aMoveEntry);
  
  // give copied bytes back to wsp of this original buffer
  i8 nBytesToGive = nSub(m_xWsp.aLatter(), aMoveEntry);
  m_xWsp.vGive(nBytesToGive);
  ASSERT(m_xWsp.aLatter() == pAdd<void*>((void*)pSplitEntry,pSplitEntry->nSizeEntry()));
  
  return pNewBuffer;
}
#endif // NEW_SPLIT

/*----------------------------------------------------------------------------*/
/* verify                                                                     */
/*----------------------------------------------------------------------------*/
bool EntryBuffer::bVerify(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   
   Entries in an index buffer
   
   EntryA                                                         EntryH
   -------                                                        -------
   |       | ---------------------------------------------------->|       |
   L2 |       |                                                      |       |
   -------                    EntryD                              -------
   -------                    -------                             -------
   |       |                  |       |                           |       |
   L1 |       |----------------->|       |-------------------------->|       |
   -------  EntryB    EntryC  -------   EntryE   EntryF  EntryG   -------
   -------  -------  -------  -------  -------  -------  -------  -------
   |       ||       ||       ||       ||       ||       ||       ||       |
   L0 |  KeyA || KeyB  || KeyC  ||  KeyD || KeyE  || KeyF  || KeyG  || KeyH  |
   -------  -------  -------  -------  -------  -------  -------  -------
   
   
   We start at L2
   EntryOne is EntryA
   EntryTwo is EntryH
   
   keyH is after keyA
   
   We reach the end of Level2
   
   Move to L1
   EntryOne is EntryA
   EntryTwo is EntryD
   keyD is after jeyA
   
   EntryOne becomes EntryD
   EntryTwo is EntryH
   KeyH is after keyD
   
   We reach the end of Level1
   
   Move to L0
   EntryOne is EntryA
   EntryTwo is EntryB
   KeyB is after KeyA
   ....
   
   // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  
  
  ln nLevelNo;
  
  // start at the highest level in the index buffer's skip list (L2)
  nLevelNo = gnIndexLevels - 1;
  
  do {
    
    // get the head entry of the buffer
    Entry* pEntryOne = pHead();
    if ( ! pEntryOne )
      return true;
    
    // If we are scanning at a level greater than the level of the entry
    // just move down one level
    if (nLevelNo > (i8)(pEntryOne->nOffsets())) {
      nLevelNo--;
      continue;
    }
    
    // we have a right entry at this level entryA
    // get the right entry
    Entry* pEntryTwo = pRightEntry(pEntryOne, nLevelNo);
    
    // if no right entry at this level, move down one level
    if ( ! pEntryTwo ) {
      nLevelNo--;
      continue;
    }
    
    // iterate through the entries at this level
    while (pEntryTwo) {
      
      // get keys of the two entries
      Key* pKeyA = pEntryOne->aKey();
      Key* pKeyB = pEntryTwo->aKey();
      
      // if entryA is not the head, compare the keys
      if (pEntryOne != pHead()) {
        
        // KeyB should should be after keyA
        bool bIsEqual = false;
        bool bAfter = pKeyB->bJuxtaAfter(pKeyA, m_pIndex->pIdxColVec(), bIsEqual);
        
        // if keyB is not after keyA , we have a problem
        if ( ! bAfter )
          return false;
      }
      
      // get next entry
      pEntryOne = pEntryTwo;
      pEntryTwo = pNextEntry(pEntryOne);
    }
    
    // move down one level and start with the head again
    nLevelNo--;
    
  } while (nLevelNo >= 0_ln );
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* truncate                                                                    */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vTruncate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  EntryBuffer* pNextBuffer;
  
  // hidden index buffers are just one level
  if (m_pIndex->bIsHidden()) {
    ASSERT(m_nBufferLevel == 0);
    pNextBuffer = this->pNextBuffer();
    
    while (pNextBuffer) {
      ASSERT(pNextBuffer->nBufferLevel() == 0);

      EntryBuffer* pThisBuffer = pNextBuffer;
      pNextBuffer = pNextBuffer->pNextBuffer();
      delete pThisBuffer;
    }
  }
  else {
    if (m_nBufferLevel != 0) {
      
      // recursively delete the lower level buffers
      for (Entry* pEntry = pNextEntry(pHead()); pEntry != nullptr;
           pEntry = pNextEntry(pEntry)) {
        
        // get the index buffer from the entry
        pNextBuffer = pEntry->pIndexBuffer();;
        
        ASSERT(pNextBuffer);
        ASSERT(pNextBuffer->eClass() == CN_ENTRYBUFFER);
        pNextBuffer->vTruncate();
      }
    }
  }
  
  delete this;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* adjust the  entry offsets                                                  */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vAdjustOffsets(Entry* pEntry, i8 nAdjustAmount)
{
  for (on nOffsetNo; nOffsetNo < pEntry->nOffsets(); nOffsetNo++) {
    
    //
    i8 iRightOffset = pEntry->iRightOffset(nOffsetNo);
    
    // here we adjust the iRightOffset if iRightOffset is > 0
    if (iRightOffset > 0) {
      if (pEntry != pHead()) {
        i8 i = 10;
      }
      i8 iNewRightOffset = iRightOffset + nAdjustAmount;
      pEntry->vRightOffset(nOffsetNo,iNewRightOffset);
    }
  }
}

/*----------------------------------------------------------------------------*/
/* allot new entry after given entry (return null if insufficient space)      */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::pAllotEntry(i8 nBytes, i8 nOffsets, Entry* aAfterEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // there should be enough space for the new entry
  if (m_xWsp.nFree() < nBytes)
    return nullptr;
  
  // allot space for the new entry
  m_xWsp.vTake(nBytes);
  Entry* pLatter = _pEntry(m_xWsp.aLatter());
  
  Entry* pNewEntry;
  
  if (aAfterEntry) {
    // compute address of new entry
    i8 nAfterBytes = aAfterEntry->nSizeEntry();
    pNewEntry = pAdd<Entry*>(aAfterEntry,nAfterBytes);
    
    // move subset of entries to right to make room for new entry
    vMoveRight(pNewEntry,nBytes);
  }
  else {
    pNewEntry = _pEntry(m_xWsp.aArea());
  }

  MemLib::vScribble(pNewEntry,nBytes,CN_ENTRYBUFFER);

  // run the constructor of the new entry
  new (pNewEntry) Entry(CN_ENTRY,nBytes,nOffsets);
  
  // increment the no of entries
  m_nEntries++;
  
  return pNewEntry;
}

/*----------------------------------------------------------------------------*/
/* allot head entry (during constructor)                                      */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vAllotHead(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // must be the first item in wsp
  ASSERT( ! m_xWsp.nUsed() );
  ASSERT( ! m_nEntries );
  
  // how many bytes are required
  i8 nBytes = Entry::nBytesRequired(gnIndexOffsets,0,0);
  
  // allot the head
  
  Entry* pHead = pAllotEntry(nBytes,gnIndexOffsets,nullptr);
  
  // initialize the head
  for (on nOffsetNo; nOffsetNo.bInRange(); nOffsetNo++)
    pHead->vRightOffset(nOffsetNo, C_NIL_OFFSET);
}

/*----------------------------------------------------------------------------*/
/* copy entries from fromBuffer starting at fromEntry to this buffer          */
/*----------------------------------------------------------------------------*/
i8 EntryBuffer::iCopyEntries(EntryBuffer* pFromBuffer, Entry* aFromEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 nBufferLevel = pFromBuffer->nBufferLevel();
  
  i8 nCopied = 0;
  for (Entry* pEntry = aFromEntry;
       pEntry;
       pEntry = pFromBuffer->pNextEntry(pEntry)) {
    
    // form KeyWrk from the entry
    KeyWrk xKeyWrk(pEntry->aKey(), m_pIndex->pIdxColVec());
    
    // form the EntWrk from the entry
    EntWrk xEntWrk(pEntry->xId(), xKeyWrk);
    
    // insert entry into this buffer
    pInsert(xEntWrk);
    
    // if this buffer is a higher level buffer,
    // set this buffer as the higher level buffer
    // for the buffer the entry is pointing to
    if (nBufferLevel > 0) {
      
      /*----------------------------------------------------------------------//
       
       We are splitting a LN buffer BN at entry kl
       
       BN
       --------------------
       |ka kj ..kl kp...   |  Level LN buffer
       --------------------
       ^
       |
       BM
       -------------
       |ka kb   ...|  Level LN-1 buffer
       -------------
       
       At this point if LN is not L0 we have a buffer BM pointing to BN.
       
       We are copying entries kl, kp ... from buffer BN to BN+1.
       
       BN             BN+1
       -------------    -------------
       |ka kj   ... |-->| kl kp...   |  Level LN buffer
       -------------    -------------
       ^                ^
       |                |
       |                |
       |                |
       BM              BM+1
       -------------   -------------
       |ka kb   ...|-->| kj kk  ...|   Level LN-1 buffer
       -------------   -------------
       
       Now we need to make buffer BM+1 have its higher level buffer point to BN
       
       //----------------------------------------------------------------------*/
      
      // get the lower level buffer the entry is pointing to
      EntryBuffer* pEntryBuffer = pEntry->pIndexBuffer();
      
      // this index buffer should be at a level lower than nBufferLevel
      ASSERT(pEntryBuffer->nBufferLevel() < nBufferLevel);
      
      // this lower level buffers higher level is this buffer
      pEntryBuffer->m_pHigherLevel = this;
    }
    
    nCopied++;
  }
  
  return nCopied;
}

/*----------------------------------------------------------------------------*/
/* dump the prior vector                                                      */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vDumpPrior(Entry* xPriorVec[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  for (on nOffsetNo; nOffsetNo.bInRange(); nOffsetNo++) {
    i8 iLevel = *nOffsetNo + 1;
    i8 iReviseOffset = xPriorVec[iLevel]->iRightOffset(nOffsetNo);
    vTrace("%p xPriorVec[%lld]->uRightOffset(%lld) = %lld\n"_Str,
           xPriorVec[iLevel], iLevel, *nOffsetNo, iReviseOffset);
  }
}

/*----------------------------------------------------------------------------*/
/* fix the higher level                                                       */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vFixHigherLevel(Entry* aNewEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  /*
   
   If a new entry is inserted in a buffer and its the first entry (not the head)
   then if this buffer has a higher level buffer above it, then we need to
   change the entry pointing to this buffer in that buffer
   
   */
  
  // fix higher levels only if there are any. Duh!
  if ( ! m_pHigherLevel )
    return;
  
  // get the first entry right of head at level 0
  Entry* aFirstEntry = pRightEntry(pHead(), ln::nZeroLevel());
  
  // if the entry we just added is not the first entry , nothing to fix
  if (aNewEntry != aFirstEntry)
    return;
  
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   
   -------------
   |Ka Kb ......|               L(N+1) buffer
   -------------
   |   |
   |    -----------
   |               |
   V               V
   ------------- -------------
   |Ka........ | | Kb.........|  LN bufer
   ------------- -------------
   
   
   Kc gets inserted before Ka, we need to remove Ka and insert Kc instead in L(N+1) buffer
   
   -------------
   |Kc Kb .....|                 L(N+1) buffer
   -------------
   |   |
   |    -----------
   |               |
   V               V
   ------------- -------------
   |Kc Ka...... | | Kb.........|  LN bufer
   ------------- -------------
   
   // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  // we need to update the higher level buffer entry key
  Entry* aOldFirstEntry = pNextEntry(aNewEntry);
  
  KeyWrk xOldKeyWrk;
  xOldKeyWrk.vAddKey(aOldFirstEntry->aKey(), m_pIndex->pIdxColVec());
  
  // create aKeyWrk with the newEntry key
  KeyWrk xNewKeyWrk;
  xNewKeyWrk.vAddKey(aNewEntry->aKey(), m_pIndex->pIdxColVec());
  
  EntryBuffer* pHigherLevel = (EntryBuffer*)m_pHigherLevel;
  
  while (pHigherLevel) {
    //pHigherLevel->vShow();
    //xOldKeyWrk.vShow();
    //xNewKeyWrk.vShow();
    
    pHigherLevel->vReplace(xOldKeyWrk, xNewKeyWrk);
    
    //pHigherLevel->vShow();
    ASSERT(pHigherLevel->bVerify());
    pHigherLevel = (EntryBuffer*)pHigherLevel->m_pHigherLevel;
  }
}

/*----------------------------------------------------------------------------*/
/* replace old key with new key in the index buffer                           */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vReplace(KeyWrk& xOldKeyWrk, KeyWrk& xNewKeyWrk)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // get the size of the old key
  u8 uOldKeySize = xOldKeyWrk.nWspUsed();
  
  // get the size of the new key
  u8 uNewKeySize = xNewKeyWrk.nWspUsed();
  
  if (uOldKeySize == uNewKeySize) {
    vReplaceEqualKey(xOldKeyWrk, xNewKeyWrk);
    return;
  }
  
  if (uOldKeySize > uNewKeySize) {
    vReplaceSmallereKey(xOldKeyWrk, xNewKeyWrk);
    return;
  }
  
  ASSERT(uOldKeySize < uNewKeySize);
  vReplaceLargerKey(xOldKeyWrk, xNewKeyWrk);
}

/*----------------------------------------------------------------------------*/
/* replace old key with new key of same size in the index buffer              */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vReplaceEqualKey(KeyWrk& xOldKeyWrk, KeyWrk& xNewKeyWrk)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   The old and new key are the same size, just change the key in the entry
   
   -------------    -------------
   |Ka(m bytes) |   |Kc(m bytes) |
   -------------    -------------
   
   We keep the same offsets etc. and just replace the key in the entry
   // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
  
  // find the oldEntry in the higher level buffer
  Entry* pOldEntry = pPosition(xOldKeyWrk, JC_SAME);
  
  // assert that we found one
  ASSERT(pOldEntry);
  
  // assert the key of the higher level entry and the old entry match
  bool bIsEqual = false;
  ASSERT(pOldEntry->aKey()->bJuxtaCompare(JC_SAME, xOldKeyWrk.aKey(), m_pIndex->pIdxColVec(), bIsEqual));
  
  EntWrk xEntWrk(pOldEntry);
  xEntWrk.vAddKey(xNewKeyWrk);
  
  // just replace the key in the buffer
  pOldEntry->vReplaceKey(xEntWrk);
}

/*----------------------------------------------------------------------------*/
/* replace old key with new key in the index buffer                           */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vReplaceLargerKey(KeyWrk& xOldKeyWrk, KeyWrk& xNewKeyWrk)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   The new key is longer than the old key
   
   ---------------     --------------------
   |Ka(m bytes)    |   |Kc(m+n bytes)       |
   ---------------     --------------------
   
   We will moveRight in the buffer making space for n bytes
   // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  
  // find the oldEntry in the higher level buffer
  Entry* pOldEntry = pPosition(xOldKeyWrk, JC_SAME);
  
  // assert that we found the old key
  ASSERT(pOldEntry);
  
  // assert the key of the higher level entry and the old entry match
  bool bIsEqual = false;
  ASSERT(pOldEntry->aKey()->bJuxtaCompare(JC_SAME, xOldKeyWrk.aKey(), m_pIndex->pIdxColVec(), bIsEqual));
  
  // create a newEntry at the same location as the pOldEntry
  Entry* pNewEntry = _pEntry(pOldEntry);
  
  // get the size of the old key
  u8 uOldKeySize = xOldKeyWrk.nWspUsed();
  
  // get the size of the new key
  u8 uNewKeySize = xNewKeyWrk.nWspUsed();
  
  // the old key is smaller than the new key
  i8 nSizeDifference = uNewKeySize - uOldKeySize;
  ASSERT(nSizeDifference > 0);
  
  
  // ensure we have space in the buffer
  if (m_xWsp.nFree() < nSizeDifference) {
    NEVER_REACH;                                                   //ALKA: TODO
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create an EntWrk from the old entry, with offsets et all
  EntWrk xEntWrk(pOldEntry);
  xEntWrk.vAddKey(xNewKeyWrk);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // we need to move right nSizeDifference bytes, making room for the bigger key
  
  // calculate the pDst address
  i8 iOldEntrySize = pOldEntry->nSizeEntry();
  void* pDst = pAdd<void*>(pOldEntry, iOldEntrySize);
  
  ASSERT(m_xWsp.nFree() >= nSizeDifference);
  
  // move right
  m_xWsp.vTake(nSizeDifference);
  vMoveRight(pDst, nSizeDifference);
  
  // calculate size of new entry
  i8 nBytes =  iOldEntrySize + nSizeDifference;
  
  // create a new entry at pNewEntry using EntWrk
  new (pNewEntry) Entry(CN_ENTRY, nBytes, xEntWrk.nOffsets());
  pNewEntry->vSetEntry(xEntWrk);
  ASSERT(pNewEntry->nSizeEntry() == nBytes);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // we have replaced the first entry in this buffer
  // hence we need to fix the right offsets in the head
  vAdjustOffsets(pHead(), nSizeDifference);
}

/*----------------------------------------------------------------------------*/
/* replace old key with new key in the index buffer                           */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vReplaceSmallereKey(KeyWrk& xOldKeyWrk, KeyWrk& xNewKeyWrk)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  /*
   The new key is shorter than the old key
   
   old key             new key
   ----------------     ------------
   |Ka(m bytes)    |   |Kc(m-nbytes)|
   ----------------    ------------
   
   We will moveLeft in the buffer reclaiming n bytes
   */
  
  // find the oldEntry in the higher level buffer
  Entry* pOldEntry = pPosition(xOldKeyWrk, JC_SAME);
  
  // assert that we found one
  ASSERT(pOldEntry);
  
  // assert the key of the higher level entry and the old entry match
  bool bIsEqual = false;
  ASSERT(pOldEntry->aKey()->bJuxtaCompare(JC_SAME, xOldKeyWrk.aKey(), m_pIndex->pIdxColVec(), bIsEqual));
  
  // create a newEntry at the same location as the foundEntry
  Entry* pNewEntry = _pEntry(pOldEntry);
  
  // get the size of the old key
  u8 uOldKeySize = xOldKeyWrk.nWspUsed();
  
  // get the size of the new key
  u8 uNewKeySize = xNewKeyWrk.nWspUsed();
  
  i8 iSize = pOldEntry->nSizeEntry();
  
  // the old key is smaller than the new key
  i8 nSizeDifference = uOldKeySize - uNewKeySize;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create an EntWrk from the old entry, with offsets et all
  EntWrk xEntWrk(pOldEntry);
  xEntWrk.vAddKey(xNewKeyWrk);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // we need to move left nSizeDifference bytes, making room for the bigger key
  
  // calculate the pDst address
  void* pDst = pAdd<void*>(pOldEntry, uNewKeySize);
  
  // move left
  vMoveLeft(pDst, nSizeDifference);
  m_xWsp.vGive(nSizeDifference);
  
  // calculate size of new entry
  i8 nBytes =  iSize - (uOldKeySize - uNewKeySize);
  
  // create a new entry at pNewEntry using EntWrk
  new (pNewEntry) Entry(CN_ENTRY, nBytes, xEntWrk.nOffsets());
  pNewEntry->vSetEntry(xEntWrk);
  ASSERT(pNewEntry->nSizeEntry() == nBytes);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // we have replaced the first entry in this buffer
  // hence we need to fix the right offsets in the head
  nSizeDifference = uNewKeySize - uOldKeySize;
  ASSERT( nSizeDifference < 0 );
  vAdjustOffsets(pHead(), nSizeDifference);
}

/*----------------------------------------------------------------------------*/
/* fix offsets of the entries in the prior vector to account for new entry    */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vFixPriorOffset(EntryVec* pPriorVec, Entry* aNewEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  //vTrace("PRIOR VECTOR BEFORE FIXING\n"_Str);
  //vDumpPrior(xPriorVec);
  //vTrace("\n"_Str);
  
  // get entry size
  i8 nNewEntryBytes = aNewEntry->nSizeEntry();
  
  // iterate thru each possible offset
  for (on nOffsetNo; nOffsetNo.bInRange(); nOffsetNo++) {
    
    //on nOffsetNo;
    //nOffsetNo= uOffsetNo;
    
    // get prior entry being fixed
    ln nLevelNo = nOffsetNo.nToLevelNo();
    Entry* aPriorEntry = (*pPriorVec)[nLevelNo];
    
    // compute and set corrected offset
    i8 iPriorsRightOffset = aPriorEntry->iRightOffset(nOffsetNo);
    
    // ignore cases where no right offset
    if (C_NIL_OFFSET == iPriorsRightOffset)
      continue;
    
    // correct the right offset by adding in the size of the new entry
    i8 iCorrectedRightOffset = iPriorsRightOffset + nNewEntryBytes;
    aPriorEntry->vRightOffset(nOffsetNo,iCorrectedRightOffset);
    
  } // for
  
  //vTrace("PRIOR VECTOR AFTER FIXING\n"_Str);
  //vDumpPrior(xPriorVec);
  //vTrace("\n"_Str);
}

/*----------------------------------------------------------------------------*/
/* get an entry by number                                                     */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::pGetEntry(i8 iEntryNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // start with the head
  Entry* pSplitEntry = pHead();
  
  i8 iEntries = 0;
  while (pSplitEntry) {
    
    iEntries++;
    
    // found split point
    if (iEntries == iEntryNo)
      break;
    
    // get next entry
    pSplitEntry = this->pNextEntry(pSplitEntry);
  }
  
  return pSplitEntry;
}

/*----------------------------------------------------------------------------*/
/* move subset of entries left (thereby removing an entry)                    */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vMoveLeft(void* aDst, i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  /*
   
   we are in the process of removing entry E from between A..D and F..Z
   
   1) we compute the source address
   2) we move the F..Z left
   3) we give space back into wsp free space
   
   aDst --+   +-- aSrc
   v   v
   ------------------------------------------+
   | A..D | E | F..Z |       free         |
   ------------------------------------------+
   ------------------------------------------+
   | A..D | F..Z |         free           |
   ------------------------------------------+
   
   aDst is the first byte of entry E
   nBytes is the size of entry E
   aSrc will be the first byte of F..Z
   
   note that MemLib::vMove() is used because the move/copy overlaps
   
   */
  
  // compute the source address
  void* aSrc = pAdd<void*>(aDst,nBytes);
  
  // bytes to copy
  i8 nBytesToCopy = nSub(m_xWsp.aLatter(), aSrc);
  
  // move the bytes left
  MemLib::vMove(aDst,aSrc,nBytesToCopy,CN_ENTRYBUFFER);
  
  // release space back to wsp
  //m_xWsp.vGive(nBytes);
  
  // for the moment we'll zero the bytes put back into the free area
  //void* aLatter = m_xWsp.aLatter();
  //MemLib::vZero(aLatter,nBytes);
}

/*----------------------------------------------------------------------------*/
/* move subset of entries right (thereby making room for a new entry)         */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vMoveRight(void* aSrc, i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  /*
   
   we are in the process of inserting entry E between A..D and F..Z
   
   1) the caller has checked there is sufficient free space in wsp
   2) we first take space for E in wsp
   3) compute the destination address
   3) then move F..Z right by bytes alloted for E
   
   aSrc --+      +-- aDst
   v      v
   ------------------------------------------+
   | A..D | F..Z | alloted | free         |
   ------------------------------------------+
   ------------------------------------------+
   | A..D | alloted | F..Z | free         |
   ------------------------------------------+
   
   aSrc is the first byte where E is to be inserted (i.e. first byte of F)
   nBytes is the size of E
   aDst will be the first byte of alloted space
   
   note that MemLib::vMove() is used because the move/copy overlaps
   
   */
  
  // the caller has checked there is sufficient space
  //ASSERT(m_xWsp.nFree() >= nBytes);
  
  // allot space for entry E in wsp
  //m_xWsp.vTake(nBytes);
  
  // compute the destination address
  void* aDst = pAdd<void*>(aSrc,nBytes);
  
  // bytes to copy
  i8 nBytesToCopy = nSub(m_xWsp.aLatter(), aSrc) - nBytes;
  
  if (nBytesToCopy == 0)
    return;

  // move F..Z right
  MemLib::vMove(aDst,aSrc,nBytesToCopy,CN_ENTRYBUFFER);
  
  // for the moment we'll zero the bytes of entry E
  _memZero_(aSrc,nBytes,CN_ENTRYBUFFER);
}

/*----------------------------------------------------------------------------*/
/* scan thru skip list and positition to entry (optionally set prior vector)  */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::pPosition(Key* aGivenKey, eJC eJuxtaCompare, EntryVec* pPriorVec, bool& bIsEqual)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  /*
   * positioning is based on eJuxtaCompare
   *   JC_BEFORE: never called
   *   JC_SAME:   scan for entry which is the same as given key
   *   JC_AFTER:  scan for entry which the given key would be inserted after
   */
  
  ASSERT(JC_SAME == eJuxtaCompare || JC_AFTER == eJuxtaCompare);
  
  bool bIsJuxta;
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  // start scanning from head entry and topmost level
  ln nScanLevel;
  nScanLevel = gnIndexLevels - 1;
  
  //aGivenKey->vShow(m_pIndex->pIdxColVec());
  //vTrace("\n"_Str);
  //vShow();
  
  Entry* pScanEntry_sl = pHead();
  Entry* pNextEntry_sl = nullptr;
  //Entry* pMoreEntry_sl = nullptr;
  
  do {
    
    pNextEntry_sl = pRightEntry(pScanEntry_sl,nScanLevel);
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // inner loop: scan right a number of entries at current scan level
    //             until we find a key that is same/after given key
    
    
    while (pNextEntry_sl) {
      
      // get next key after scan key
      Key* aNextKey = pNextEntry_sl->aKey();
      
      // trace:
      //aGivenKey->vShow(m_pIndex->pIdxColVec());
      //aNextKey->vShow(m_pIndex->pIdxColVec());
      
      // juxta-compare given key with next key at this level
      bIsJuxta = aGivenKey->bJuxtaCompare(eJuxtaCompare,aNextKey,pIdxColVec, bIsEqual);
      
      //vTrace(" %s\n"_Str,bAfter ? "after" : "before");
      
      // if given key is not same/after next key then break to outer loop
      if (! bIsJuxta)
        break;
      
      pScanEntry_sl = pNextEntry_sl;
      pNextEntry_sl = pRightEntry(pNextEntry_sl,nScanLevel);;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // outer loop: having gone right as far as we can, go down one level
    //             note that aNextEntry may be null at this point
    
    // opt record scan entry as an insert-after revision point at this level
    if (pPriorVec)
      (*pPriorVec)[nScanLevel] = pScanEntry_sl;
    
    // now go down one level
    nScanLevel--;
    
    
    // and repeat till we have scanned down thru all levels
  } while (*nScanLevel >= 0);
  
  // JC_SAME : return the entry which is same as given key
  // JC_AFTER: return the entry which the given key would be inserted after
  return pScanEntry_sl;
}

/*----------------------------------------------------------------------------*/
/* get the prior vector of given entry                                        */
/*----------------------------------------------------------------------------*/
void EntryBuffer::vPriorVec(Entry* pEntry, EntryVec* pPriorVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  ln nLevel;
  
  // set all entries to null
  Entry* pHeadEntry = pHead();
  for (nLevel = 0_on; *nLevel < gnIndexLevels; nLevel++)
    (*pPriorVec)[nLevel] = nullptr;
  
  // scan till we find the entry
  for (Entry* pScan = pHead(); pScan; pScan = pNextEntry(pScan)) {
    
    if (pScan == pEntry)
      return;
    
    for (on nOffset; *nOffset < pScan->nOffsets(); nOffset++) {
      nLevel = nOffset.nToLevelNo();
      if (C_NIL_OFFSET == pScan->iRightOffset(nOffset) ||
          pRightEntry(pScan,nLevel) >= pEntry)
        (*pPriorVec)[nLevel] = pScan;
    }
    
  }
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* get right entry at given level (returns null if no right)                  */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer::pRightEntry(Entry* pEntry, ln nLevelNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  Entry* pRightEntry;
  
  /*
   
   1) entries are in key order within a given buffer
   2) all entries have an implied level 0
   2) this means the right entry for level 0 for an entry is
   the next entry in the buffer
   3) entries may also have level 1 or level 1 & level 2 right entries
   
   pEntry is the entry we want a right entry from
   iLevel is the level we want the right entry of
   
   */
  
  // the caller must request a valid right entry of pEntry
  ASSERT(nLevelNo >= 0_ln && *nLevelNo < gnIndexLevels);
  
  if (nLevelNo == 0_ln) {
    // for level 0, the right entry is the next entry
    pRightEntry = pNextEntry(pEntry);
  }
  else {
    
    on nOffsetNo = nLevelNo.nToOffsetNo();
    //ASSERT(nOffsetNo >= 0_on && *nOffsetNo < pEntry->nOffsets());
    
    // for levels 1 & 2 the right entry is m_xWsp + rightOffset[iRight]
    i8 iBytes = pEntry->nSizeEntry();
    i8 iRightOffset = pEntry->iRightOffset(nOffsetNo);
    if (C_NIL_OFFSET == iRightOffset)
      pRightEntry = nullptr;
    else
      pRightEntry = pAdd<Entry*>(pEntry,iRightOffset+iBytes);
  }
  
  ASSERT( ! pRightEntry ||
         ( pRightEntry > pHead() &&
          pRightEntry < _pEntry(m_xWsp.aLatter()) ) );
  
  return pRightEntry;
}

/*----------------------------------------------------------------------------*/
/* converts given entry to absolute offset to end of entry                    */
/*----------------------------------------------------------------------------*/
i8 EntryBuffer::iToEndOffsetAbs(Entry* pEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 iOffset = nSub(pEntry,m_xWsp.aArea());
  
  ASSERT(iOffset >= 0 && iOffset < m_xWsp.nSize());
  
  return iOffset + pEntry->nSizeEntry();
}

/*----------------------------------------------------------------------------*/
/* calculate the absolute offset within workspace for given entry             */
/*----------------------------------------------------------------------------*/
i8 EntryBuffer::iToOffsetAbs(Entry* pEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 iOffset = nSub(pEntry,m_xWsp.aArea());
  
  ASSERT(iOffset >= 0 && iOffset < m_xWsp.nSize());
  
  return iOffset;
}

/*----------------------------------------------------------------------------*/
/* calculate the absolute right offset for a given entry and offset no        */
/*----------------------------------------------------------------------------*/
i8 EntryBuffer::iToRightOffsetAbs(Entry* pEntry, on nOffsetNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_); 
  
  i8 iEntryOffsetAbs = iToOffsetAbs(pEntry);
  i8 iRightOffset = pEntry->iRightOffset(nOffsetNo);
  i8 iEntrySize = pEntry->nSizeEntry();
  
  if (iRightOffset == C_NIL_OFFSET)
    iRightOffset = 0;
  
  i8 iRightOffsetAbs = iEntryOffsetAbs + iRightOffset + iEntrySize;
  
  return iRightOffsetAbs;
}

/*----------------------------------------------------------------------------*/
/* calculate the relative offset between two entries                          */
/*----------------------------------------------------------------------------*/
u8 EntryBuffer::uOffsetBetween(Entry* pEntryA, Entry* pEntryB)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  u8 uOffset = 0;
  
  u8 uEntryOffsetA = iToOffsetAbs(pEntryA);
  u8 uEntryEndOffsetA = iToEndOffsetAbs(pEntryA);
  
  u8 uEntryOffsetB = iToOffsetAbs(pEntryB);
  u8 uEntryEndOffsetB = iToEndOffsetAbs(pEntryB);
  
  if (uEntryOffsetA < uEntryOffsetB)
    uOffset = uEntryOffsetB - uEntryEndOffsetA;
  else
    uOffset = uEntryOffsetA - uEntryEndOffsetB;
  
  return uOffset;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
