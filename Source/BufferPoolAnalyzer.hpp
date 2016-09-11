/*============================================================================*/
/*  BufferPoolAnalyzer.hpp - buffer pool analyzer                             */
/*============================================================================*/
#pragma once

#include "Common.h"

using namespace Common;

namespace Pool
{
  class Heap;
  
  class BufferPoolAnalyzer : public Header
  {
  private:
    
    // must be last
    Footer m_xFooter;

  public:
    
    BufferPoolAnalyzer(void);
    ~BufferPoolAnalyzer(void);
    void vDump(void);
    void vDump(Buffer* pBuffer);
    void vDumpHeap(Heap* pBuffer);
    void vAnalyze(void);
    
    Footer* pFooter(void);
  };
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/