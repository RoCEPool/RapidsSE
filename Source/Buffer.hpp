/*============================================================================*/
/* Buffer.hpp - header for buffers in buffer pool                             */
/*============================================================================*/
#pragma once

namespace Pool
{
  class Buffer : public Object
  {
  protected:
    bn   m_nBufferNo;        // this buffer number
    bn   m_nOverflowBuffer;  // logical overflow
    Stz* m_aLastByte;
    // ..data..
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Buffer(eCN eClass);
    ~Buffer(void);
    
    bn    nBufferNo(void);
    void  vBufferNo(bn nBufferNo);
    Stz*  aLastUsableByte(void);
    bn    nOverflowBuffer(void);
    void  vOverflowBuffer(bn nOverflowBuffer);
    
    Footer* aFooter(void);
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
