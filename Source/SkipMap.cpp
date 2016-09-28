/*============================================================================*/
/* SkipMap.hpp -- Skip Map                                                    */
/*============================================================================*/

#include "DBobjects.h"

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* SkipMap::operator new(size_t iSize)
{
  RASSERT(iSize);

  Buffer* pBuffer = gpBufferPool->pAllocBuffer();
  
  return pBuffer;
}

/*----------------------------------------------------------------------------*/
/* return a segment to the buffer pool                                        */
/*----------------------------------------------------------------------------*/
void SkipMap::operator delete(void* pSkipMapAddress)
{
  Buffer* pBuffer = static_cast<Buffer*>(pSkipMapAddress);
  
  gpBufferPool->vFree(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SkipMap::SkipMap(void)
: Buffer(CN_SKIPMAP)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPMAP);
  
  m_iLimit = 0;
  //m_zLock
  m_iNext  = 0;
  for (i8 i = 0; i < C_SKIPMAP_SIZE; i++)
    m_iSeek[i] = -1;

  _checkInitialised_(sizeof(CN_SKIPMAP));
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SKIPMAP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump dictionary                                                            */
/*----------------------------------------------------------------------------*/
void SkipMap::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_TX);
  ASSERT(pObject->eClass() == CN_TX);
  
  Tx* pTx = reinterpret_cast<Tx*>(pObject);
  
  vTrace("%sTx %p:\n", pTab, pTx);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SkipMap::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SKIPMAP);
  
  Footer* pFooter = &m_xFooter;
  
  return pFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/