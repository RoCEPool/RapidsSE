/*============================================================================*/
/* BPA.hpp -- buffer pool analyzer                                            */
/*============================================================================*/
#pragma once

namespace Pool
{
  class Heap;
  
  class BPA
  {
  public:
    
    static void vBufferDump(PoolBuffer* pBuffer);
    static void vEpochDump(s48K& sPortrait);
    static void vEpochTake(void);
    static void vHeapDump(Heap* pBuffer);
    static void vHeapDump(Heap* pBuffer, i8 iEpoch);
    static void vObjectList(void);
    static void vObjectSummary(void);
    static void vObjectSnapshot(i8* pCountObjects);
    static void vMonitorPortrait(s48K& sPortrait);
    static void vPoolAnalyze(void);
    static void vPoolDump(void);
    static i8   iPoolFree(void);
    static void vReadBuffers(Stz* sFilePath);
    static void vRegionSummary(void);
    static void vResourceSummary(void);
    static void vWordList(i8 iEpoch);
    static void vWriteBuffers(Stz* sFilePath);
    static void vWriteHeap(Heap* pHeap, File* pFile);
    static void vWritePersistentHeapMem(Stz* sFilePath);
  };
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
