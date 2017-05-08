
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
  m_pRoot = new IndexBuffer(0);
  
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
/* insert an entry for the row into the index                                 */
/*----------------------------------------------------------------------------*/
void IndexData::vInsertEntry(Row* pRow, i8 nRowNo, bn nBufferNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  Id xId;
  KeyWsp xKeyWsp;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // form the rowId
  xId.vBuffer(*nBufferNo);
  xId.vRow(nRowNo);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get the key from the row
  pRow->vGetKey(xKeyWsp, m_pIndex->pIdxColVec());
    
  // insert row entry in index at level 0
  vInsertEntry(m_pRoot, xKeyWsp, xId, m_pIndex->pIdxColVec(), 0);
}

/*----------------------------------------------------------------------------*/
/* position the indexData at aparticular level of index buffer                */
/*----------------------------------------------------------------------------*/
IndexBuffer* IndexData::aPosition(KeyWsp& xKeyWsp, RelOpAry* pOpAryGreater, i8 iLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  return aPositionL(m_pRoot, xKeyWsp, pOpAryGreater, m_pIndex->pIdxColVec(), iLevel);
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
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* insert an entry for a row in the index buffer                              */
/*----------------------------------------------------------------------------*/
void IndexData::vInsertEntry(IndexBuffer* pHigher, KeyWsp& xKeyWsp, Id xId, IdxColVec* pColVec, i8 iLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  EntWsp xEntWsp;
  bool   bResult = false;
  
  // initialize the EntWsp
  xEntWsp.vId(xId);
  xEntWsp.vAddKey(xKeyWsp);
  
  // get the size of the EntWsp
  i8 nBytes = Entry::nBytesRequired(xEntWsp);
  
  // find buffer to insert into, starting with root
  IndexBuffer* pIndexBuffer = nullptr;
  
  // create a relational operator array
  RelOpAry* pOpAryFind = new RelOpAry;
  pOpAryFind->vAdd(RO_GREATER_EQUAL);
  
  pIndexBuffer = aPositionL(pHigher, xKeyWsp, pOpAryFind, pColVec, iLevel);
  
  // check if IndexBuffer to insert into has space
  if ( ! pIndexBuffer->bHasSpace(nBytes) ) {
    
    // split the index buffer
    pIndexBuffer = pSplitBuffer(pIndexBuffer, pColVec);
    
    // find position again
    pIndexBuffer = aPositionL(pIndexBuffer->pHigherLevel(), xKeyWsp, pOpAryFind, pColVec, iLevel);
  }
  
  // assert we have a buffer
  ASSERT(pIndexBuffer);
  
  // insert entry into index buffer
  bResult = pIndexBuffer->bInsert(xKeyWsp, xId, pColVec);
  ASSERT(bResult);
}

/*----------------------------------------------------------------------------*/
/* add first entry of fromBuffer into the toBuffer                            */
/*----------------------------------------------------------------------------*/
void IndexData::vInsertFirstEntry(IndexBuffer* pToBuffer, IndexBuffer* pFromBuffer, IdxColVec* pColVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);

  Id  xNewId;
  
  ASSERT(pToBuffer->nBufferLevel() == pFromBuffer->nBufferLevel() + 1);
  // form the Id
  bn nFirstBuffer = pFromBuffer->nBufferNo();
  xNewId.vBuffer(*nFirstBuffer);
  xNewId.vRow(C_HIGHER_LEVEL_ID);
  
  // get the first entry from the first buffer
  Entry* pHead = pFromBuffer->aHead();
  Entry* pFirstEntry = pFromBuffer->aNextEntry(pHead);
  
  // get the key from the first entry
  Key* aKey = pFirstEntry->aKey();
  
  // form a keyWsp from the key
  KeyWsp xFirstKeyWsp;
  xFirstKeyWsp.vAddKey(aKey, pColVec);
  
  vInsertEntry(pToBuffer, xFirstKeyWsp, xNewId, pColVec, pToBuffer->nBufferLevel());
}

