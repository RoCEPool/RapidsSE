/*============================================================================*/
/*  BufferPoolAnalyzer.hpp - buffer pool analyzer                             */
/*============================================================================*/
#pragma once

#include "Common.h"

using namespace Common;

namespace Pool
{
  class Heap;
  
  class BufferPoolAnalyzer : public mn
  {
  private:
    cn m_nClassNo;
    
  public:
    
    BufferPoolAnalyzer(void);
    ~BufferPoolAnalyzer(void);
    void vDump(void);
    void vDump(Buffer* pBuffer);
    void vDumpHeap(Heap* pBuffer);
    void vAnalyze(void);
    void vAssertClass(eCN eClassNo) const;
  };
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/