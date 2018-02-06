/*============================================================================*/
/* EntryBuffer.cpp -- Entry Buffer                                            */
/*============================================================================*/

#include "DBobjects.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
EntryBuffer_v1* DBobjects::_pEntryBuffer_v1(void* aAddr)
{
  EntryBuffer_v1* pEntryBuffer = reinterpret_cast<EntryBuffer_v1*>(aAddr);
  
  RASSERT( !pEntryBuffer || CN_ENTRYBUFFER == pEntryBuffer->eClass());
  
  return pEntryBuffer;
}

/*----------------------------------------------------------------------------*/
/* fetch a new buffer from buffer pool                                        */
/*----------------------------------------------------------------------------*/
void* EntryBuffer_v1::operator new(size_t iSize)
{
  RASSERT(iSize);
  Buffer* pBuffer = (Buffer*)Heap::pAllocate(CN_ENTRYBUFFER);
  
  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* fetch a new buffer from buffer pool                                        */
/*----------------------------------------------------------------------------*/
void* EntryBuffer_v1::operator new(size_t iSize, void* aAddr)
{
  RASSERT(iSize);
  
  return aAddr;
}

/*----------------------------------------------------------------------------*/
/* return a row buffer to the buffer pool                                     */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v1::operator delete(void* pRowBufferAddress)
{
  Buffer* pBuffer = reinterpret_cast<Buffer*>(pRowBufferAddress);
  
  Heap::vFree(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EntryBuffer_v1::EntryBuffer_v1(void)
: ItemBuffer(CN_ENTRYBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_eDurability   = BD_NONE;
  m_bIsLast       = false;
  
  m_pHigherLevel  = nullptr;
  m_pNextBuffer   = nullptr;
  m_pPrevBuffer   = nullptr;

  m_pIndex        = nullptr;
  m_nBufferLevel  = 0;
  m_nPartitionNo  = -1;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  _checkInitialised_(CN_ENTRYBUFFER);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EntryBuffer_v1::EntryBuffer_v1(i8 iDataBytes)
: ItemBuffer(CN_ENTRYBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_xWsp.vUsed(iDataBytes);
  
  m_eDurability   = BD_NONE;
  m_bIsLast       = false;
  
  m_pHigherLevel  = nullptr;
  m_pNextBuffer   = nullptr;
  m_pPrevBuffer   = nullptr;
  
  m_pIndex        = nullptr;
  m_nBufferLevel  = 0;
  m_nPartitionNo  = -1;
  
  _checkInitialised_(CN_ENTRYBUFFER);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EntryBuffer_v1::EntryBuffer_v1(Index* pIndex, i8 nBufferLevel, i8 nPartitionNo)
: ItemBuffer(CN_ENTRYBUFFER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_eDurability  = BD_NONE;
  m_bIsLast      = false;
  
  m_pHigherLevel = nullptr;
  m_pNextBuffer  = nullptr;
  m_pPrevBuffer  = nullptr;
  
  m_pIndex       = pIndex;
  m_nBufferLevel = nBufferLevel;
  m_nPartitionNo = nPartitionNo;
  
  _checkInitialised_(CN_ENTRYBUFFER);
  
  // new the head
  vAllotHeadEntry();
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EntryBuffer_v1::EntryBuffer_v1(eCN eClassNo)
: ItemBuffer(eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClassNo,_FOOTER_);
  
  m_eDurability  = BD_TEMPORARY;
  m_bIsLast      = false;
  
  m_pHigherLevel = nullptr;
  m_pNextBuffer  = nullptr;
  m_pPrevBuffer  = nullptr;
  
  m_pIndex       = nullptr;
  m_nBufferLevel = 0;
  m_nPartitionNo = -1;
  
  _checkInitialised_(eClassNo);
  
  _postcondition_(_ADDRESS_,_MAGIC_,eClassNo,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
EntryBuffer_v1::~EntryBuffer_v1(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump entry buffer                                                          */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v1::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  ASSERT(eClass == CN_ENTRYBUFFER);
  ASSERT(pObject->eClass() == CN_ENTRYBUFFER);
  
  EntryBuffer_v1* pEntryBuffer = reinterpret_cast<EntryBuffer_v1*>(pObject);
  
  vTrace("durability %s partition %lld rows %lld used %lld free %lld prevBuffer %p nextBuffer %p\n"_Stz,
         pEntryBuffer->m_nPartitionNo,
         aBD[pEntryBuffer->m_eDurability],
         pEntryBuffer->m_nItems,
         pEntryBuffer->m_xWsp.nUsed(),
         pEntryBuffer->m_xWsp.nFree(),
         pEntryBuffer->m_pPrevBuffer,
         pEntryBuffer->m_pNextBuffer);
}

/*----------------------------------------------------------------------------*/
/*-- Member Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get durability                                                             */
/*----------------------------------------------------------------------------*/
eBD EntryBuffer_v1::eDurability(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_eDurability;
}

/*----------------------------------------------------------------------------*/
/* set durability                                                             */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v1::vDurability(eBD eDurability)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_eDurability = eDurability;
}

/*----------------------------------------------------------------------------*/
/* set buffer is last                                                         */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v1::vIsLast(bool bIsLast)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_bIsLast = bIsLast;
}

/*----------------------------------------------------------------------------*/
/* get if buffer is last                                                      */
/*----------------------------------------------------------------------------*/
bool EntryBuffer_v1::bIsLast(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_bIsLast;
}




/*----------------------------------------------------------------------------*/
/* get the higher level index buffer                                          */
/*----------------------------------------------------------------------------*/
EntryBuffer_v1* EntryBuffer_v1::pHigherLevel(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return (EntryBuffer_v1*)m_pHigherLevel;
}

/*----------------------------------------------------------------------------*/
/* set the higher level index buffer                                          */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v1::vHigherLevel(EntryBuffer_v1* pHigherLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  //ASSERT( ! m_pHigherLevel);
  
  m_pHigherLevel = pHigherLevel;
}

/*----------------------------------------------------------------------------*/
/* get next buffer                                                            */
/*----------------------------------------------------------------------------*/
EntryBuffer_v1* EntryBuffer_v1::pNextBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  return m_pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* set next buffer                                                            */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v1::vNextBuffer(EntryBuffer_v1* pNextBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_pNextBuffer = pNextBuffer;
}

/*----------------------------------------------------------------------------*/
/* get prev buffer                                                            */
/*----------------------------------------------------------------------------*/
EntryBuffer_v1* EntryBuffer_v1::pPrevBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_pPrevBuffer;
}

/*----------------------------------------------------------------------------*/
/* set prev buffer                                                            */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v1::vPrevBuffer(EntryBuffer_v1* pPrevBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_pPrevBuffer = pPrevBuffer;
}

/*----------------------------------------------------------------------------*/
/* get the buffer level                                                       */
/*----------------------------------------------------------------------------*/
i8 EntryBuffer_v1::nBufferLevel(void)
{
  return m_nBufferLevel;
}

/*----------------------------------------------------------------------------*/
/* set the index of the index buffer                                          */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v1::vIndex(Index* pIndex)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_pIndex = pIndex;
}

/*----------------------------------------------------------------------------*/
/* get the index of the index buffer                                          */
/*----------------------------------------------------------------------------*/
Index* EntryBuffer_v1::pIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_pIndex;
}

/*----------------------------------------------------------------------------*/
/* set partition no                                                           */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v1::vPartitionNo(i8 nPartitionNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_nPartitionNo = nPartitionNo;
}

/*----------------------------------------------------------------------------*/
/* get partition no                                                           */
/*----------------------------------------------------------------------------*/
i8 EntryBuffer_v1::nPartitionNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return m_nPartitionNo;
}

/*----------------------------------------------------------------------------*/
/*-- Complex Functions -------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot an entry in this buffer                                              */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer_v1::pAllotEntry(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,_FOOTER_);
  
  // nBytes takes care of data, offsets and key
  i8 nBytesRequired = Entry::nBytesRequired(0,0,nBytes); //# change to 1 offset
  
  Entry* pEntry = reinterpret_cast<Entry*>(pAllotItem(nBytesRequired));
  if ( ! pEntry )
    return nullptr;
  
  new(pEntry) Entry(CN_ENTRY,nBytesRequired,0);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return pEntry;
}

