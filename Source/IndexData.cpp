
/*============================================================================*/
/* IndexData.cpp - index data portion of table partition                      */
/*============================================================================*/

#include "DBobjects.h"
#include "StorageEngine.h"

extern BufferPool*  gpBufferPool;
extern Grid*        gpGrid;

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
IndexData* DBobjects::_pIndexData(void* pAddr)
{
  IndexData* pIndexData = reinterpret_cast<IndexData*>(pAddr);
  
  RASSERT( !pIndexData || CN_INDEXDATA == pIndexData->eClass() );
  
  return pIndexData;
}

/*----------------------------------------------------------------------------*/
/* dump data                                                                  */
/*----------------------------------------------------------------------------*/
void IndexData::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  ASSERT(eClass == CN_INDEXDATA);
  ASSERT(pObject->eClass() == CN_INDEXDATA);
  
}

/*----------------------------------------------------------------------------*/
/* finalize                                                                   */
/*----------------------------------------------------------------------------*/
void IndexData::vFinalize(eCN eClass, Object* pObject)
{
  ASSERT(eClass == CN_INDEXDATA);
  ASSERT(pObject->eClass() == CN_INDEXDATA);
  
  IndexData* pIndexData = reinterpret_cast<IndexData*>(pObject);
  ASSERT(pIndexData->bWordBody());
  
  pIndexData->vTidyUp();
}

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* IndexData::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  void* pObject = Heap::pAllocate(CN_INDEXDATA);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void IndexData::operator delete(void* pDataAddress)
{
  RASSERT(pDataAddress);
  IndexData* pIndexData = reinterpret_cast<IndexData*>(pDataAddress);
  
  Heap::vFree(pIndexData);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
IndexData::IndexData(Index* pIndex, i8 nPartitionNo)
: Object(CN_INDEXDATA)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  m_zLock.vInitLock(LK_INDEX_DATA);
  m_pIndex = pIndex;
  
  // start with a root buffer at L0
  //m_pRoot = new EntryBuffer(pIndex, 0);
  
  /// set current buffer
  //m_pCurrentBuffer = m_pRoot;
  
  m_pRoot = nullptr;
  m_pCurrentBuffer = nullptr;
  m_nPartitionNo = nPartitionNo;
  
  _checkInitialised_(CN_INDEXDATA);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
IndexData::~IndexData(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  vTidyUp();
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* analyze table                                                              */
/*----------------------------------------------------------------------------*/
Stz* IndexData::pAnalyze(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  i8 nRowsTotal = 0;
  i8 nBuffersTotal = 0;
  i8 nStrandsTotal = 0;
  s48K sAnalysis;
  
  sAnalysis.vCatSprint("N%lld: Analysis of Index %s of table %s\n"_Stz,
                       gpGrid->iThisNode(),
                       m_pIndex->aIndexName()->aChar(),
                       m_pIndex->pTable()->aTableName());
  
  return sAnalysis;
}

/*----------------------------------------------------------------------------*/
/* dump the index                                                             */
/*----------------------------------------------------------------------------*/
void IndexData::vDump(bool bSameLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);

  if (bSameLevel) {
    EntryBuffer* pEntryBuffer = m_pRoot;
    
    while (pEntryBuffer) {
      pEntryBuffer->vShow();
      pEntryBuffer = pEntryBuffer->pNextBuffer();
    }
  }
  else {
    // start with the root
    if (m_pRoot)
      m_pRoot->vDump();
  }
}

/*----------------------------------------------------------------------------*/
/* get the first L0 buffer                                                    */
/*----------------------------------------------------------------------------*/
EntryBuffer* IndexData::pFirstBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  EntryBuffer* pEntryBuffer = m_pRoot;
  
  if ( ! pEntryBuffer )
    return nullptr;
  
  if (pEntryBuffer->nBufferLevel() == 0)
    return pEntryBuffer;
  
  while (pEntryBuffer) {
    
    Entry* pFirstEntry = pEntryBuffer->pNextEntry(pEntryBuffer->pHead());
    pEntryBuffer = pFirstEntry->pEntryBuffer();
    
    if (pEntryBuffer->nBufferLevel() == 0) {
      return pEntryBuffer;
    }
  }
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get the index                                                              */
/*----------------------------------------------------------------------------*/
Index* IndexData::pIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  return m_pIndex;
}

