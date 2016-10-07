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