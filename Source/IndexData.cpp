
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
void IndexData::vDump(eCN eClass, Object* pObject, Str* aTab)
{
  ASSERT(eClass == CN_INDEXDATA);
  ASSERT(pObject->eClass() == CN_INDEXDATA);
  
  Data* pData = reinterpret_cast<Data*>(pObject);
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
IndexData::IndexData(Index* pIndex)
: Object(CN_INDEXDATA)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  m_zLock.vInit("IndexData::m_zLock"_Str);
  m_pIndex = pIndex;
  
  // start with a root buffer at L0
  m_pRoot = new IndexBuffer(pIndex, 0);
  
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
Str* IndexData::pAnalyze(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  i8 nRowsTotal = 0;
  i8 nBuffersTotal = 0;
  i8 nStrandsTotal = 0;
  s48K sAnalysis;
  
  sAnalysis.vCatSprint("N%lld: Analysis of Index %s of table %s\n"_Str,
                       gpGrid->iThisNode(),
                       m_pIndex->aIndexName()->s(),
                       m_pIndex->pTable()->aTableName());
  
  return sAnalysis;
}

/*----------------------------------------------------------------------------*/
/* dump the index                                                             */
/*----------------------------------------------------------------------------*/
void IndexData::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);

  // start with the root
  m_pRoot->vDump();
}

