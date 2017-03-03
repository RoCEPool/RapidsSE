/*============================================================================*/
/* BPA.hpp -- buffer pool analyzer                                            */
/*============================================================================*/
#pragma once

namespace Pool
{
  class Heap;
  
  class BPA : public Object
  {
  private:
    
    // must be last
    Footer m_xFooter;

  public:
    
    BPA(void);
    ~BPA(void);
    
    void vBufferDump(Buffer* pBuffer);
    void vHeapDump(Heap* pBuffer);
    void vHeapDump(Heap* pBuffer, i8 iEpoch);
    void vObjectList(void);
    void vObjectSummary(void);
    void vObjectSnapshot(i8* pCountObjects);
    void vPoolAnalyze(void);
    void vPoolDump(void);
    void vRegionSummary(void);
    void vResourceSummary(void);
    void vWordList(i8 iEpoch);
    
    Footer* pFooter(void);
  };
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
