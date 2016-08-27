/*============================================================================*/
/* Buffer.hpp - header for buffers in buffer pool                             */
/*============================================================================*/

#pragma once

#include "Common.h"

namespace Pool
{
  class Buffer : public Object
  {
  private:
    bn m_nBufferNo;        // this buffer number
    bn m_nOverflowBuffer;  // logical overflow
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Buffer(eCN eClass);
    ~Buffer(void);
    
    //void  vAssertClass(eCN eClassNo) const;
    char* pBufferLimit(void) const;
    bn    nBufferNo(void);
    void  vBufferNo(bn nBufferNo);
    //void  vClass(eCN eClassNo);
    //eCN   eClassNo(void);
    char* pLastByte(void);
    bn    nOverflowBuffer(void);
    void  vOverflowBuffer(bn nOverflowBuffer);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