/*----------------------------------------------------------------------------*/
/* get the first L0 buffer                                                    */
/*----------------------------------------------------------------------------*/
IndexBuffer* IndexData::pFirstBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  IndexBuffer* pIndexBuffer = m_pRoot;
  
  if (pIndexBuffer->nBufferLevel() == 0)
    return pIndexBuffer;
  
  while (pIndexBuffer) {
    
    Entry* pFirstEntry = pIndexBuffer->pNextEntry(pIndexBuffer->pHead());
    pIndexBuffer = pFirstEntry->pIndexBuffer();
    
    if (pIndexBuffer->nBufferLevel() == 0) {
      return pIndexBuffer;
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
i8 IndexData::nIndexBuffers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  i8 nBufferCount = 0;
  m_pRoot->vBufferCount(nBufferCount);
  
  return nBufferCount;
}

/*----------------------------------------------------------------------------*/
/* insert an entry for given row into this index at level 0                   */
/*----------------------------------------------------------------------------*/
void IndexData::vInsertEntry(Row* pRow, i8 nRowNo, bn nBufferNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  Id xId(*nBufferNo,nRowNo);
  
  KeyWsp xKeyWsp;
  pRow->vExtractKey(xKeyWsp, m_pIndex->pIdxColVec());
    
  vInsertEntry(m_pRoot, xKeyWsp, xId, 0);
  
  //if ( ! bVerify() ) {
  //  vTrace("Inserting row that caused verify to fail:\n"_Str);
  //  pRow->vContentDump();
  //  vDump();
  //  ASSERT(0);
  //}
}

/*----------------------------------------------------------------------------*/
/* position the indexData at a particular level of index buffer               */
/*----------------------------------------------------------------------------*/
IndexBuffer* IndexData::pPositionLevel(KeyWsp& xKeyWsp, eJC eJuxtaCompare, i8 iLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  return m_pRoot->pPositionLevel(xKeyWsp, eJuxtaCompare, iLevel);
}

/*----------------------------------------------------------------------------*/
/* get root entry                                                             */
/*----------------------------------------------------------------------------*/
IndexBuffer* IndexData::pRoot(void)
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
  IndexBuffer* pIndexBuffer = m_pRoot;
  
  while (pIndexBuffer) {
    
    // first just run verify on the entries of the bufer
    // ka < kl < .....
    bool bVerify = pIndexBuffer->bVerify();
    if ( ! bVerify ) {
      return false;
    }
    
    // if we are at level 0 , get the next buffer at the same level
    if (pIndexBuffer->nBufferLevel() == 0) {
      // if at B1 get B2
      pIndexBuffer = pIndexBuffer->pNextBuffer();
      continue;
    }
    
    // higher level buffer ( L1, L2...) get first entry
    // if at L2 , we are in B5 , we get entry ka
    Entry* aEntryLN = pIndexBuffer->pFirstEntry();
    
    // iterate through each entry and ensure correct order
    while (aEntryLN) {
      
      // Note: this is an L1, L2 etc buffer
      // each entry points to another index buffer
      // get buffer first entry points to
      // if k1 we get B1
      IndexBuffer* pIndexBufferN_1 = aEntryLN->pIndexBuffer();
      
      // get the key of the first entry
      // if ka get ka
      Key* pKeyLN = aEntryLN->aKey();
      
      // now get the first entry from the lower buffer
      // bet B1's first entry ka
      Entry* aEntryLN_1 = pIndexBufferN_1->pFirstEntry();
      
      // get the key of the first entry of the lower buffer
      // we get ka from B1
      Key* pKeyLN_1 = aEntryLN_1->aKey();

      // pKeyLN should be the same as pKeyLN_1
      // ka of B1 and ka of B3 should be the same
      bool bSame = pKeyLN->bJuxtaCompare(JC_SAME, aEntryLN_1->aKey(), m_pIndex->pIdxColVec());
      if ( ! bSame ) {
        return false;
      }
      
      // get the next entry at the level we are at
      aEntryLN = pIndexBuffer->pNextEntry(aEntryLN);
    }
  
    // move to the next buffer at the same level
    pIndexBuffer = pIndexBuffer->pNextBuffer();
  }
  
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* insert an entry for {key,id} into an index buffer at given level           */
/*----------------------------------------------------------------------------*/
void IndexData::vInsertEntry(IndexBuffer* pHigher, KeyWsp& xKeyWsp, Id xId, i8 iLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  // initialize the entry wsp
  EntWsp xEntWsp(xId,xKeyWsp);
  
  // find index buffer which contains the entry after which the new entry will
  // be inserted into, starting with given higher
  IndexBuffer* pIndexBuffer = pHigher->pPositionLevel(xKeyWsp, JC_AFTER, iLevel);
  
  // check if found index buffer has enough space
  i8 nEntryBytes = BufferItem::nBytesRequiredEntry(xEntWsp);
  if ( ! pIndexBuffer->bHasSpace(nEntryBytes) ) {
    
    // not enough space, so split the index buffer
    pIndexBuffer = pSplitBuffer(pIndexBuffer);
    
    // find position again
    pIndexBuffer = pIndexBuffer->pHigherLevel()->pPositionLevel(xKeyWsp, JC_AFTER, iLevel);
  }
  
  // assert we have a buffer
  ASSERT(pIndexBuffer);
  
  // insert entry into index buffer
  bool bResult = pIndexBuffer->bInsert(xEntWsp);
  ASSERT(bResult);
}

/*----------------------------------------------------------------------------*/
/* add first entry of fromBuffer into the toBuffer                            */
/*----------------------------------------------------------------------------*/
void IndexData::vInsertFirstEntry(IndexBuffer* pToBuffer, IndexBuffer* pFromBuffer)
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
  
  // form a keyWsp from the key
  KeyWsp xFirstKeyWsp;
  xFirstKeyWsp.vAddKey(aKey, m_pIndex->pIdxColVec());
  
  vInsertEntry(pToBuffer, xFirstKeyWsp, xNewId, pToBuffer->nBufferLevel());
}

/*----------------------------------------------------------------------------*/
/* fix the higher level buffer in the given index buffer                      */
/*----------------------------------------------------------------------------*/
void IndexData::vFixHigherBuffer(IndexBuffer* pIndexBuffer, i8 iLevel)
{
   _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  KeyWsp xKeyWsp;
  IndexBuffer* pRoot;
  
  // get first entry keywsp from first buffer
  Entry* aFirstEntry = pIndexBuffer->pNextEntry(pIndexBuffer->pHead());

  // form a keyWsp from the key
  xKeyWsp.vAddKey(aFirstEntry->aKey(), m_pIndex->pIdxColVec());
  
  pRoot = m_pRoot->pPositionLevel(xKeyWsp, JC_AFTER, iLevel);

  pIndexBuffer->vHigherLevel(pRoot);
}