/*----------------------------------------------------------------------------*/
/* get the index buffers used                                                 */
/*----------------------------------------------------------------------------*/
i8 IndexData::nEntryBuffers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  i8 nBufferCount = 0;
  if (m_pRoot)
    m_pRoot->vBufferCount(nBufferCount);
  
  return nBufferCount;
}

/*----------------------------------------------------------------------------*/
/* insert an entry for {key,id} into an index buffer at given level           */
/*----------------------------------------------------------------------------*/
void IndexData::vAppendBuffer(EntryBuffer* pEntryBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  m_zLock.vGrabWrite();
  
  if (m_pRoot == nullptr) {
    ASSERT( ! m_pCurrentBuffer )
    m_pRoot = pEntryBuffer;
  }
  else {
    pEntryBuffer->vNextBuffer(m_pCurrentBuffer->pNextBuffer());
    pEntryBuffer->vPrevBuffer(m_pCurrentBuffer);
    m_pCurrentBuffer->vNextBuffer(pEntryBuffer);
  }
  
  m_pCurrentBuffer = pEntryBuffer;
  
   m_zLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* insert an entry for {key,id} into an index buffer at given level           */
/*----------------------------------------------------------------------------*/
void IndexData::vAppendEntries(EntryBuffer* pEntryBuffer, EntryBuffer** ppFirstBuffer, Entry** ppFirstEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  EntryVec xPriorVec;
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  Entry* pNewEntry;
  
  *ppFirstBuffer = nullptr;
  *ppFirstEntry = nullptr;
  
  m_zLock.vGrabWrite();
  
  Entry* pRow = pEntryBuffer->pFirstRow();

  if (m_pRoot == nullptr) {
    ASSERT( ! m_pCurrentBuffer )
    m_pRoot = new EntryBuffer(m_pIndex, 0, m_nPartitionNo);
    m_pCurrentBuffer = m_pRoot;
  }
  
  for ( pRow = pEntryBuffer->pNextRow(pRow) ; pRow;
       pRow = pEntryBuffer->pNextRow(pRow)) {
    
    i8 nEntryBytes = pRow->nSizeEntry();
    
    if ( ! m_pCurrentBuffer->bHasSpace(nEntryBytes) ) {
      
      //----------------------------------------------------------------------------
      // create a new entry buffer and link with current buffer
      EntryBuffer* pNewBuffer = new EntryBuffer(m_pIndex, 0, m_nPartitionNo);
      pNewBuffer->vNextBuffer(m_pCurrentBuffer->pNextBuffer());
      pNewBuffer->vPrevBuffer(m_pCurrentBuffer);
      m_pCurrentBuffer->vNextBuffer(pNewBuffer);
      
      //----------------------------------------------------------------------------
      // toggle to the new buffer
      m_pCurrentBuffer = pNewBuffer;
    }
    
    bool bInserted = m_pCurrentBuffer->bAppendEntry(pRow, &pNewEntry);
    ASSERT(bInserted);
    
    if ( ! *ppFirstBuffer )
      *ppFirstBuffer = m_pCurrentBuffer;
    
    if ( ! *ppFirstEntry )
      *ppFirstEntry = pNewEntry;
    
  }
  
  m_zLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* append an entry into this index at level 0                                 */
/*----------------------------------------------------------------------------*/
void IndexData::vAppendEntry(KeyWrk& xKeyWrk, Id xId, EntryVec& xPriorVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  m_zLock.vGrabWrite();
  
  m_pCurrentBuffer = pAppendL0Entry(m_pCurrentBuffer,xKeyWrk,xId,xPriorVec);
  
  m_zLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* insert an entry for {key,id} into an index buffer at given level           */
/*----------------------------------------------------------------------------*/
Entry* IndexData::pInsertEntry(i8 iTxId, Stz* pRow, i8 iLength)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  Entry* pEntry;
  
  m_zLock.vGrabWrite();
  
  ASSERT(m_pIndex->bIsHidden());

  ASSERT(m_pCurrentBuffer);
  ASSERT(m_pCurrentBuffer->nBufferLevel() == 0);
  
  KeyWrk xKeyWrk;
  ValWrk xValWrk;
  
  //----------------------------------------------------------------------------
  // form the rowId
  bn nBufferNo = m_pCurrentBuffer->nBufferNo();
  i8 nRowNo = m_pCurrentBuffer->nEntries();
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
  
  if ( ! m_pCurrentBuffer->bHasSpace(nEntryBytes) ) {
    
    //----------------------------------------------------------------------------
    // create a new entry buffer and link with current buffer
    EntryBuffer* pNewBuffer = new EntryBuffer(m_pIndex, 0, m_nPartitionNo);
    pNewBuffer->vNextBuffer(m_pCurrentBuffer->pNextBuffer());
    pNewBuffer->vPrevBuffer(m_pCurrentBuffer);
    m_pCurrentBuffer->vNextBuffer(pNewBuffer);
    
    //----------------------------------------------------------------------------
    // toggle to the new buffer
    m_pCurrentBuffer = pNewBuffer;
    
    //----------------------------------------------------------------------------
    // form the rowId from new current buffer
    nBufferNo = m_pCurrentBuffer->nBufferNo();
    nRowNo = m_pCurrentBuffer->nEntries();
    Id newRowId(*nBufferNo, nRowNo);
    
    //----------------------------------------------------------------------------
    // form the new key
    KeyWrk xNewKeyWrk;
    xValWrk = nRowNo;
    Column* pColumn = m_pIndex->pIdxColVec()->pGet(0);
    ASSERT(pColumn->iColumnNo() == -1);
    xNewKeyWrk.vAddDatum(xValWrk, pColumn);

    //----------------------------------------------------------------------------
    // add the id and key to NewEntWrk
    EntWrk xNewEntWrk(newRowId, xNewKeyWrk);
    
    //----------------------------------------------------------------------------
    // add the row data to the NewEntWrk
    xNewEntWrk.vData(pRow, iLength);
    
    pEntry = m_pCurrentBuffer->pInsert(xNewEntWrk);
    
    //m_pCurrentBuffer->vShow();
  }
  else {

    // insert entry into current index buffer
    pEntry = m_pCurrentBuffer->pInsert(xEntWrk);
  }
  
  m_zLock.vDropWrite();
  
  return pEntry;
}

/*----------------------------------------------------------------------------*/
/* insert entries from entryBuffer into this index at level 0                 */
/*----------------------------------------------------------------------------*/
void IndexData::vInsertEntries(EntryBuffer* pEntryBuffer, Entry* pRowEntry)
{ //# indexTuning
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  //m_zLock.vGrabWrite();
  
  for ( Entry* pEntry = pRowEntry ; pEntry; pEntry = pEntryBuffer->pNextRow(pEntry)) {
    vInsertEntry(pEntry);
  }
  
  //m_zLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* insert an entry for given row into this index at level 0                   */
/*----------------------------------------------------------------------------*/
void IndexData::vInsertEntry(Entry* pRow)
{ //# indexTuning
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  m_zLock.vGrabWrite();
  
  KeyWrk xKeyWrk;
  pRow->vExtractKey(xKeyWrk, m_pIndex->pIdxColVec());
  
  //xKeyWrk.vShow();
  
  if ( ! m_pRoot )
    m_pRoot = new EntryBuffer(m_pIndex, 0, m_nPartitionNo);
  
  vInsertEntry(m_pRoot, xKeyWrk, pRow->xId(), 0);
  
  m_zLock.vDropWrite();
  
  //if ( ! bVerify() ) {
  //  vTrace("Inserting row that caused verify to fail:\n"_Stz);
  //  pRow->vContentDump();
  //  vDump();
  //  NEVER_REACH;
  //}
}

/*----------------------------------------------------------------------------*/
/* position the indexData at a particular level of index buffer               */
/*----------------------------------------------------------------------------*/
EntryBuffer* IndexData::pPositionLevel(KeyWrk& xKeyWrk, eJC eJuxtaCompare, i8 iLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  if (m_pRoot)
    return m_pRoot->pPositionLevel(xKeyWrk, eJuxtaCompare, iLevel);
  else
    return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get root entry                                                             */
/*----------------------------------------------------------------------------*/
EntryBuffer* IndexData::pRoot(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  return m_pRoot;
}

/*----------------------------------------------------------------------------*/
/* tidyup                                                                     */
/*----------------------------------------------------------------------------*/
void IndexData::vTidyUp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  // nothing to do
}

/*----------------------------------------------------------------------------*/
/* truncate the index                                                         */
/*----------------------------------------------------------------------------*/
void IndexData::vTruncate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  m_zLock.vGrabWrite();
  
  if (m_pRoot)
    m_pRoot->vTruncate();
  
  m_pRoot = nullptr;
  
  m_zLock.vDropWrite();

}

/*----------------------------------------------------------------------------*/
/* verify                                                                     */
/*----------------------------------------------------------------------------*/
bool IndexData::bVerify(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);

  /*--------------------------------------------------------------------------//
   B5
   -------------
   |ka Kl   ...|                   Level L2 buffer (root)
   -------------
   |   |
   |    -----------
   |               |
   V B3            V B4
   -------------   -------------
   |ka kj   ...|-->| kl kp...   |  Level L1 buffer
   -------------   -------------
   |   |
   |    -----------
   |               |
   V B1            V B2
   -------------   -------------
   |ka kb   ...|-->| kj kk  ...|   Level L0 buffer
   -------------   -------------
   
  //--------------------------------------------------------------------------*/

  
  // start with the root buffer  L2
  EntryBuffer* pEntryBuffer = m_pRoot;
  
  while (pEntryBuffer) {
    
    // first just run verify on the entries of the bufer
    // ka < kl < .....
    bool bVerify = pEntryBuffer->bVerify();
    if ( ! bVerify ) {
      return false;
    }
    
    // if we are at level 0 , get the next buffer at the same level
    if (pEntryBuffer->nBufferLevel() == 0) {
      // if at B1 get B2
      pEntryBuffer = (EntryBuffer*)pEntryBuffer->pNextBuffer();
      continue;
    }
    
    // higher level buffer ( L1, L2...) get first entry
    // if at L2 , we are in B5 , we get entry ka
    Entry* aEntryLN = pEntryBuffer->pFirstEntry();
    
    // iterate through each entry and ensure correct order
    while (aEntryLN) {
      
      // Note: this is an L1, L2 etc buffer
      // each entry points to another index buffer
      // get buffer first entry points to
      // if k1 we get B1
      EntryBuffer* pEntryBufferN_1 = aEntryLN->pEntryBuffer();
      
      // get the key of the first entry
      // if ka get ka
      Key* pKeyLN = aEntryLN->aKey();
      
      // now get the first entry from the lower buffer
      // bet B1's first entry ka
      Entry* aEntryLN_1 = pEntryBufferN_1->pFirstEntry();
      
      // get the key of the first entry of the lower buffer
      // we get ka from B1
      Key* pKeyLN_1 = aEntryLN_1->aKey();

      // pKeyLN should be the same as pKeyLN_1
      // ka of B1 and ka of B3 should be the same
      bool bIsEqual = false;
      bool bSame = pKeyLN->bJuxtaCompare(JC_SAME, aEntryLN_1->aKey(), m_pIndex->pIdxColVec(), bIsEqual);
      if ( ! bSame ) {
        return false;
      }
      
      // get the next entry at the level we are at
      aEntryLN = pEntryBuffer->pNextEntry(aEntryLN);
    }
  
    // move to the next buffer at the same level
    pEntryBuffer = (EntryBuffer*)pEntryBuffer->pNextBuffer();
  }
  
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* check current L0 index buffer for space and append an entry for {key,id}   */
/*----------------------------------------------------------------------------*/
EntryBuffer* IndexData::pAppendL0Entry(EntryBuffer* pEntryBuffer,
                                       KeyWrk& xKeyWrk, Id xId,
                                       EntryVec& xPriorVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  // current working buffer should be a L0 buffer
  ASSERT(pEntryBuffer == nullptr || pEntryBuffer->nBufferLevel() == 0);
  
  // initialize the entry wsp (generates randon number of offsets)
  EntWrk xEntWrk(xId,xKeyWrk);

  //xKeyWrk.vShow();
  
  // get the space required for this new entry
  i8 nEntryBytes = Entry::nBytesRequired(xEntWrk);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // case where we append into an existing L0 entry buffer
  
  // check for a current L0 buffer and that it has enough space
  if (pEntryBuffer && pEntryBuffer->bHasSpace(nEntryBytes)) {
    
    // append entry into L0 index buffer
    Entry* pEntry = pEntryBuffer->pAppendEntry(xEntWrk,xPriorVec);
    ASSERT(pEntry);

    return pEntryBuffer;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // case where existing L0 entry buffer is full
  
  EntryBuffer* pHigherLevel = nullptr;
  
  if (pEntryBuffer) {
    
    // get the L1 higher level buffer
    pHigherLevel = pEntryBuffer->pHigherLevel();
    ASSERT(pHigherLevel);
    
    // create a subsequent L0 entry buffer and set its higher level buffer
    pEntryBuffer = pEntryBuffer->pNewNextBuffer();
    pEntryBuffer->vHigherLevel(pHigherLevel);
    
    // set the prior vector to be the head entry of L0 buffer
    pEntryBuffer->vPriorVecHead(xPriorVec);
    
    // append entry into subsequent L0 buffer
    Entry* pEntry = pEntryBuffer->pAppendEntry(xEntWrk,xPriorVec);
    
    // insert the first entry of L0 entry buffer into L1 higher level buffer
    vInsertFirstEntry(pHigherLevel,pEntryBuffer);
    
    //---------------------------------------------------------
    // fix the index buffer's higher level buffer
    vFixHigherBuffer(pEntryBuffer, pHigherLevel->nBufferLevel());

    //pEntryBuffer->vDump();
    
    return pEntryBuffer;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // case where there is no existing L0 entry buffer
  
  ASSERT(! m_pRoot);
  
  // create very first L1 buffer and set it as root
  pHigherLevel = new EntryBuffer(m_pIndex,1,m_nPartitionNo);
  m_pRoot = pHigherLevel;
  
  // create very first L0 buffer and set its higher level buffer
  pEntryBuffer = new EntryBuffer(m_pIndex,0,m_nPartitionNo);
  pEntryBuffer->vHigherLevel(pHigherLevel);
  
  // set the prior vector to be the head entry of L0 buffer
  pEntryBuffer->vPriorVecHead(xPriorVec);
  
  // append entry into first L0 buffer
  Entry* pEntry = pEntryBuffer->pAppendEntry(xEntWrk,xPriorVec);
  
  // insert the first entry of L0 entry buffer into L1 higher level buffer
  vInsertFirstEntry(pHigherLevel,pEntryBuffer);
  
  return pEntryBuffer;
}

/*----------------------------------------------------------------------------*/
/* insert an entry for {key,id} into an index buffer at given level           */
/*----------------------------------------------------------------------------*/
void IndexData::vInsertEntry(EntryBuffer* pHigher, KeyWrk& xKeyWrk, Id xId, i8 iLevel)
{ //# indexTuning
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  // initialize the entry wsp
  EntWrk xEntWrk(xId,xKeyWrk);
  
  // find index buffer which contains the entry after which the new entry will
  // be inserted into, starting with given higher
  EntryBuffer* pEntryBuffer = pHigher->pPositionLevel(xKeyWrk, JC_AFTER, iLevel);
  
  // check if found index buffer has enough space
  i8 nEntryBytes = Entry::nBytesRequired(xEntWrk);
  if ( ! pEntryBuffer->bHasSpace(nEntryBytes) ) {
    
    // not enough space, so split the index buffer
    pEntryBuffer = pSplitBuffer(pEntryBuffer);
    
    //vTrace("-----------------------\n"_Stz);
    //m_pRoot->vDump();
    //vTrace("-----------------------\n"_Stz);
    
    // find position again
    bool bEqual;
    pEntryBuffer = pEntryBuffer->pHigherLevel()->pPositionLevel(xKeyWrk, JC_AFTER, iLevel);
  }
  
  // assert we have a buffer
  ASSERT(pEntryBuffer);
  
  // insert entry into index buffer
  Entry* pEntry = pEntryBuffer->pInsert(xEntWrk);
  //pEntry->vShow(0, m_pIndex);
  
  //pEntryBuffer->vShow();
  
  ASSERT(pEntry);
}

/*----------------------------------------------------------------------------*/
/* add first entry of fromBuffer into the toBuffer                            */
/*----------------------------------------------------------------------------*/
void IndexData::vInsertFirstEntry(EntryBuffer* pToBuffer, EntryBuffer* pFromBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);

  Id  xNewId;
  
  ASSERT(pToBuffer->nBufferLevel() == pFromBuffer->nBufferLevel() + 1);
  // form the Id
  bn nFirstBuffer = pFromBuffer->nBufferNo();
  xNewId.vBuffer(*nFirstBuffer);
  xNewId.vRow(C_HIGHER_LEVEL_ID);
  
  // get the first entry from the first buffer
  Entry* pHeadEntry = pFromBuffer->pHead();
  Entry* pFirstEntry = pFromBuffer->pNextEntry(pHeadEntry);
  
  // get the key from the first entry
  Key* aKey = pFirstEntry->aKey();
  
  // form a KeyWrk from the key
  KeyWrk xFirstKeyWrk;
  xFirstKeyWrk.vAddKey(aKey, m_pIndex->pIdxColVec());
  
  vInsertEntry(pToBuffer, xFirstKeyWrk, xNewId, pToBuffer->nBufferLevel());
}

/*----------------------------------------------------------------------------*/
/* fix the higher level buffer in the given index buffer                      */
/*----------------------------------------------------------------------------*/
void IndexData::vFixHigherBuffer(EntryBuffer* pEntryBuffer, i8 iLevel)
{
   _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  KeyWrk xKeyWrk;
  EntryBuffer* pRoot;
  
  // get first entry KeyWrk from first buffer
  Entry* aFirstEntry = pEntryBuffer->pNextEntry(pEntryBuffer->pHead());

  // form a KeyWrk from the key
  xKeyWrk.vAddKey(aFirstEntry->aKey(), m_pIndex->pIdxColVec());
  
  pRoot = m_pRoot->pPositionLevel(xKeyWrk, JC_AFTER, iLevel);

  pEntryBuffer->vHigherLevel(pRoot);
}

/*----------------------------------------------------------------------------*/
/* split a buffer                                                             */
/*----------------------------------------------------------------------------*/
EntryBuffer* IndexData::pSplitBuffer(EntryBuffer* pEntryBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  EntryBuffer* pRoot;
  EntryBuffer* pNewRoot;
  
  KeyWrk xFirstKeyWrk1;
  KeyWrk xFirstKeyWrk2;
  
  // split the index buffer
  EntryBuffer* pEntryBufferNew = pEntryBuffer->pSplitBuffer();
  
  // get the next level buffer
  EntryBuffer* pHigherLevel = pEntryBuffer->pHigherLevel();
  
  // create a next level buffer if it does not exist
  if ( ! pHigherLevel ) {
    
    i8 nHigherLevel = pEntryBuffer->nBufferLevel() + 1;
    pRoot = new EntryBuffer(m_pIndex, nHigherLevel, m_nPartitionNo);
    
    // insert first entry in new root from buffer we split
    vInsertFirstEntry(pRoot, pEntryBuffer);
    
    // root now points to the new higher level
    m_pRoot = pRoot;
  }
  else {
    pRoot = pHigherLevel;
  }
  
  //---------------------------------------------------------
  // insert the first entry in root buffer from the new EntryBuffer
  vInsertFirstEntry(pRoot, pEntryBufferNew);
  
  //---------------------------------------------------------
  // fix the first index buffer's higher level buffer
  vFixHigherBuffer(pEntryBuffer, pRoot->nBufferLevel());
  
  //---------------------------------------------------------
  // fix the second index buffer's higher level buffer
  vFixHigherBuffer(pEntryBufferNew, pRoot->nBufferLevel());
  
  return pEntryBuffer;
}

/*----------------------------------------------------------------------------*/
/* visualize the index                                                        */
/*----------------------------------------------------------------------------*/
void IndexData::vDumpLevel(EntryBuffer* pEntryBuffer)
{
  
  // get the first entry
  Entry* pEntry = pEntryBuffer->pFirstEntry();
  
  while (pEntry) {
    
    // get the index buffer the first enetry poinst to
    EntryBuffer* pEntryEntryBuffer = pEntry->pEntryBuffer();
    
    // print the decile of the buffer
    vTrace("%lld"_Stz, pEntryEntryBuffer->iDecile());
    
    // insert spaces for no of entries in buffer
    if (pEntryEntryBuffer->nBufferLevel() > 0) {
      
      // exclude the head entry
      i8 iEntries = pEntryEntryBuffer->nEntries() - 1;
      
      while (iEntries--)
        vTrace(" "_Stz);
    }
    
    // move to the next entry
    pEntry = pEntryBuffer->pNextEntry(pEntry);
  }
}

/*----------------------------------------------------------------------------*/
/* visualize the index                                                        */
/*----------------------------------------------------------------------------*/
void IndexData::vVisualize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  // start with the root buffer
  EntryBuffer* pEntryBuffer = m_pRoot;
  
  // hidden index has all level 0 buffers
  if (m_pIndex->bIsHidden()) {
    
    do {
      ASSERT(pEntryBuffer->nBufferLevel() == 0);
      // print the decile of pNextBuffer
      vTrace("%lld"_Stz, pEntryBuffer->iDecile());
      
      pEntryBuffer = pEntryBuffer->pNextBuffer();
      
    } while (pEntryBuffer);
    
    vTrace("\n"_Stz);
    return;
  }

  // print the decile of the root buffer
  vTrace("%lld\n"_Stz, pEntryBuffer->iDecile());
  
  i8 iBufferLevel = pEntryBuffer->nBufferLevel();
  
  // if there is only one buffer i.e root is at level 0, we are done
  if (iBufferLevel == 0) {
    vTrace("\n"_Stz);
    return;
  }
  
  // iterate through the buffers
  while (pEntryBuffer) {
    
    // dump this buffer
    vDumpLevel(pEntryBuffer);

    // if we are at level 1, iterate thru next buffers
    if (pEntryBuffer->nBufferLevel() == 1) {
      pEntryBuffer = (EntryBuffer*)pEntryBuffer->pNextBuffer();
    }
    else {
      vTrace("\n"_Stz);
      
      // get the first entry of the buffer
      Entry* pEntry = pEntryBuffer->pFirstEntry();
      
      // get the index buffer this entry points to
      pEntryBuffer = pEntry->pEntryBuffer();
    }
  }
  
  vTrace("\n"_Stz);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* IndexData::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
