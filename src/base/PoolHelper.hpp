/*============================================================================*/
/* PoolHelper.hpp -- buffer pool analyzer                                     */
/*============================================================================*/
#pragma once

namespace BASE
{
  class PoolHelper
  {
  public:
    static void vHeapDump(Heap* pBuffer);
    static void vHeapDump(Heap* pBuffer, i8 iEpoch);
    static void vInspectObjects(i8 iAfter);
    static void vObjectDump(i8 iAfter);
    static void vObjectDump(i8 iAfter, Heap* pHeap);
    static void vObjectSummary(void);
    static void vObjectSnapshot(ObjectSnapshot* pPoolSnapshot);
    static void vObjectValidate(void);
    static void vMonitorPortrait(s48K& sPortrait);
    static void vPoolAnalyze(void);
    static void vPoolDump(void);
    static i8   iPoolFree(void);
    static void vRegionSummary(void);
    static void vValidateEnums(void);
    static void vWordList(i8 iEpoch);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