/*----------------------------------------------------------------------------*/
/* split a buffer                                                             */
/*----------------------------------------------------------------------------*/
IndexBuffer* IndexData::pSplitBuffer(IndexBuffer* pIndexBuffer)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  IndexBuffer* pRoot;
  IndexBuffer* pNewRoot;
  
  KeyWsp xFirstKeyWsp1;
  KeyWsp xFirstKeyWsp2;
  
  // split the index buffer
  IndexBuffer* pIndexBufferNew = pIndexBuffer->pSplitBuffer();
  
  // get the next level buffer
  IndexBuffer* pHigherLevel = pIndexBuffer->pHigherLevel();
  
  // create a next level buffer if it does not exist
  if ( ! pHigherLevel ) {
    
    i8 nHigherLevel = pIndexBuffer->nBufferLevel() + 1;
    pRoot = new IndexBuffer(m_pIndex, nHigherLevel);
    
    // insert first entry in new root from buffer we split
    vInsertFirstEntry(pRoot, pIndexBuffer);
    
    // root now points to the new higher level
    m_pRoot = pRoot;
  }
  else {
    pRoot = pHigherLevel;
  }
  
  //---------------------------------------------------------
  // insert the first entry in root buffer from the new IndexBuffer
  vInsertFirstEntry(pRoot, pIndexBufferNew);
  
  //---------------------------------------------------------
  // fix the first index buffer's higher level buffer
  vFixHigherBuffer(pIndexBuffer, pRoot->nBufferLevel());
  
  //---------------------------------------------------------
  // fix the second index buffer's higher level buffer
  vFixHigherBuffer(pIndexBufferNew, pRoot->nBufferLevel());
  
  return pIndexBuffer;
}

/*----------------------------------------------------------------------------*/
/* visualize the index                                                        */
/*----------------------------------------------------------------------------*/
void IndexData::vDumpLevel(IndexBuffer* pIndexBuffer)
{
  
  // get the first entry
  Entry* pEntry = pIndexBuffer->pFirstEntry();
  
  while (pEntry) {
    
    // get the index buffer the first enetry poinst to
    IndexBuffer* pEntryIndexBuffer = pEntry->pIndexBuffer();
    
    // print the decile of the buffer
    vTrace("%lld"_Str, pEntryIndexBuffer->iDecile());
    
    // insert spaces for no of entries in buffer
    if (pEntryIndexBuffer->nBufferLevel() > 0) {
      
      // exclude the head entry
      i8 iEntries = pEntryIndexBuffer->nEntries() - 1;
      
      while (iEntries--)
        vTrace(" "_Str);
    }
    
    // move to the next entry
    pEntry = pIndexBuffer->pNextEntry(pEntry);
  }
}

/*----------------------------------------------------------------------------*/
/* visualize the index                                                        */
/*----------------------------------------------------------------------------*/
void IndexData::vVisualize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  // start with the root buffer
  IndexBuffer* pIndexBuffer = m_pRoot;
  
  // print the decile of the root buffer
  vTrace("%lld\n"_Str, pIndexBuffer->iDecile());
  
  i8 iBufferLevel = pIndexBuffer->nBufferLevel();
  
  // if there is only one buffer i.e root is at level 0, we are done
  if (iBufferLevel == 0) {
    vTrace("\n"_Str);
    return;
  }
  
  // iterate through the buffers
  while (pIndexBuffer) {
    
    // dump this buffer
    vDumpLevel(pIndexBuffer);

    // if we are at level 1, iterate thru next buffers
    if (pIndexBuffer->nBufferLevel() == 1) {
      pIndexBuffer = pIndexBuffer->pNextBuffer();
    }
    else {
      vTrace("\n"_Str);
      
      // get the first entry of the buffer
      Entry* pEntry = pIndexBuffer->pFirstEntry();
      
      // get the index buffer this entry points to
      pIndexBuffer = pEntry->pIndexBuffer();
    }
  }
  
  vTrace("\n"_Str);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* IndexData::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
