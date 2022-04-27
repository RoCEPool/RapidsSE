/*============================================================================*/
/* PersistenceHelper.cpp -- Persistence Helper                                */
/*============================================================================*/

#include "BASE.h"

extern SizeHeap     gxaSizeHeap[];
extern ClassCatalog gxClassCatalog;

/*----------------------------------------------------------------------------*/
/* read all the buffer pool buffers from file                                 */
/*----------------------------------------------------------------------------*/
void PersistenceHelper::vReadBuffers(Stz* sFilePath)
{
  _static_

  File* pFile = new File(nullptr);
  pFile->bOpenRead(sFilePath);
  
  PoolBuffer* pBuffer = new PoolBuffer(CN_HEAPBUFFER);
  
  while (true) {
    i8 nBytes = pFile->nReadBytes(pBuffer, C_SIZE_POOL_BUFFER);
    if (nBytes != C_SIZE_POOL_BUFFER)
      break;
    
    //vBufferDump(pBuffer);
  }
}

/*----------------------------------------------------------------------------*/
/* write all the buffer pool buffers to file                                  */
/*----------------------------------------------------------------------------*/
void PersistenceHelper::vWriteBuffers(Stz* sFilePath)
{
  _static_

  File* pFile = new File(nullptr);
  pFile->bOpenWrite(sFilePath);
  
  i8 iStart  = TickingDevice::iGetTick();
  
  for (i8 i_PoolBuffer = gpBufferPool->iBufferFirst();
       i_PoolBuffer <= gpBufferPool->iBufferLast();
       i_PoolBuffer++) {
    
    PoolBuffer* pBuffer = FileMap::pToPoolBuffer(i_PoolBuffer);
    bool bResult = pFile->bWriteBytes((void*)pBuffer,C_SIZE_POOL_BUFFER);
    ASSERT(bResult);
    UNUSED(bResult);
    
  }
  i8 iUpdate = TickingDevice::iGetTick();
  
  i8 iTicks = (iUpdate - iStart);
  i8 iTicksPerSec = TickingDevice::iTicksPerSec();
  
  i8 iMilli = iTicks * C_1K / iTicksPerSec;
  
  i8 nBuffers =gpBufferPool->iBufferLast() - 1;
  vTrace("Writing %lld buffers took %lld ms\n"_Stz, nBuffers, iMilli);
}

/*----------------------------------------------------------------------------*/
/* write given heap to file                                                   */
/*----------------------------------------------------------------------------*/
void PersistenceHelper::vWriteHeap(Heap* pHeap, File* pFile)
{
  _static_

  Timer oTimer;
  oTimer.vStart();
  
  pHeap->vGrabLock();
  
  bool bResult = pFile->bWriteBytes(pHeap,C_SIZE_POOL_BUFFER);
  ASSERT(bResult);
  
  for (HeapBuffer* pHeapBuffer = pHeap->pFirstBuffer();
       pHeapBuffer;
       pHeapBuffer = pHeap->pNextBuffer(pHeapBuffer)) {
    
    bResult = pFile->bWriteBytes(pHeapBuffer,C_SIZE_POOL_BUFFER);
    ASSERT(bResult);
  }
  
  pHeap->vDropLock();
  
  oTimer.vUpdate();
  vTrace("duration %lldms\n"_Stz,oTimer.iMilliseconds());
}

/*----------------------------------------------------------------------------*/
/* write all the heaps to given file                                          */
/*----------------------------------------------------------------------------*/
void PersistenceHelper::vWritePersistentHeaps(Stz* sFilePath)
{
  _static_

  File* pFile = new File(nullptr);
  pFile->bOpenWrite(sFilePath);
  
  vTrace("Writing persistent tiny heap\n"_Stz);
  vWriteHeap(AM::pHeaps()->Persistent.m_pTiny,pFile);
  
  vTrace("Writing persistent small heap\n"_Stz);
  vWriteHeap(AM::pHeaps()->Persistent.m_pSmall,pFile);
  
  vTrace("Writing persistent medium heap\n"_Stz);
  vWriteHeap(AM::pHeaps()->Persistent.m_pMedium,pFile);
  
  vTrace("Writing persistent large heap\n"_Stz);
  vWriteHeap(AM::pHeaps()->Persistent.m_pLarge,pFile);

  pFile->vClose();
  delete pFile;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
