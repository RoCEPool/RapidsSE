/*============================================================================*/
/* PoolHelper.hpp -- buffer pool analyzer                                            */
/*============================================================================*/
#pragma once

namespace Resources
{
  class PoolHelper
  {
  public:
    static void vBufferDump(PoolBuffer* pBuffer);
    static void vDumpObjects(i8 iAfter);
    static void vDumpObjects(i8 iAfter, Heap* pHeap);
    static void vHeapDump(Heap* pBuffer);
    static void vHeapDump(Heap* pBuffer, i8 iEpoch);
    static void vInspectObjects(i8 iAfter);
    static void vObjectList(void);
    static void vObjectSummary(void);
    static void vObjectSnapshot(ObjectSnapshot* pPoolSnapshot);
    static void vObjectValidate(void);
    static void vMonitorPortrait(s48K& sPortrait);
    static void vPoolAnalyze(void);
    static void vPoolDump(void);
    static i8   iPoolFree(void);
    static void vRegionSummary(void);
    static void vWordList(i8 iEpoch);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
