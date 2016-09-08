/*============================================================================*/
/* AttachMap.cpp - map of addresses when process attaches                     */
/*============================================================================*/

#include "StorageEngine.h"

extern BufferPool*           gpBufferPool;

extern Heap*                 gpMediumHeap;
extern Dictionary*           gpMainDic;

extern Grid*                 gpGrid;
extern TaskList*             gpTaskList;
extern SEitfList*            gpSEitfList;
extern Arguments             gxArguments;

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
  
  Memory::vCheckInitialised(this,sizeof(AttachMap));
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
  gpMainDic          = &m_xMainDic;
  
  gpGrid             = m_pGrid;
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
  
  m_iRegionSize       = gpBufferPool->iRegionSize();
  m_iPartitions       = gxArguments.m_iPartitions;
  
  m_pBufferPool       = gpBufferPool;
  
  m_pMediumHeap       = gpMediumHeap;
  m_pSmallHeap        = gpSmallHeap;
  
  m_pGrid             = gpGrid;
  m_pTaskList         = gpTaskList;
  m_pSEitfList        = gpSEitfList;
  
  m_iStop             = 0;
  m_iQuit             = 0;
}

/*----------------------------------------------------------------------------*/
/* dump this attachmap                                                        */
/*----------------------------------------------------------------------------*/
void AttachMap::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  AttachMap* pAttachMap = reinterpret_cast<AttachMap*>(pObject);
  ASSERT(pAttachMap->eClass() == CN_ATTACHMAP);
  
  vTrace("%sAttachMap %p:\n", pTab, pAttachMap);
  vTrace("%s  pid %lld\n", pTab, pAttachMap->m_iPid);
  vTrace("%s  utc %s\n", pTab, pAttachMap->m_sUTC.a());
  vTrace("%s  regionSize %lld\n", pTab, pAttachMap->m_iRegionSize);
  vTrace("%s  partitions %lld\n\n", pTab, pAttachMap->m_iPartitions);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* AttachMap::pFooter(void)
{
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/