/*----------------------------------------------------------------------------*/
/* position at an index buffer at given level                                 */
/*----------------------------------------------------------------------------*/
IndexBuffer* IndexData::aPositionLevel(KeyWsp& xKeyWsp, IdxColVec* pColVec, i8 iLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  ASSERT ( m_pRoot );
  
  // create a relational operator array
  RelOpAry* pOpAryGreater = new RelOpAry;
  pOpAryGreater->vAdd(RO_GREATER_EQUAL);
  
  IndexBuffer* pIndexBuffer =  aPositionL(m_pRoot, xKeyWsp, pOpAryGreater, pColVec, iLevel);
  
  delete pOpAryGreater;
  return pIndexBuffer;
}

/*----------------------------------------------------------------------------*/
/* position at an index buffer in level 0                                     */
/*----------------------------------------------------------------------------*/
IndexBuffer* IndexData::aPositionL(IndexBuffer* pRoot, KeyWsp& xKeyWsp, RelOpAry* pOpAryGreater, IdxColVec* pColVec, i8 iLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  
  IndexBuffer* pIndexBuffer = pRoot;
  i8 iBufferLevel;
  Entry* pEntry;
  
  Key xKey(xKeyWsp);
  
  while (true) {
    iBufferLevel = pIndexBuffer->nBufferLevel();
    
    if (iBufferLevel == iLevel)
      break;
    
    // get the entry
    pEntry = pIndexBuffer->aPosition(xKey, pOpAryGreater, pColVec);
    ASSERT( pEntry );
    
    // get the index buffer of the entry
    pIndexBuffer = pEntry->pIndexBuffer();
    ASSERT(pIndexBuffer);
    ASSERT(pIndexBuffer->eClass() == CN_INDEXBUFFER);
  }
  
  return pIndexBuffer;
}

/*----------------------------------------------------------------------------*/
/* fix the higher level buffer in the given index buffer                      */
/*----------------------------------------------------------------------------*/
void IndexData::vFixHigherBuffer(IndexBuffer* pIndexBuffer, IdxColVec* pColVec, i8 iLevel)
{
   _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  KeyWsp xKeyWsp;
  IndexBuffer* pRoot;
  
  // get first entry keywsp from first buffer
  Entry* pFirstEntry = pIndexBuffer->aNextEntry(pIndexBuffer->aHead());

  // form a keyWsp from the key
  xKeyWsp.vAddKey(pFirstEntry->aKey(), pColVec);

  pRoot = aPositionLevel(xKeyWsp, pColVec, iLevel);
  pIndexBuffer->vHigherLevel(pRoot);
}

/*----------------------------------------------------------------------------*/
/* split a buffer                                                             */
/*----------------------------------------------------------------------------*/
IndexBuffer* IndexData::pSplitBuffer(IndexBuffer* pIndexBuffer, IdxColVec* pColVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXDATA,_FOOTER_);
  
  IndexBuffer* pRoot;
  IndexBuffer* pNewRoot;
  KeyWsp xFirstKeyWsp1;
  KeyWsp xFirstKeyWsp2;
  
  // split the index buffer
  IndexBuffer* pIndexBufferNew = pIndexBuffer->pSplitBuffer(pColVec);
  
  // get the next level buffer
  IndexBuffer* pHigherLevel = pIndexBuffer->pHigherLevel();
  
  // create a next level buffer if it does not exist
  if ( ! pHigherLevel ) {
    
    i8 nHigherLevel = pIndexBuffer->nBufferLevel() + 1;
    pRoot = new IndexBuffer(nHigherLevel);
    
    // insert first entry in new root from buffer we split
    vInsertFirstEntry(pRoot, pIndexBuffer, pColVec);
    
    // root now points to the new higher level
    m_pRoot = pRoot;
  }
  else {
    pRoot = pHigherLevel;
  }
  
  //---------------------------------------------------------
  // insert the first entry in root buffer from the new IndexBuffer
  vInsertFirstEntry(pRoot, pIndexBufferNew, pColVec);
  
  //---------------------------------------------------------
  // fix the first index buffer's higher level buffer
  vFixHigherBuffer(pIndexBuffer, pColVec, pRoot->nBufferLevel());
  
  //---------------------------------------------------------
  // fix the second index buffer's higher level buffer
  vFixHigherBuffer(pIndexBufferNew, pColVec, pRoot->nBufferLevel());
  
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
