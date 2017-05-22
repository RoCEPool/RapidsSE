
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
  
  i8 nBuffers = 0;
  
  IndexBuffer* pIndexBuffer = m_pRoot;
  
  while (pIndexBuffer) {
    nBuffers++;
    pIndexBuffer = pIndexBuffer->pNextBuffer();
  }
  
  return nBuffers;
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
  
  if ( ! bVerify() ) {
    vTrace("Inserting row that caused verify to fail:\n"_Str);
    pRow->vContentDump();
    vDump();
    ASSERT(0);
  }
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
  
  IndexBuffer* pIndexBuffer = m_pRoot;
  IndexBuffer* pNextIndexBuffer;
  
  while(pIndexBuffer) {
    pNextIndexBuffer = pIndexBuffer->pNextBuffer();
    delete pIndexBuffer;
    pIndexBuffer = pNextIndexBuffer;
  }
  
  m_pRoot = nullptr;
  
  m_zLock.vDropWrite();

}

/*----------------------------------------------------------------------------*/
/* verify                                                                     */
/*----------------------------------------------------------------------------*/
bool IndexData::bVerify(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);

  // start with the root buffer
  IndexBuffer* pIndexBuffer = m_pRoot;
  
  while (pIndexBuffer) {
    
    // first just run verify on its entries
    bool bVerify = pIndexBuffer->bVerify();
    if ( ! bVerify ) {
      return false;
    }
    
    if (pIndexBuffer->nBufferLevel() == 0) {
      pIndexBuffer = pIndexBuffer->pNextBuffer();
      continue;
    }
    
    // higher level buffer get first entry
    Entry* aEntryLN = pIndexBuffer->pNextEntry(pIndexBuffer->pHead());
    
    while (aEntryLN) {
      
      // get buffer first entry points to
      IndexBuffer* pIndexBufferN_1 = aEntryLN->pIndexBuffer();
      
      // get the key of the first entry points to
      Key* pKeyLN = aEntryLN->aKey();
      
      // get the first entry from the lower buffer
      Entry* aEntryLN_1 = pIndexBufferN_1->pNextEntry(pIndexBufferN_1->pHead());
      
      // get the key of the first entry points to
      Key* pKeyLN_1 = aEntryLN_1->aKey();

      bool bSame = pKeyLN->bJuxtaCompare(JC_SAME, aEntryLN_1->aKey(), m_pIndex->pIdxColVec());
      if ( ! bSame ) {
        return false;
      }
      
      aEntryLN = pIndexBuffer->pNextEntry(aEntryLN);
    }
  
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
  i8 nEntryBytes = Entry::nBytesRequired(xEntWsp);
  if ( ! pIndexBuffer->bHasSpace(nEntryBytes) ) {
    
    // not enough space, so split the index buffer
    pIndexBuffer = pSplitBuffer(pIndexBuffer);
    
    // find position again
    pIndexBuffer = pIndexBuffer->pHigherLevel()->pPositionLevel(xKeyWsp, JC_AFTER, iLevel);
  }
  
  // assert we have a buffer
  ASSERT(pIndexBuffer);
  
  // insert entry into index buffer
  bool bResult = pIndexBuffer->bInsert(xKeyWsp, xId);
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
