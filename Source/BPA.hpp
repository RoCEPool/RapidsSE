/*============================================================================*/
/* BPA.hpp -- buffer pool analyzer                                            */
/*============================================================================*/
#pragma once

namespace Utilities
{
  class BPA : public Object
  {
  public:
    i8  m_anObjectsInPool[CN_ENUMS];

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
 
    BPA(void);
   ~BPA(void) noexcept(false);

  public:
    static void vBufferDump(PoolBuffer* pBuffer);
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
  
    void vEpochDump(s48K& sPortrait);
    void vEpochTake(void);

    Footer* aFooter(void);
  };

  BPA* _pBPA(void* aAddr);
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
