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
void IndexLevels::vInsertEntry(EntWsp& xEntWsp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_INDEXLEVELS,_FOOTER_);
  
  
  if (!m_pIndexBuffer[0])
    m_pIndexBuffer[0] = new IndexBuffer;

  bool bResult = false;
  IndexBuffer* pIndexBuffer = m_pIndexBuffer[0];
  
  while ( ! bResult) {
    pIndexBuffer = pIndexBuffer->pFindBuffer(xEntWsp);
 
    // insert entry into index buffer
    bResult = pIndexBuffer->bInsertEntry(xEntWsp);
  }
  
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
