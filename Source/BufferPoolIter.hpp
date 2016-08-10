/*============================================================================*/
/*  BufferPoolIter.hpp - buffer pool iterator                                 */
/*============================================================================*/

#pragma once

#include "MemoryManagement.h"

using namespace Common;

namespace MemoryManagement
{
  class BufferFree;
  class BufferPool;
  
  class BufferPoolIter : public mn
  {
  private:
    cn                m_nClassNo;         
    BufferFree*       m_pCurrentBuffer;
    const BufferPool* m_pBufferPoolator;

  public:
    BufferPoolIter(const BufferPool* pBufferPoolator, BufferFree* pCurrentBuffer);
    ~BufferPoolIter(void);
    
    bool operator !=(const BufferPoolIter& other) const;
    BufferFree*  operator *(void) const;
    const BufferPoolIter& operator ++(void);
    
    void vAssertClass(eCN eClassNo) const;
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/