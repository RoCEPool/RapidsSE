/*============================================================================*/
/* IndexLevels.cpp - set of index buffers by level for an index partition     */
/*============================================================================*/

#include "DBobjects.h"

extern BufferPool*  gpBufferPool;

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump data                                                                  */
/*----------------------------------------------------------------------------*/
void IndexLevels::vDump(eCN eClass, Object* pObject, Str* aTab)
{
  ASSERT(eClass == CN_INDEXLEVELS);
  ASSERT(pObject->eClass() == CN_INDEXLEVELS);
  
  Data* pData = reinterpret_cast<Data*>(pObject);
}

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* IndexLevels::operator new(size_t iSize)
{
  RASSERT(iSize);
  void* pObject = Heap::pAllocate(CN_INDEXLEVELS);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void IndexLevels::operator delete(void* pIndexLevelsAddress)
{
  RASSERT(pIndexLevelsAddress);
  Buffer* pBuffer = static_cast<Buffer*>(pIndexLevelsAddress);
  
  Heap::vFree(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
IndexLevels::IndexLevels(void)
: Object(CN_INDEXLEVELS)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXLEVELS,_FOOTER_);
  
  m_zLock.vInit("IndexLevels::m_zLock"_Str);
  
  // start with a root buffer at L0
  m_pRoot = new IndexBuffer(0);
  
  for (i8 iLevel = 0; iLevel < C_LEVELS_PER_INDEX; iLevel++)
    m_pIndexBuffer[iLevel] = nullptr;
  
  _checkInitialised_(CN_INDEXLEVELS);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_INDEXLEVELS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
IndexLevels::~IndexLevels(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXLEVELS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump the index buffers                                                     */
/*----------------------------------------------------------------------------*/
void IndexLevels::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXLEVELS,_FOOTER_);
  
  IndexBuffer* pIndexBuffer = m_pRoot;
  
  m_pRoot->vDump();
}

/*----------------------------------------------------------------------------*/
/* insert a row                                                               */
/*----------------------------------------------------------------------------*/
IndexBuffer* IndexLevels::pFirstBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXLEVELS,_FOOTER_);
  
  return m_pRoot;
}

/*----------------------------------------------------------------------------*/
/* insert a row                                                               */
/*----------------------------------------------------------------------------*/
void IndexLevels::vInsertEntry(KeyWsp& xKeyWsp, Id xId, IdxColVec* pColVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXLEVELS,_FOOTER_);
  
  IndexBuffer* pRoot = pInsertEntry(m_pRoot, xKeyWsp, xId, pColVec, 0);
  
  if (m_pRoot->nBufferLevel() < pRoot->nBufferLevel())
    m_pRoot = pRoot;
}

/*----------------------------------------------------------------------------*/
/* insert a row                                                               */
/*----------------------------------------------------------------------------*/
IndexBuffer* IndexLevels::pInsertEntry(IndexBuffer* pHigher, KeyWsp& xKeyWsp, Id xId, IdxColVec* pColVec, i8 iLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXLEVELS,_FOOTER_);
  
  EntWsp xEntWsp;
  bool   bResult = false;
  IndexBuffer* pRoot = pHigher;
  
  // initialize the EntWsp
  xEntWsp.vId(xId);
  xEntWsp.vAddKey(xKeyWsp);
  
  IndexBuffer* pOldRoot = m_pRoot;
  
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
  //ASSERT(pIndexBuffer->nBufferLevel() == 0);
  bResult = pIndexBuffer->bInsert(xKeyWsp, xId, pColVec);
  ASSERT(bResult);
  
  return pRoot;
}

/*----------------------------------------------------------------------------*/
/* insert an entry into the root buffer                                       */
/*----------------------------------------------------------------------------*/
IndexBuffer* IndexLevels::pInsertEntry(IndexBuffer* pRoot, IndexBuffer* pIndexBuffer, IdxColVec* pColVec)
{
  Id  xNewId;

  IndexBuffer* pNewRoot = pRoot;
  
  ASSERT(pRoot->nBufferLevel() == pIndexBuffer->nBufferLevel() + 1);
  // form the Id
  bn nFirstBuffer = pIndexBuffer->nBufferNo();
  xNewId.vBuffer(*nFirstBuffer);
  xNewId.vRow(C_HIGHER_ID);

  // get the first entry from the first buffer
  Entry* pHead = pIndexBuffer->aHead();
  Entry* pFirstEntry = pIndexBuffer->aNextEntry(pHead);

  // get the key from the first entry
  Key* aKey = pFirstEntry->aKey();

  // form a keyWsp from the key
  KeyWsp xFirstKeyWsp;
  xFirstKeyWsp.vAddKey(aKey, pColVec);
 
  pRoot = pInsertEntry(pRoot, xFirstKeyWsp, xNewId, pColVec, pRoot->nBufferLevel());
  
  return pRoot;
}

