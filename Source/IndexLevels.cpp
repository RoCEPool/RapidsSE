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
/* insert a row                                                               */
/*----------------------------------------------------------------------------*/
IndexBuffer* IndexLevels::pFirstBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXLEVELS,_FOOTER_);
  
  return m_pIndexBuffer[0];
}

/*----------------------------------------------------------------------------*/
/* insert a row                                                               */
/*----------------------------------------------------------------------------*/
void IndexLevels::vInsertEntry(KeyWsp& xKeyWsp, Id xId, IdxColVec* pColVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXLEVELS,_FOOTER_);
  
  EntWsp xEntWsp;
  
  // initialize the EntWsp
  xEntWsp.vId(xId);
  xEntWsp.vAddKey(xKeyWsp);
  
  // get the size of the EntWsp
  i8 nBytes = Entry::nBytesRequired(xEntWsp);
  
  if (!m_pIndexBuffer[0])
    m_pIndexBuffer[0] = new IndexBuffer(0);

  bool bResult = false;
  
  // start with level 0
  IndexBuffer* pIndexBuffer = m_pIndexBuffer[0];
  
  // assuming IndexBuffer[0] is the buffer to insert into
  
  // check if IndexBuffer to insert into has space
  if ( ! pIndexBuffer->bHasSpace(nBytes) ) {
    
    // split the index buffer
    vSplitBuffer(pIndexBuffer, pColVec);
    
    // find buffer into which to insert

  }
  
  // insert entry into index buffer
  bResult = pIndexBuffer->bInsert(xKeyWsp, xId, pColVec);
  ASSERT(bResult);
}

/*----------------------------------------------------------------------------*/
/* insert an entry into an L1 buffer                                          */
/*----------------------------------------------------------------------------*/
void IndexLevels::vInsertEntryL1(IndexBuffer* pIndexBuffer, IdxColVec* pColVec)
{
  Id  xNewId;
  
  // create an L1 buffer if it does not exist
  if ( ! m_pIndexBuffer[1] )
    m_pIndexBuffer[1] = new IndexBuffer(1);

  // form the Id
  bn nFirstBuffer = pIndexBuffer->nBufferNo();
  xNewId.vBuffer(*nFirstBuffer);
  xNewId.vRow(C_L1_ID);

  // get the first entry from the first buffer
  Entry* pHead = pIndexBuffer->aHead();
  Entry* pFirstEntry = pIndexBuffer->aNextEntry(pHead);

  // get the key from teh first entry
  Key* aKey = pFirstEntry->aKey();

  // form a keyWsp from the key
  KeyWsp xFirstKeyWsp;
  xFirstKeyWsp.vAddKey(aKey, pColVec);
  
  // insert the keywsp in teh IndexBuffer at L1
  bool bResult = m_pIndexBuffer[1]->bInsert(xFirstKeyWsp, xNewId, pColVec);
  ASSERT(bResult);
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
  
  
  ASSERT ( m_pIndexBuffer[0] );
  
  IndexBuffer* pIndexBuffer = m_pIndexBuffer[0];
  
  return pIndexBuffer->aPosition(xKey, pOpAryLower, pColumnVec);
}

/*----------------------------------------------------------------------------*/
/* split a buffer                                                             */
/*----------------------------------------------------------------------------*/
void IndexLevels::vSplitBuffer(IndexBuffer* pIndexBuffer, IdxColVec* pColVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXLEVELS,_FOOTER_);
  
  vTrace("-------------------------------------------------------------\n"_Str);
  vTrace("IndexBuffer before split\n"_Str);
  pIndexBuffer->vShow();

  // split the index buffer
  IndexBuffer* pIndexBufferNew = pIndexBuffer->pSplitBuffer(pColVec);

  // old buffer
  vTrace("IndexBuffers after split\n"_Str);
  pIndexBuffer->vShow();

  pIndexBufferNew->vShow();
  
  ASSERT( ! m_pIndexBuffer[1] );
  
  // insert first entry in L1 from IndexBuffer
  vInsertEntryL1(pIndexBuffer, pColVec);
  ASSERT(m_pIndexBuffer[1]);

  // insert the second entry in L1 from the new IndexBuffer
  vInsertEntryL1(pIndexBufferNew, pColVec);
  
  m_pIndexBuffer[1]->vShow();
  vTrace("-------------------------------------------------------------\n"_Str);
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
