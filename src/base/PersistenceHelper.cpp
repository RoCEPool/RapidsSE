/*============================================================================*/
/* PersistenceHelper.cpp -- Persistence Helper                                */
/*============================================================================*/

#include "BASE.h"
#include "Op.hpp"
#include "Stream.hpp"
#include "CmdLineArgs.hpp"

extern SizeHeap     gxaSizeHeap[];
extern ClassCatalog gxClassCatalog;
extern CmdLineArgs  gxCmdLineArgs;

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
/* disk write speed test                                                      */
/*----------------------------------------------------------------------------*/
void PersistenceHelper::vWriteDiskTest(Stz* sFilePath, i8 iNoOfGb)
{
  _static_

  i8 iSize = C_1GB * iNoOfGb;
  char *azBuffer = new char [iSize];
  for(i8 i = 0; i < iSize; i++)
    azBuffer[i] = RndLib::iInteger(0,100);

  u8 start;
  start = Hardware::uReadClock();

  Timer oTimer;
  oTimer.vStart();

  i8 iPtnOfNode = -1;
  if(MY::pOp()) {
    iPtnOfNode = MY::pOp()->pStream()->i_PtnOfNode();
  }

  File* pFile = new(C_VIA_MALLOC) File(nullptr);
  pFile->bOpenWrite(sFilePath);

  void* pIter = azBuffer;

  for (i8 nBytesWritten = 0;
       nBytesWritten < iSize;
       ) {
    bool bResult = pFile->bWriteBytes(pIter,C_1MB);
    ASSERT(bResult);
    UNUSED(bResult);
    nBytesWritten += C_1MB;
    pIter = aAdd<void *>(pIter, C_1MB);
  }

  pFile->vSync();// Try fdatasync()

  pFile->vClose();
  delete pFile;

  oTimer.vUpdate();
  vTrace("Partition %lld took %lld ms to write to disk\n"_Stz,
         iPtnOfNode,
         oTimer.iMilliseconds());

  vTrace("New Timer: Partition %lld took %lld ms to write to disk\n"_Stz,
         iPtnOfNode,
         (Hardware::uReadClock() - start) / 1000000);

}

/*----------------------------------------------------------------------------*/
/* write given heap to file                                                   */
/*----------------------------------------------------------------------------*/
void PersistenceHelper::vWriteHeap(Heap* pHeap, File* pFile)
{
  _static_

  pHeap->vGrabWriteLock();
  bool bResult = pFile->bWriteBytes(pHeap,C_SIZE_POOL_BUFFER);
  ASSERT(bResult);
  UNUSED(bResult);
  pHeap->vDropWriteLock();

}
/*----------------------------------------------------------------------------*/
/* write given heap's HeapBuffers to file                                     */
/*----------------------------------------------------------------------------*/
void PersistenceHelper::vWriteHeapBuffers(Heap* pHeap, File* pFile)
{
  _static_

  i8 iNumPtn = gxCmdLineArgs.m_xSysArgs.m_nPtnsPerNode;

  i8 iPtnOfNode = -1;
  if(MY::pOp()) {
    iPtnOfNode = MY::pOp()->pStream()->i_PtnOfNode();
  }

  ASSERT(iPtnOfNode >= 0);
  
  pHeap->vGrabReadLock();

  i8 nBuffCount = 0;
  for (HeapBuffer* pHeapBuffer = pHeap->pFirstBuffer();
       pHeapBuffer;
       pHeapBuffer = pHeap->pNextBuffer(pHeapBuffer), nBuffCount++) {
    i8 mod = nBuffCount % iNumPtn;
    if(mod == iPtnOfNode) {
      bool bResult = pFile->bWriteBytes(pHeapBuffer,C_SIZE_POOL_BUFFER);
      ASSERT(bResult);
      UNUSED(bResult);
    }
  }
  
  pHeap->vDropReadLock();
}

/*----------------------------------------------------------------------------*/
/* write all the heaps to given file                                          */
/*----------------------------------------------------------------------------*/
void PersistenceHelper::vWritePersistentHeaps(Stz* sFilePath)
{
  _static_

  i8 iPtnOfNode = -1;
  if(MY::pOp()) {
    iPtnOfNode = MY::pOp()->pStream()->i_PtnOfNode();
  }

  ASSERT(iPtnOfNode >= 0);

  Timer oTimer;
  oTimer.vStart();

  File* pFile = new(C_VIA_MALLOC) File(nullptr);
  pFile->bOpenWrite(sFilePath);

  if(iPtnOfNode == 0)
  {
    vWriteHeap(AM::pHeaps()->Persistent.m_pTiny, pFile);
    vWriteHeap(AM::pHeaps()->Persistent.m_pSmall, pFile);
    vWriteHeap(AM::pHeaps()->Persistent.m_pMedium, pFile);
    vWriteHeap(AM::pHeaps()->Persistent.m_pLarge, pFile);
  }

//  vTrace("Writing persistent tiny heap\n"_Stz);
  vWriteHeapBuffers(AM::pHeaps()->Persistent.m_pTiny, pFile);
  
//  vTrace("Writing persistent small heap\n"_Stz);
  vWriteHeapBuffers(AM::pHeaps()->Persistent.m_pSmall, pFile);
  
//  vTrace("Writing persistent medium heap\n"_Stz);
  vWriteHeapBuffers(AM::pHeaps()->Persistent.m_pMedium, pFile);
  
//  vTrace("Writing persistent large heap\n"_Stz);
  vWriteHeapBuffers(AM::pHeaps()->Persistent.m_pLarge, pFile);

  pFile->vSync();

  pFile->vClose();
  delete pFile;

  oTimer.vUpdate();
  vTrace("Partition %lld took %lld ms to write heap buffers to disk\n"_Stz,
         iPtnOfNode,
         oTimer.iMilliseconds());
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
