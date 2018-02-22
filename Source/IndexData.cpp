
/*============================================================================*/
/* IndexData.cpp - index data portion of table partition                      */
/*============================================================================*/

#include "DBobjects.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

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
  
  if (m_pIndex->bIsHidden())
      sAnalysis.vCatSprint("N%lld: Analysis of table %s\n"_Stz,
                           gpAttachMap->m_pGrid->iThisNode(),
                           m_pIndex->pTable()->aTableName());
  else
    sAnalysis.vCatSprint("N%lld: Analysis of Index %s of table %s\n"_Stz,
                         gpAttachMap->m_pGrid->iThisNode(),
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
EntryBuffer* IndexData::pFirstL0Buffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  EntryBuffer* pEntryBuffer = m_pRoot;
  
  if ( ! pEntryBuffer )
    return nullptr;
  
  if (pEntryBuffer->nBufferLevel() == 0)
    return pEntryBuffer;
  
  while (pEntryBuffer) {
    
    Entry* pFirstEntry = pEntryBuffer->pFirstEntry();
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
/* append a buffer into the indexData (should be hidden index indexdata)      */
/*----------------------------------------------------------------------------*/
void IndexData::vAppendBuffer(EntryBuffer* pEntryBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  // we should be appending buffers only for hidden index
  ASSERT(m_pIndex->bIsHidden());
  
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
void IndexData::vAppendEntriesFromBuffer(EntryBuffer* pEntryBuffer, EntryBuffer** ppFirstBuffer, Entry** ppFirstEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  EntryVec xPriorVec;
  IdxColVec* pIdxColVec = m_pIndex->pIdxColVec();
  Entry* pNewEntry;
  
  *ppFirstBuffer = nullptr;
  *ppFirstEntry = nullptr;
  
  m_zLock.vGrabWrite();
  
  Entry* pRow = pEntryBuffer->pHeadEntry();

  if (m_pRoot == nullptr) {
    ASSERT( ! m_pCurrentBuffer )
    m_pRoot = new EntryBuffer(m_pIndex, 0, m_nPartitionNo);
    m_pCurrentBuffer = m_pRoot;
  }
  
  for ( pRow = pEntryBuffer->pNextEntry(pRow, 0) ; pRow;
       pRow = pEntryBuffer->pNextEntry(pRow, 0)) {
    
    bool bInserted = m_pCurrentBuffer->bAppendEntry(pRow, &pNewEntry);
    if ( !bInserted ) {
      //----------------------------------------------------------------------------
      // create a new entry buffer and link with current buffer
      EntryBuffer* pNewBuffer = new EntryBuffer(m_pIndex, 0, m_nPartitionNo);
      pNewBuffer->vNextBuffer(m_pCurrentBuffer->pNextBuffer());
      pNewBuffer->vPrevBuffer(m_pCurrentBuffer);
      m_pCurrentBuffer->vNextBuffer(pNewBuffer);
      
      //----------------------------------------------------------------------------
      // toggle to the new buffer
      m_pCurrentBuffer = pNewBuffer;
      
      // now insert into new buffer
      bInserted = m_pCurrentBuffer->bAppendEntry(pRow, &pNewEntry);
      ASSERT(bInserted);
    }
    
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
void IndexData::vAppendEntry(KeyWrk& xKeyWrk,
                             //Id xId,
                             EntryVec& xPriorVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  // this is invoked during ld while building non-hidden indexes
  ASSERT( ! m_pIndex->bIsHidden() );
  
  m_zLock.vGrabWrite();
  
  //m_pCurrentBuffer = pAppendL0Entry(m_pCurrentBuffer,xKeyWrk,xId);
  
  if ( ! m_pRoot ) {
    m_pRoot = new EntryBuffer;
    m_pRoot->vIndex(m_pIndex);
    m_pRoot->vPartitionNo(m_nPartitionNo);
  }
  
  //vTrace("\nInserting key: "_Stz);
  //xKeyWrk.aKey()->vShow(m_pIndex->pIdxColVec());
  
  Entry* pKeyEntry =
    EntryBuffer::pInsertKeyTree(xKeyWrk,
                                //0,
                                //xKeyWrk.iTxId(),
                                //xId,
                                //xKeyWrk.xId(),
                                //xKeyWrk.aKey(),
                                //xKeyWrk.nKeySize(),
                                &m_pRoot);
  
  //m_pRoot->vShowTree(true);
  
  m_zLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* insert entries from entryBuffer into this index at level 0                 */
/*----------------------------------------------------------------------------*/
void IndexData::vInsertEntriesFromBuffer(EntryBuffer* pEntryBuffer, Entry* pRowEntry)
{ //# indexTuning
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  //m_zLock.vGrabWrite();
  
  for ( Entry* pEntry = pRowEntry ; pEntry; pEntry = pEntryBuffer->pNextEntry(pEntry, 0)) {
    bool bInserted = bInsertEntry(pEntry);
    ASSERT(bInserted);
  }
  
  //m_zLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/* insert an entry for given row into this index at level 0                   */
/*----------------------------------------------------------------------------*/
bool IndexData::bInsertEntry(Entry* pRow)
{ //# indexTuning
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  m_zLock.vGrabWrite();
  
  KeyWrk xKeyWrk;
  pRow->vExtractIndexKey(xKeyWrk, m_pIndex->pIdxColVec());
  
  xKeyWrk.vTxId(0);
  xKeyWrk.vId(pRow->xId());
  
  //xKeyWrk.vShow(m_pIndex->pIdxColVec());
  
  if ( ! m_pRoot ) {
    m_pRoot = new EntryBuffer;
    m_pRoot->vIndex(m_pIndex);
    m_pRoot->vPartitionNo(m_nPartitionNo);
  }
  
  //bool bInserted = bInsertEntry(m_pRoot, xKeyWrk, pRow->xId(), 0);
  
  Entry* pKeyEntry =
    EntryBuffer::pInsertKeyTree(xKeyWrk,
                                //0,
                                //xKeyWrk.iTxId(),
                                //pRow->xId(),
                                //xKeyWrk.xId(),
                                //xKeyWrk.aKey(),
                                //xKeyWrk.nKeySize(),
                                &m_pRoot);
  ASSERT(pKeyEntry);
  
  m_zLock.vDropWrite();
  
  //if ( ! bVerify() ) {
  //  vTrace("Inserting row that caused verify to fail:\n"_Stz);
  //  pRow->vContentDump();
  //  vDump();
  //  NEVER_REACH;
  //}
  
  return true;                                                    //ALKA
}

/*----------------------------------------------------------------------------*/
/* position the indexData at a particular level of index buffer               */
/*----------------------------------------------------------------------------*/
EntryBuffer* IndexData::pPositionLevel(KeyWrk& xKeyWrk, i8 nKeyCols, eJC eJuxtaCompare, i8 iLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  if (m_pRoot)
    return m_pRoot->pPositionLevel(xKeyWrk.aKey(), nKeyCols, eJuxtaCompare, iLevel);
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
      bool bSame = pKeyLN->bJuxtaCompare(JC_SAME,
                                         aEntryLN_1->aKey(),
                                         m_pIndex->pIdxColVec(),
                                         m_pIndex->pIdxColVec()->nItems(),
                                         bIsEqual);
      if ( ! bSame ) {
        return false;
      }
      
      // get the next entry at the level we are at
      aEntryLN = pEntryBuffer->pNextEntry(aEntryLN, 0);
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
      i8 iEntries = pEntryEntryBuffer->nItems() - 1;
      
      while (iEntries--)
        vTrace(" "_Stz);
    }
    
    // move to the next entry
    pEntry = pEntryBuffer->pNextEntry(pEntry, 0);
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