/*----------------------------------------------------------------------------*/
/* allot an entry in this buffer                                 TEMPORARY!!! */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer_v1::pAllotEntryBytes(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_);
  
  Entry* pEntry = reinterpret_cast<Entry*>(pAllotItem(nBytes));
  if ( ! pEntry )
    return nullptr;
  
  new(pEntry) Entry(CN_ENTRY,nBytes,0);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return pEntry;
}

/*----------------------------------------------------------------------------*/
/* analyze                                                                    */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v1::vAnalyze(s48K& sAnalysis, bool bDetailed, i8& iBuffers, i8& iEntries)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  if (m_pIndex->bIsHidden()) {
    
    ASSERT(m_nBufferLevel == 0);
    EntryBuffer_v1* pNextBuffer = this;
    
    do {
      ASSERT(pNextBuffer->nBufferLevel() == 0);
      
      iBuffers++;
      iEntries += (pNextBuffer->m_nItems);
      
      if (bDetailed)
      sAnalysis.vCatSprint("     buffer %5lld (L%lld) : %lld entries\n"_Stz,
                           iBuffers,
                           pNextBuffer->m_nBufferLevel,
                           pNextBuffer->m_nItems);

      EntryBuffer_v1* pThisBuffer = pNextBuffer;
      pNextBuffer = pNextBuffer->pNextBuffer();
    } while (pNextBuffer);
    
    return;
  }

  iBuffers++;
  iEntries += m_nItems;
  
  if (bDetailed)
    sAnalysis.vCatSprint("     buffer %5lld (L%lld) : %lld entries\n"_Stz,
                         iBuffers,
                         m_nBufferLevel,
                         m_nItems);
  
  // recursively dump the lower level buffers
  if (m_nBufferLevel != 0) {
    
    for (Entry* pEntry = pNextEntry(pHeadEntry()); pEntry != nullptr;
         pEntry = pNextEntry(pEntry)) {
      
      // get the index buffer from the entry
      EntryBuffer* pEntryBuffer = pEntry->pEntryBuffer();;
      
      ASSERT(pEntryBuffer);
      ASSERT(pEntryBuffer->eClass() == CN_ENTRYBUFFER);
      
      pEntryBuffer->vAnalyze(sAnalysis, bDetailed, iBuffers, iEntries);
      
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* append new entry into this index buffer                                    */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer_v1::pAppendEntry(EntWrk_v1& xEntWrk,EntryVec& xPriorVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append the entry
  
  // determine the number of bytes required for entry
  i8 nEntryBytes = Entry::nBytesRequired_v1(xEntWrk);
  
  // allot space for entry at end of buffer
  Entry* pNewEntry = pAllotEntryBytes(nEntryBytes);
  
  // set the entry
  pNewEntry->vSetEntry_v1(xEntWrk);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // revise prior offsets
  
  // step thru the offsets of the new entry
  for (on n_OffsetNo = pNewEntry->nHighestOffset();
       n_OffsetNo.bInRange();
       n_OffsetNo--) {
    
    // get prior entry for offset level
    ln nLevelNo = n_OffsetNo.nToLevelNo();
    Entry* aPriorEntry = xPriorVec[nLevelNo];
    
    // calculate and set offset from prior entry to new entry
    i8 iOffset = uOffsetBetween(pNewEntry,aPriorEntry);
    aPriorEntry->vRightOffset(n_OffsetNo,iOffset);
    
    // revise the prior vector for level
    xPriorVec[nLevelNo] = pNewEntry;
  }
  
  return pNewEntry;
}

/*----------------------------------------------------------------------------*/
/* append an entry for {key,id} into an index buffer at level 0               */
/*----------------------------------------------------------------------------*/
bool EntryBuffer_v1::bAppendEntry(Entry* pRowEntry, Entry** ppEntry)     //# critical
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  Entry* pEntry;
  
  ASSERT(nBufferLevel() == 0);
  
  KeyWrk xKeyWrk;
  ValWrk xValWrk;
  
  //----------------------------------------------------------------------------
  // form the rowId
  bn nBufferNo = this->nBufferNo();
  i8 nRowNo = nItems();
  Id rowId(*nBufferNo, nRowNo);
  
  //----------------------------------------------------------------------------
  // form the key
  xValWrk = nRowNo;
  Column* pColumn = m_pIndex->pIdxColVec()->pGet(0);
  ASSERT(pColumn->iColumnNo() == -1);
  xKeyWrk.vAddDatum(xValWrk, pColumn);
  
  //----------------------------------------------------------------------------
  // add the id, txId and key to EntWrk
  i8 iTxId = pRowEntry->iTxId();
  EntWrk_v1 xEntWrk(rowId, iTxId, xKeyWrk);
  
  //----------------------------------------------------------------------------
  // add the row data to the EntWrk
  char* pRow = pRowEntry->aRowData();
  i8 iLength = pRowEntry->nRowSize();
  
  xEntWrk.vData((Stz*)pRow, iLength);
  //xEntWrk.vShow(m_pIndex->pIdxColVec());
  
  //----------------------------------------------------------------------------
  // check if current index buffer has enough space
  i8 nEntryBytes = Entry::nBytesRequired_v1(xEntWrk);
  
  if ( ! bHasSpace(nEntryBytes) ) {
    return false;
  }
  else {
    
    // insert entry into current index buffer
    //pEntry = pInsert(xEntWrk);
    
    pEntry = pAllotEntryBytes(nEntryBytes); //# why + sizeof(i8) ?
    ASSERT(pEntry)
    
    //----------------------------------------------------------------------------
    // add the id and key to EntWrk
    pEntry->vSetEntry_v1(xEntWrk);
    
    *ppEntry = pEntry;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* append an entry for {key,id} into an index buffer at level 0               */
/*----------------------------------------------------------------------------*/
bool EntryBuffer_v1::bAppendEntry(i8 iTxId, Stz* pRow, i8 iLength)     //# critical
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  Entry* pEntry;
  
  ASSERT(nBufferLevel() == 0);
  
  KeyWrk xKeyWrk;
  ValWrk xValWrk;
  
  //----------------------------------------------------------------------------
  // form the rowId
  bn nBufferNo = this->nBufferNo();
  i8 nRowNo = nItems();
  Id rowId(*nBufferNo, nRowNo);
  
  //----------------------------------------------------------------------------
  // form the key
  xValWrk = nRowNo;
  Column* pColumn = m_pIndex->pIdxColVec()->pGet(0);
  ASSERT(pColumn->iColumnNo() == -1);
  xKeyWrk.vAddDatum(xValWrk, pColumn);
  
  //----------------------------------------------------------------------------
  // add the id, txId and key to EntWrk
  EntWrk_v1 xEntWrk(rowId, iTxId, xKeyWrk);
  
  //----------------------------------------------------------------------------
  // add the row data to the EntWrk
  xEntWrk.vData(pRow, iLength);
  //xEntWrk.vShow(m_pIndex->pIdxColVec());
  
  //----------------------------------------------------------------------------
  // check if current index buffer has enough space
  i8 nEntryBytes = Entry::nBytesRequired(xEntWrk);
  
  if ( ! bHasSpace(nEntryBytes) ) {
    return false;
  }
  else {
    
    // insert entry into current index buffer
    //pEntry = pInsert(xEntWrk);
    
    Entry* pEntry = pAllotEntryBytes(nEntryBytes); //# why + sizeof(i8) ?
    ASSERT(pEntry)
    
    //----------------------------------------------------------------------------
    // add the id and key to EntWrk
    pEntry->vSetEntry(xEntWrk);
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* append a row into an index buffer                                          */
/*----------------------------------------------------------------------------*/
bool EntryBuffer_v1::bAppendRow(i8 iTxId, Stz* pRow, i8 iLength)     //# critical
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  Entry* pEntry;
  
  ASSERT(nBufferLevel() == 0);
  
  KeyWrk xKeyWrk;
  ValWrk xValWrk;
  
  //----------------------------------------------------------------------------
  // form the rowId
  bn nBufferNo = this->nBufferNo();
  i8 nRowNo = nItems();
  Id rowId(*nBufferNo, nRowNo);
  
  //----------------------------------------------------------------------------
  // form the key
  xValWrk = nRowNo;
  Column* pColumn = m_pIndex->pIdxColVec()->pGet(0);
  ASSERT(pColumn->iColumnNo() == -1);
  xKeyWrk.vAddDatum(xValWrk, pColumn);
  
  //----------------------------------------------------------------------------
  // add the id, txId and key to EntWrk
  EntWrk_v1 xEntWrk(rowId, iTxId, xKeyWrk);
  
  //----------------------------------------------------------------------------
  // add the row data to the EntWrk
  xEntWrk.vData(pRow, iLength);
  xEntWrk.vTxId(iTxId);
  //xEntWrk.vShow(m_pIndex->pIdxColVec());
  
  //----------------------------------------------------------------------------
  // check if current index buffer has enough space
  i8 nEntryBytes = Entry::nBytesRequired(xEntWrk);
  
  if ( ! bHasSpace(nEntryBytes) ) {
    return false;
  }
  else {
    
    // insert entry into current index buffer
    //pEntry = pInsert(xEntWrk);
    
    pEntry = pAllotEntryBytes(nEntryBytes); //# why + sizeof(i8) ?
    ASSERT(pEntry)
    
    //----------------------------------------------------------------------------
    // add the id and key to EntWrk
    pEntry->vSetEntry(xEntWrk);
  }
  
  //pEntry->vShow(pEntry->nOffsets(), m_pIndex);
  return true;
}

/*----------------------------------------------------------------------------*/
/* return no of index buffers under this buffer                               */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v1::vBufferCount(i8& nBuffers)
{
  EntryBuffer_v1* pNextBuffer = nullptr;
  
  if (m_pIndex->bIsHidden()) {
    ASSERT(m_nBufferLevel == 0);
    pNextBuffer = this->pNextBuffer();
    nBuffers++;
    
    while (pNextBuffer) {
      ASSERT(pNextBuffer->nBufferLevel() == 0);
      
      EntryBuffer_v1* pThisBuffer = pNextBuffer;
      pNextBuffer = pNextBuffer->pNextBuffer();
      nBuffers++;
    }
    
  }
  else {
    // count this buffer
    nBuffers++;
    
    // recursively dump the lower level buffers
    if (m_nBufferLevel != 0) {
      
      for (Entry* pEntry = pNextEntry(pHeadEntry()); pEntry != nullptr;
           pEntry = pNextEntry(pEntry)) {
        
        // get the index buffer from the entry
        pNextBuffer = pEntry->pEntryBuffer_v1();;
        
        ASSERT(pNextBuffer);
        ASSERT(pNextBuffer->eClass() == CN_ENTRYBUFFER);
        
        pNextBuffer->vBufferCount(nBuffers);
        
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/* get decile full of the buffer                                              */
/*----------------------------------------------------------------------------*/
i8 EntryBuffer_v1::iDecile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 iDecile;
  
  if (gpAttachMap->m_nIndexEntries == 0)
    iDecile = IntLib::iDecile(m_xWsp.nUsed(), C_SIZE_POOL_BUFFER);
  else
    iDecile = IntLib::iDecile(m_nItems, gpAttachMap->m_nIndexEntries);
  
  
  return iDecile;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v1::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // show this buffer
  vShow();
  
  // recursively dump the lower level buffers
  if (m_nBufferLevel != 0) {
    
    for (Entry* pEntry = pNextEntry(pHeadEntry());
         pEntry != nullptr;
         pEntry = pNextEntry(pEntry)) {
      
      // get the index buffer from the entry
      EntryBuffer_v1* pEntryBuffer = pEntry->pEntryBuffer_v1();
      
      ASSERT(pEntryBuffer);
      ASSERT(pEntryBuffer->eClass() == CN_ENTRYBUFFER);
      
      pEntryBuffer->vDump();
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* get first entry (after the head) of skip list in this index buffer         */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer_v1::pFirstEntry(void)
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
bool EntryBuffer_v1::bHasSpace(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 nFree = m_xWsp.nFree();
  
  if (gpAttachMap->m_nIndexEntries > 0) {
    // has space calculation based on no of entries
    if (m_nItems >= gpAttachMap->m_nIndexEntries)
      return false;
    else
      return true;
  }
  else {
    // has space calculation based on free space
    if (nFree < nBytes)
      return false;
    else
      return true;
  }
}

/*----------------------------------------------------------------------------*/
/* get head entry of skip list in this index buffer                           */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer_v1::pHeadEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // the head entry will have been alloted during constructor
  ASSERT(m_xWsp.nUsed());
  
  // the head entry is the first entry in wsp
  Entry* pHead = _pEntry(m_xWsp.aArea());
  
  return pHead;
}

/*----------------------------------------------------------------------------*/
/* insert new entry into this index buffer                                    */
/*----------------------------------------------------------------------------*/
bool EntryBuffer_v1::bInsert(EntWrk_v1& xEntWrk)                        //# critical
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  //vTrace("Inserting key:\n"_Stz);
  //xKeyWrk.vShow();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // insert a new entry into the skip list
  
  // find the entry after which the new entry.key will be inserted
  EntryVec xPriorVec;
  bool bIsEqual = false;
  i8 nKeyCols = m_pIndex->pIdxColVec()->nItems();
  vPositionPrior(xEntWrk.aKey(), &xPriorVec);
  
  Entry* pAfterEntry = xPriorVec[0_ln];
  
//  if (m_pIndex->bIsUnique() and bIsEqual) { //# note the and !!!
//    vTrace("Warning: Duplicate key in unique index %s "_Stz, m_pIndex->aIndexName());
//    xEntWrk.aKey()->vShow(m_pIndex->pIdxColVec());
//    vTrace("\n"_Stz);
//    return false;
//  }
  
  // create a new entry after the prior entry and set it
  //EntWrk xEntWrk(xId,xKeyWrk);
  i8 nEntryBytes = Entry::nBytesRequired_v1(xEntWrk);
  Entry* pNewEntry = pAllotEntryAfter(nEntryBytes, xEntWrk.nOffsets(), pAfterEntry);
  pNewEntry->vSetEntry_v1(xEntWrk);
  
  //vTrace("\n\n---------INDEX BUFFER AFTER MOVING --------------\n"_Stz);
  //vShow();
  //vTrace("-----------------------\n\n"_Stz);
  
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
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* insert an entry for {key,id} into an index buffer                          */
/*----------------------------------------------------------------------------*/
bool EntryBuffer_v1::bInsertEntry(i8 iTxId, Stz* pRow, i8 iLength)     //# critical
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  Entry* pEntry;
  
  ASSERT(nBufferLevel() == 0);
  
  KeyWrk xKeyWrk;
  ValWrk xValWrk;
  
  //----------------------------------------------------------------------------
  // form the rowId
  bn nBufferNo = this->nBufferNo();
  i8 nRowNo = nItems();
  Id rowId(*nBufferNo, nRowNo);
  
  //----------------------------------------------------------------------------
  // form the key
  xValWrk = nRowNo;
  Column* pColumn = m_pIndex->pIdxColVec()->pGet(0);
  ASSERT(pColumn->iColumnNo() == -1);
  xKeyWrk.vAddDatum(xValWrk, pColumn);
  
  //----------------------------------------------------------------------------
  // add the id, txId and key to EntWrk
  EntWrk_v1 xEntWrk(rowId, iTxId, xKeyWrk);
  
  //----------------------------------------------------------------------------
  // add the row data to the EntWrk
  xEntWrk.vData(pRow, iLength);
  //xEntWrk.vShow(m_pIndex->pIdxColVec());
  
  //----------------------------------------------------------------------------
  // check if current index buffer has enough space
  i8 nEntryBytes = Entry::nBytesRequired_v1(xEntWrk);
  
  if ( ! bHasSpace(nEntryBytes) ) {
    return false;
  }
  else {
    
    // insert entry into current index buffer
    bool bInserted = bInsert(xEntWrk);
    ASSERT(bInserted);
    
  }
  //pEntry->vShow(pEntry->nOffsets(), m_pIndex);
  return true;
}

/*----------------------------------------------------------------------------*/
/* insert row into this buffer     (invoked from rowBuffer.appendRecord/append*/
/*----------------------------------------------------------------------------*/
bool EntryBuffer_v1::bInsertRow(EntWrk_v1& xRowWrk, Entry** ppRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 nBytes = Entry::nBytesRequired_v1(xRowWrk);
  
  Entry* pRowEntry = pAllotEntry(nBytes);
  if ( ! pRowEntry )
    return false;
  
  pRowEntry->vSetEntry_v1(xRowWrk);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  *ppRow = pRowEntry;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* insert row into this buffer (invoked from entryBuffer.insertRow)           */
/*----------------------------------------------------------------------------*/
bool EntryBuffer_v1::bInsertRow(Entry* pOldRow, Entry** ppRow)         //# critical
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // create working row
  ValWrk xValWrk;
  
  //----------------------------------------------------------------------------
  // form the rowId
  bn nBufferNo = this->nBufferNo();
  i8 nRowNo = nItems();
  Id rowId(*nBufferNo, nRowNo);
  
  //----------------------------------------------------------------------------
  // form the key
  xValWrk = nRowNo;
  
  KeyWrk xKeyWrk; //# i don't think xKeyWrk gets used?
  Column* pColumn = m_pIndex->pIdxColVec()->pGet(0);
  ASSERT(pColumn->iColumnNo() == -1);
  ASSERT(pColumn->eColumnType() == CT_INTEGER);
  xKeyWrk.vAddDatum(xValWrk, pColumn);
  
  Entry* pRowEntry = pAllotEntry(pOldRow->nRowSize() + sizeof(i8)); //# why + sizeof(i8) ?
  if ( ! pRowEntry )
    return false;
  
  //----------------------------------------------------------------------------
  // add the id and key to EntWrk
  pRowEntry->vSetEntry(rowId, nRowNo, pOldRow);
  
  //pRowEntry->vShow(0, m_pIndex);
  
  *ppRow = pRowEntry;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* create a next buffer                                                       */
/*----------------------------------------------------------------------------*/
EntryBuffer_v1* EntryBuffer_v1::pNewNextBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // create new buffer at the same index level and link the two buffers
  EntryBuffer_v1* pNewBuffer = new EntryBuffer_v1(m_pIndex, m_nBufferLevel, m_nPartitionNo);
  pNewBuffer->m_pNextBuffer = pNextBuffer();
  pNewBuffer->m_pPrevBuffer = this;
  m_pNextBuffer = pNewBuffer;
  
  return pNewBuffer;
}

/*----------------------------------------------------------------------------*/
/* get next entry (eq to right L0) after given entry (null if no next)        */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer_v1::pNextEntry(Entry* pEntry)                           //! tune
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
Entry* EntryBuffer_v1::pPosition(KeyWrk& xKeyWrk, i8 nKeyCols, eJC eJuxtaCompare)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  Entry* pEntry;
  bool bIsEqual = false;
  
  Key* aKey = xKeyWrk.aKey();
  
  pEntry = pPosition(aKey, nKeyCols, eJuxtaCompare, nullptr, bIsEqual, true);
  
  if (pEntry == pHeadEntry())
    pEntry = pNextEntry(pEntry);
  
  return pEntry;
}

/*----------------------------------------------------------------------------*/
/* position at key as per relational operator                                 */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer_v1::pPosition(KeyWrk &xKeyWrk, eRO eRelnOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  Entry* pIndexEntry = pPosition(xKeyWrk, 1, JC_AFTER);
  
  bool bAfter;
  if (pIndexEntry) {
    do {
      bool bIsEqual = false;
      bAfter = pIndexEntry->aKey()->bJuxtaCompare(JC_AFTER, xKeyWrk.aKey(),
                                                  m_pIndex->pIdxColVec(), 1,
                                                  bIsEqual, eRelnOp);
      if (! bAfter) {
        pIndexEntry = pNextEntry(pIndexEntry);
      }
    } while (pIndexEntry && ! bAfter);
  }
  
  return pIndexEntry;
}

/*----------------------------------------------------------------------------*/
/* position at an index buffer at level iLevel starting from this buffer      */
/*----------------------------------------------------------------------------*/
EntryBuffer_v1* EntryBuffer_v1::pPositionLevel(KeyWrk& xKeyWrk, i8 nKeyCols, eJC eJuxtaCompare, i8 iLevel)  //# critical
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  EntryBuffer_v1* pEntryBuffer = this;
  
  i8 iBufferLevel;
  Entry* pEntry;
  
  while (true) {
    iBufferLevel = pEntryBuffer->nBufferLevel();
    
    if (iBufferLevel == iLevel)
      break;
    
    // get the entry
    pEntry = pEntryBuffer->pPosition(xKeyWrk, nKeyCols, eJuxtaCompare);
    ASSERT(pEntry);
    
    // get the index buffer of the entry
    pEntryBuffer = pEntry->pEntryBuffer_v1();
    ASSERT(pEntryBuffer);
    ASSERT(pEntryBuffer->eClass() == CN_ENTRYBUFFER);
  }
  
  return pEntryBuffer;
}

/*----------------------------------------------------------------------------*/
/* set the prior vector to the head entry                                     */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v1::vPriorVecHead(EntryVec& xPriorVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  ln nLevel;
  for (nLevel = 0_on; *nLevel < gpAttachMap->m_nIndexLevels; nLevel++)
    xPriorVec[nLevel] = pHeadEntry();
}

 /*----------------------------------------------------------------------------*/
/* remove existing entry                                                      */
/*----------------------------------------------------------------------------*/
bool EntryBuffer_v1::bRemove(Key* aKey)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* set row count                                                              */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v1::vRowCount(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  m_nItems = 0;
  for (Entry* pRow = pHeadEntry(); pRow; pRow = pNextEntry(pRow))
    m_nItems++;
  
  _postcondition_(_ADDRESS_,_MAGIC_,_FOOTER_,CN_ENTRYBUFFER);
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v1::vShow(void)
{
  s256 sHigherLevel;
  if (m_pHigherLevel)
    StrLib::wToString(((EntryBuffer_v1*)m_pHigherLevel)->m_nBufferLevel, sHigherLevel.aChar());
  else
    sHigherLevel = "..."_Stz;
  
  vTrace("EntryBuffer %p (buffer %lld): %s index %s partition %lld level L%lld HigerLevel %p (L%s) "
         "entries %lld prevBuffer %p nextBuffer %p \n"_Stz,
         this,
         *nBufferNo(),
         aBD[m_eDurability],
         m_pIndex->aIndexName(),
         m_nPartitionNo,
         m_nBufferLevel,
         m_pHigherLevel,
         sHigherLevel.aStr(),
         m_nItems,
         m_pPrevBuffer,
         m_pNextBuffer);
  
  Entry* pHead = pHeadEntry();
  pHead->vShow(iToOffsetAbs(pHead), m_pIndex, false);
  
  for (Entry* pEntry = pFirstEntry(); pEntry != nullptr; pEntry = pNextEntry(pEntry)) {
    vTrace("%8lld: "_Stz, nSub(pEntry, m_xWsp.aArea()));
    pEntry->vShow(iToOffsetAbs(pEntry), m_pIndex, true);
  }
  
  vTrace("\n"_Stz);
  
}

/*----------------------------------------------------------------------------*/
/* split this buffer into two: this original buffer plus a new buffer         */
/*----------------------------------------------------------------------------*/
EntryBuffer_v1* EntryBuffer_v1::pSplitBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  EntryVec xPriorVec;
  
  // get split entry to remain in this buffer and
  // move entry (and those that follow) which will be moved into new buffer
  i8 iSplitAtEntry   = m_nItems/2;
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
  
  //  vTrace("Split %p\n"_Stz,pSplitEntry);
  //  vTrace("Move  %p\n"_Stz,aMoveEntry);
  //  for (on nOffsetNo; nOffsetNo.bInRange(); nOffsetNo++) {
  //    ln nLevel = nOffsetNo.nToLevelNo();
  //    vTrace("%p v %p\n"_Stz,xPrevVec[nLevel],xPriorVec[nLevel]);
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
  EntryBuffer_v1* pNewBuffer = new EntryBuffer_v1(m_pIndex, m_nBufferLevel, m_nPartitionNo);
  pNewBuffer->m_pNextBuffer = pNextBuffer();
  pNewBuffer->m_pPrevBuffer = this;
  m_pNextBuffer = pNewBuffer;
  
  // copy entries from this buffer starting at splitEntry to the new buffer
  m_nItems -= pNewBuffer->iCopyEntries(this, aMoveEntry);
  
  // give copied bytes back to wsp of this original buffer
  i8 nBytesToGive = nSub(m_xWsp.aLatter(), aMoveEntry);
  m_xWsp.vGive(nBytesToGive);
  ASSERT(m_xWsp.aLatter() == pAdd<void*>((void*)pSplitEntry,pSplitEntry->nSizeEntry()));
  
  return pNewBuffer;
}

/*----------------------------------------------------------------------------*/
/* truncate                                                                    */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v1::vTruncate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  EntryBuffer_v1* pNextBuffer;
  
  // hidden index buffers are just one level
  if (m_pIndex->bIsHidden()) {
    ASSERT(m_nBufferLevel == 0);
    pNextBuffer = this->pNextBuffer();
    
    while (pNextBuffer) {
      ASSERT(pNextBuffer->nBufferLevel() == 0);
      
      EntryBuffer_v1* pThisBuffer = pNextBuffer;
      pNextBuffer = pNextBuffer->pNextBuffer();
      delete pThisBuffer;
    }
  }
  else {
    if (m_nBufferLevel != 0) {
      
      // recursively delete the lower level buffers
      for (Entry* pEntry = pNextEntry(pHeadEntry()); pEntry != nullptr;
           pEntry = pNextEntry(pEntry)) {
        
        // get the index buffer from the entry
        pNextBuffer = pEntry->pEntryBuffer_v1();;
        
        ASSERT(pNextBuffer);
        ASSERT(pNextBuffer->eClass() == CN_ENTRYBUFFER);
        pNextBuffer->vTruncate();
      }
    }
  }
  
  delete this;
}

/*----------------------------------------------------------------------------*/
/* verify                                                                     */
/*----------------------------------------------------------------------------*/
bool EntryBuffer_v1::bVerify(void)                                     //# critical
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
  nLevelNo = gpAttachMap->m_nIndexLevels - 1;
  
  do {
    
    // get the head entry of the buffer
    Entry* pEntryOne = pHeadEntry();
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
      if (pEntryOne != pHeadEntry()) {
        
        // KeyB should should be after keyA
        bool bIsEqual = false;
        bool bAfter = pKeyB->bJuxtaAfter(pKeyA,
                                         m_pIndex->pIdxColVec(),
                                         m_pIndex->pIdxColVec()->nItems(),
                                         bIsEqual);
        
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
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* adjust the  entry offsets                                                  */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v1::vAdjustOffsets(Entry* pEntry, i8 nAdjustAmount)   //# critical
{
  for (on nOffsetNo; nOffsetNo < pEntry->nOffsets(); nOffsetNo++) {
    
    //
    i8 iRightOffset = pEntry->iRightOffset(nOffsetNo);
    
    // here we adjust the iRightOffset if iRightOffset is > 0
    if (iRightOffset > 0) {
      if (pEntry != pHeadEntry()) {
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
Entry* EntryBuffer_v1::pAllotEntryAfter(i8 nBytes, i8 nOffsets, Entry* aAfterEntry)  //# critical
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  //# ASSERT(nOffsets >= 1);
  
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

#ifdef __DEBUG_CHECKS
  MemLib::vScribble(pNewEntry,nBytes,CN_ENTRYBUFFER);
#endif
  
  // run the constructor of the new entry
  new (pNewEntry) Entry(CN_ENTRY,nBytes,nOffsets);
  
  // increment the no of entries
  m_nItems++;
  
  return pNewEntry;
}

/*----------------------------------------------------------------------------*/
/* allot head entry (during constructor)                                      */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v1::vAllotHeadEntry(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // must be the first item in wsp
  ASSERT( ! m_xWsp.nUsed() );
  ASSERT( ! m_nItems );
  
  // how many bytes are required
  i8 nBytes = Entry::nBytesRequired(gpAttachMap->m_nIndexOffsets,0,0);
  
  // allot the head
  
  Entry* pHead = pAllotEntryAfter(nBytes,gpAttachMap->m_nIndexOffsets,nullptr);
  
  // initialize the head
  for (on nOffsetNo; nOffsetNo.bInRange(); nOffsetNo++)
    pHead->vRightOffset(nOffsetNo, C_NIL_OFFSET);
}

/*----------------------------------------------------------------------------*/
/* copy entries from fromBuffer starting at fromEntry to this buffer          */
/*----------------------------------------------------------------------------*/
i8 EntryBuffer_v1::iCopyEntries(EntryBuffer_v1* pFromBuffer, Entry* aFromEntry)  //# critical
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
    i8 iTxId = pEntry->iTxId();
    EntWrk_v1 xEntWrk(pEntry->xId(), iTxId, xKeyWrk);
    
    // insert entry into this buffer
    bool bInserted = bInsert(xEntWrk);
    ASSERT(bInserted);
    
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
      EntryBuffer* pEntryBuffer = pEntry->pEntryBuffer();
      
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
void EntryBuffer_v1::vDumpPriorVec(Entry* xPriorVec[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  for (on nOffsetNo; nOffsetNo.bInRange(); nOffsetNo++) {
    i8 iLevel = *nOffsetNo + 1;
    i8 iReviseOffset = xPriorVec[iLevel]->iRightOffset(nOffsetNo);
    vTrace("%p xPriorVec[%lld]->uRightOffset(%lld) = %lld\n"_Stz,
           xPriorVec[iLevel], iLevel, *nOffsetNo, iReviseOffset);
  }
}

/*----------------------------------------------------------------------------*/
/* fix the higher level                                                       */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v1::vFixHigherLevel(Entry* aNewEntry)                 //# critical
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
  Entry* aFirstEntry = pRightEntry(pHeadEntry(), ln::nZeroLevel());
  
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
    
    pHigherLevel->vReplaceKey(xOldKeyWrk, xNewKeyWrk);
    
    //pHigherLevel->vShow();
    ASSERT(pHigherLevel->bVerify());
    pHigherLevel = (EntryBuffer*)pHigherLevel->m_pHigherLevel;
  }
}

/*----------------------------------------------------------------------------*/
/* fix offsets of the entries in the prior vector to account for new entry    */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v1::vFixPriorOffset(EntryVec* pPriorVec, Entry* aNewEntry) //# critical
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  //vTrace("PRIOR VECTOR BEFORE FIXING\n"_Stz);
  //vDumpPrior(xPriorVec);
  //vTrace("\n"_Stz);
  
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
  
  //vTrace("PRIOR VECTOR AFTER FIXING\n"_Stz);
  //vDumpPrior(xPriorVec);
  //vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* get an entry by number                                                     */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer_v1::pGetEntry(i8 iEntryNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // start with the head
  Entry* pSplitEntry = pHeadEntry();
  
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
void EntryBuffer_v1::vMoveLeft(void* aDst, i8 nBytes)                  //# critical
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
void EntryBuffer_v1::vMoveRight(void* aSrc, i8 nBytes)                 //# critical
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
Entry* EntryBuffer_v1::pPosition(Key* aGivenKey, i8 nKeyCols,
                              eJC eJuxtaCompare, EntryVec* pPriorVec,
                              bool& bIsEqual, bool bIsQuery) //# critical
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  /*
   * positioning is based on eJuxtaCompare
   *   JC_BEFORE: never called
   *   JC_SAME:   scan for entry which is the same as given key
   *   JC_AFTER:  scan for entry which the given key would be inserted after
   */
  
  ASSERT(JC_SAME == eJuxtaCompare || JC_AFTER == eJuxtaCompare);
  
  if (bIsQuery)
    int iDebug = 1;
  
  bool bIsJuxta;
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  // start scanning from head entry and topmost level
  ln nScanLevel;
  nScanLevel = gpAttachMap->m_nIndexLevels - 1;
  
  //aGivenKey->vShow(m_pIndex->pIdxColVec());
  //vTrace("\n"_Stz);
  //vShow();
  
  Entry* pScanEntry_sl = pHeadEntry();
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
      bIsJuxta = aGivenKey->bJuxtaCompare(eJuxtaCompare,aNextKey,
                                          pIdxColVec,nKeyCols,bIsEqual);
      
      //vTrace(" %s\n"_Stz,bAfter ? "after" : "before");
      
      // have we found to key during a query?
      if (bIsQuery && bIsEqual) {
        if (0 == nScanLevel)
          return pNextEntry_sl;
        else
          break;
      }
      
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
/* scan thru skip list and positition to entry (optionally set prior vector)  */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v1::vPositionPrior(Key* aFindKey, EntryVec* pPriorVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  
  // start scanning from head entry and topmost level
  ln nScanLevel;
  nScanLevel = gpAttachMap->m_nIndexLevels - 1;
  
  Entry* pScanEntry_sl = pHeadEntry();
  Entry* pNextEntry_sl = nullptr;
  
  bool bIsJuxta;
  bool bIsEqual = false;
  i8 nKeyCols = pIdxColVec->nItems();
  
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
      bIsJuxta = aFindKey->bJuxtaCompare(JC_AFTER,aNextKey,
                                          pIdxColVec,nKeyCols,bIsEqual);
      
      //vTrace(" %s\n"_Stz,bAfter ? "after" : "before");
      
      // if given key is not same/after next key then break to outer loop
      if (! bIsJuxta)
        break;
      
      pScanEntry_sl = pNextEntry_sl;
      pNextEntry_sl = pRightEntry(pNextEntry_sl,nScanLevel);
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

}

/*----------------------------------------------------------------------------*/
/* get the prior vector of given entry                                        */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v1::vPriorVec(Entry* pEntry, EntryVec* pPriorVec)     //# critical
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  ln nLevel;
  
  // set all entries to null
  for (nLevel = 0_on; *nLevel < gpAttachMap->m_nIndexLevels; nLevel++)
    (*pPriorVec)[nLevel] = nullptr;
  
  // scan till we find the entry
  for (Entry* pScan = pHeadEntry(); pScan; pScan = pNextEntry(pScan)) {
    
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
/* replace old key with new key in the index buffer                           */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v1::vReplaceKey(KeyWrk& xOldKeyWrk, KeyWrk& xNewKeyWrk)  //# critical
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  // get the size of the old key
  u8 uOldKeySize = xOldKeyWrk.nWspUsed();
  
  // get the size of the new key
  u8 uNewKeySize = xNewKeyWrk.nWspUsed();
  
  if (uOldKeySize == uNewKeySize) {
    vReplaceKeyEqual(xOldKeyWrk, xNewKeyWrk);
    return;
  }
  
  if (uOldKeySize > uNewKeySize) {
    vReplaceKeySmaller(xOldKeyWrk, xNewKeyWrk);
    return;
  }
  
  ASSERT(uOldKeySize < uNewKeySize);
  vReplaceKeyLarger(xOldKeyWrk, xNewKeyWrk);
}

/*----------------------------------------------------------------------------*/
/* replace old key with new key of same size in the index buffer              */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v1::vReplaceKeyEqual(KeyWrk& xOldKeyWrk, KeyWrk& xNewKeyWrk) //# critical
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
  i8 nKeyCols = m_pIndex->pIdxColVec()->nItems();
  Entry* pOldEntry = pPosition(xOldKeyWrk, nKeyCols, JC_SAME);
  
  // assert that we found one
  ASSERT(pOldEntry);
  
  // assert the key of the higher level entry and the old entry match
  bool bIsEqual = false;
  ASSERT(pOldEntry->aKey()->bJuxtaCompare(JC_SAME,
                                          xOldKeyWrk.aKey(),
                                          m_pIndex->pIdxColVec(),
                                          nKeyCols,
                                          bIsEqual));
  
  EntWrk_v1 xEntWrk(pOldEntry);
  xEntWrk.vAddKey(xNewKeyWrk);
  
  // just replace the key in the buffer
  pOldEntry->vReplaceKey(xEntWrk);
}

/*----------------------------------------------------------------------------*/
/* replace old key with new key in the index buffer                           */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v1::vReplaceKeyLarger(KeyWrk& xOldKeyWrk, KeyWrk& xNewKeyWrk) //# critical
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
  i8 nKeyCols = m_pIndex->pIdxColVec()->nItems();
  Entry* pOldEntry = pPosition(xOldKeyWrk,nKeyCols,JC_SAME);
  
  // assert that we found the old key
  ASSERT(pOldEntry);
  
  // assert the key of the higher level entry and the old entry match
  bool bIsEqual = false;
  ASSERT(pOldEntry->aKey()->bJuxtaCompare(JC_SAME,
                                          xOldKeyWrk.aKey(),
                                          m_pIndex->pIdxColVec(),
                                          nKeyCols,
                                          bIsEqual));
  
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
  EntWrk_v1 xEntWrk(pOldEntry);
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
  pNewEntry->vSetEntry_v1(xEntWrk);
  ASSERT(pNewEntry->nSizeEntry() == nBytes);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // we have replaced the first entry in this buffer
  // hence we need to fix the right offsets in the head
  vAdjustOffsets(pHeadEntry(), nSizeDifference);
}

/*----------------------------------------------------------------------------*/
/* replace old key with new key in the index buffer                           */
/*----------------------------------------------------------------------------*/
void EntryBuffer_v1::vReplaceKeySmaller(KeyWrk& xOldKeyWrk, KeyWrk& xNewKeyWrk) //# critical
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
  i8 nKeyCols = m_pIndex->pIdxColVec()->nItems();
  Entry* pOldEntry = pPosition(xOldKeyWrk,nKeyCols,JC_SAME);
  
  // assert that we found one
  ASSERT(pOldEntry);
  
  // assert the key of the higher level entry and the old entry match
  bool bIsEqual = false;
  ASSERT(pOldEntry->aKey()->bJuxtaCompare(JC_SAME,
                                          xOldKeyWrk.aKey(),
                                          m_pIndex->pIdxColVec(),
                                          nKeyCols,
                                          bIsEqual));
  
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
  EntWrk_v1 xEntWrk(pOldEntry);
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
  pNewEntry->vSetEntry_v1(xEntWrk);
  ASSERT(pNewEntry->nSizeEntry() == nBytes);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // we have replaced the first entry in this buffer
  // hence we need to fix the right offsets in the head
  nSizeDifference = uNewKeySize - uOldKeySize;
  ASSERT( nSizeDifference < 0 );
  vAdjustOffsets(pHeadEntry(), nSizeDifference);
}

/*----------------------------------------------------------------------------*/
/* get right entry at given level (returns null if no right)                  */
/*----------------------------------------------------------------------------*/
Entry* EntryBuffer_v1::pRightEntry(Entry* pEntry, ln nLevelNo)         //# critical
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
  ASSERT(nLevelNo >= 0_ln && *nLevelNo < gpAttachMap->m_nIndexLevels);
  
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
         ( pRightEntry > pHeadEntry() &&
          pRightEntry < _pEntry(m_xWsp.aLatter()) ) );
  
  return pRightEntry;
}

/*----------------------------------------------------------------------------*/
/* converts given entry to absolute offset to end of entry                    */
/*----------------------------------------------------------------------------*/
i8 EntryBuffer_v1::iToEndOffsetAbs(Entry* pEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 iOffset = nSub(pEntry,m_xWsp.aArea());
  
  ASSERT(iOffset >= 0 && iOffset < m_xWsp.nSize());
  
  return iOffset + pEntry->nSizeEntry();
}

/*----------------------------------------------------------------------------*/
/* calculate the absolute offset within workspace for given entry             */
/*----------------------------------------------------------------------------*/
i8 EntryBuffer_v1::iToOffsetAbs(Entry* pEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER,_FOOTER_);
  
  i8 iOffset = nSub(pEntry,m_xWsp.aArea());
  
  ASSERT(iOffset >= 0 && iOffset < m_xWsp.nSize());
  
  return iOffset;
}

/*----------------------------------------------------------------------------*/
/* calculate the absolute right offset for a given entry and offset no        */
/*----------------------------------------------------------------------------*/
i8 EntryBuffer_v1::iToRightOffsetAbs(Entry* pEntry, on nOffsetNo)
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
u8 EntryBuffer_v1::uOffsetBetween(Entry* pEntryA, Entry* pEntryB)
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

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* EntryBuffer_v1::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ENTRYBUFFER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
