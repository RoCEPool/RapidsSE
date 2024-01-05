/*============================================================================*/
/* PoolHelper.hpp -- buffer pool analyzer                                     */
/*============================================================================*/
#pragma once

namespace STORE
{
  class StreamSet;
}

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
    static void vObjectSummary(RowBuffer* pRowBuffer, StreamDef* pStreamDef,
                               Pds* pPdsWarn);
    static void vObjectSnapshot(ObjectSnapshot* pPoolSnapshot);
    static void vObjectValidate(void);
    static void vPoolAnalyze(void);
    static void vPoolDump(void);
    static i8   iPoolFree(void);
    static void vRegionSummary(void);
    static void vRegionSummary(RowBuffer* pRowBuffer, StreamDef* pStreamDef,
                               Pds* pPdsWarn, StreamDef* pWarnDef);
    static void vValidateEnums(void);
    static void vWordList(i8 iEpoch);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
