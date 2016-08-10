/*============================================================================*/
/*  BufferPoolAnalyzer.hpp - buffer pool analyzer                             */
/*============================================================================*/
#pragma once

#include "Common.h"

using namespace Common;

namespace MemoryManagement
{
  class BufferPoolAnalyzer : public mn
  {
  private:
    cn m_nClassNo;
    
  public:
    
    BufferPoolAnalyzer(void);
    ~BufferPoolAnalyzer(void);
    
    void vAnalyze(void);
    void vAssertClass(eCN eClassNo) const;
  };
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/