/*============================================================================*/
/* AttachMap.cpp - map of addresses when process attaches                     */
/*============================================================================*/

#include "StorageEngine.h"

extern BufferPool*           gpBufferPool;
extern Heap*                 gpMediumHeap;
extern GlobalDictionary*     gpGlobalDictionary;
extern Dictionary*           gpDictionary;
extern TaskList*             gpTaskList;
extern SEitfList*            gpSEitfList;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* AttachMap::operator new(size_t iSize)
{
  Buffer* pBuffer = gpBufferPool->pAllocBuffer();
  
  return pPoolCheck<void*>(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void AttachMap::operator delete(void* pBufferAddress)
{
  Buffer* pBuffer = static_cast<Buffer*>(pBufferAddress);
  
  gpBufferPool->vFree(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
AttachMap::AttachMap(void)
: Buffer(CN_ATTACHMAP)
{
  // the attach map must always be the first buffer in the buffer pool
  ASSERT(nBufferNo() == 1_bn);
  
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
AttachMap::~AttachMap(void)
{
}

/*----------------------------------------------------------------------------*/
/* get all the needed globals                                                 */
/*----------------------------------------------------------------------------*/
void AttachMap::vGetGlobals(void)
{
  gpMediumHeap       = m_pMediumHeap;
  gpSmallHeap        = m_pSmallHeap;
  gpGlobalDictionary = m_pGlobalDictionary;
  gpDictionary       = m_pDictionary;
  gpTaskList         = m_pTaskList;
  gpSEitfList        = m_pSEitfList;
}

/*----------------------------------------------------------------------------*/
/* record all the needed globals                                              */
/*----------------------------------------------------------------------------*/
void AttachMap::vRecordGlobals(void)
{
  m_iPid              = Process::iGetPid();
  
  m_xSince.vSet();
  m_xTime.vSet(&m_xSince);
  m_xTime.vUTC(m_sUTC);
  
  m_iStop             = 0;
  m_iQuit             = 0;
  
  m_iPoolSize         = gpBufferPool->iPoolSize();

  m_pBufferPool       = gpBufferPool;
  m_pMediumHeap       = gpMediumHeap;
  m_pSmallHeap        = gpSmallHeap;
  m_pGlobalDictionary = gpGlobalDictionary;
  m_pDictionary       = gpDictionary;
  m_pTaskList         = gpTaskList;
  m_pSEitfList        = gpSEitfList;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/