/*----------------------------------------------------------------------------*/
/* insert a row                                                               */
/*----------------------------------------------------------------------------*/
Entry* IndexLevels::pNextEntry(Entry* pEntry)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXLEVELS,_FOOTER_);
  
  IndexBuffer* pIndexBuffer = m_pIndexBuffer[0];
  
  return pIndexBuffer->aNextEntry(pEntry);
}

/*----------------------------------------------------------------------------*/
/* position index at given key                                                */
/*----------------------------------------------------------------------------*/
Entry* IndexLevels::aPosition(Key& xKey, RelOpAry* pOpAryLower, IdxColVec* pColumnVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXLEVELS,_FOOTER_);
  
  ASSERT ( m_pRoot );
  
  IndexBuffer* pIndexBuffer = m_pRoot;
  Entry* pEntry = pIndexBuffer->aPosition(xKey, pOpAryLower, pColumnVec);
  
  return pEntry;
}

/*----------------------------------------------------------------------------*/
/* position at an index buffer in level 0                                     */
/*----------------------------------------------------------------------------*/
IndexBuffer* IndexLevels::aPositionLevel(KeyWsp& xKeyWsp, RelOpAry* pOpAryGreater, IdxColVec* pColVec, i8 iLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXLEVELS,_FOOTER_);  
  
  ASSERT ( m_pRoot );

  return aPositionL(m_pRoot, xKeyWsp, pOpAryGreater, pColVec, iLevel);
}

/*----------------------------------------------------------------------------*/
/* position at an index buffer in level 0                                     */
/*----------------------------------------------------------------------------*/
IndexBuffer* IndexLevels::aPositionLevel(KeyWsp& xKeyWsp, IdxColVec* pColVec, i8 iLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXLEVELS,_FOOTER_);
  
  ASSERT ( m_pRoot );
  
  // create a relational operator array
  RelOpAry* pOpAryGreater = new RelOpAry;
  pOpAryGreater->vAdd(RO_GREATER_EQUAL);
  
  IndexBuffer* pIndexBuffer =  aPositionL(m_pRoot, xKeyWsp, pOpAryGreater, pColVec, iLevel);
  
  delete pOpAryGreater;
  return pIndexBuffer;
}

/*----------------------------------------------------------------------------*/
/* truncate all levels                                                        */
/*----------------------------------------------------------------------------*/
void IndexLevels::vTruncate(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXLEVELS,_FOOTER_);
  
  m_zLock.vGrabWrite();
  
  IndexBuffer* pIndexBuffer = m_pIndexBuffer[0];
  IndexBuffer* pNextIndexBuffer;
  
  while(pIndexBuffer) {
    pNextIndexBuffer = pIndexBuffer->pNextBuffer();
    delete pIndexBuffer;
    pIndexBuffer = pNextIndexBuffer;
  }
  
  m_pIndexBuffer[0] = nullptr;
  
  m_zLock.vDropWrite();
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* position at an index buffer in level 0                                     */
/*----------------------------------------------------------------------------*/
IndexBuffer* IndexLevels::aPositionL(IndexBuffer* pRoot, KeyWsp& xKeyWsp, RelOpAry* pOpAryGreater, IdxColVec* pColVec, i8 iLevel)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXLEVELS,_FOOTER_);

  
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
/* split a buffer                                                             */
/*----------------------------------------------------------------------------*/
IndexBuffer* IndexLevels::pSplitBuffer(IndexBuffer* pIndexBuffer, IdxColVec* pColVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXLEVELS,_FOOTER_);
  
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
    pRoot = pInsertEntry(pRoot, pIndexBuffer, pColVec);
    
    // root now points to the new higher level
    m_pRoot = pRoot;
  }
  else {
    pRoot = pHigherLevel;
  }

  // insert the second entry in root buffer from the new IndexBuffer
  pRoot = pInsertEntry(pRoot, pIndexBufferNew, pColVec);
  
  
  //---------------------------------------------------------
  // fix the first index buffer's higher level buffer
  KeyWsp xKeyWsp1;
  
  // get first entry keywsp from first buffer
  Entry* pFirstEntry = pIndexBuffer->aNextEntry(pIndexBuffer->aHead());
  
  // form a keyWsp from the key
  xKeyWsp1.vAddKey(pFirstEntry->aKey(), pColVec);
  
  pNewRoot = aPositionLevel(xKeyWsp1, pColVec, pRoot->nBufferLevel());
  pIndexBuffer->vHigherLevel(pNewRoot);

  //---------------------------------------------------------
  // fix the second index buffer's higher level buffer
  KeyWsp xKeyWsp2;
  
  // get first entry keywsp from first buffer
  pFirstEntry = pIndexBufferNew->aNextEntry(pIndexBufferNew->aHead());
  
  // form a keyWsp from the key
  xKeyWsp2.vAddKey(pFirstEntry->aKey(), pColVec);
  
  pNewRoot = aPositionLevel(xKeyWsp2, pColVec, pRoot->nBufferLevel());
  pIndexBufferNew->vHigherLevel(pNewRoot);
  
  
  return pIndexBuffer;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* IndexLevels::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXLEVELS);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
