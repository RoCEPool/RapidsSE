/*============================================================================*/
/*  BPA.hpp - buffer pool analyzer                                            */
/*============================================================================*/
#pragma once

#include "Common.h"

using namespace Common;

namespace Pool
{
  class Heap;
  
  class BPA : public Header
  {
  private:
    
    // must be last
    Footer m_xFooter;

  public:
    
    BPA(void);
    ~BPA(void);
    
    void vBufferDump(Buffer* pBuffer);
    void vHeapDump(Heap* pBuffer);
    void vObjectList(void);
    void vObjectSummary(void);
    void vPoolAnalyze(void);
    void vPoolDump(void);
    void vRegionSummary(void);
    void vResourceSummary(void);
    
    Footer* pFooter(void);
  };
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
