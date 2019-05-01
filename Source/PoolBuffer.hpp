/*============================================================================*/
/* PoolBuffer.hpp - header for buffers in buffer pool                         */
/*============================================================================*/
#pragma once

namespace Pool
{
  class PoolBuffer : public Object
  {
  protected:
    bn   m_nBufferNo;        // this buffer number
    bn   m_nOverflowBuffer;  // logical overflow
    Stz* m_aLastByte;
    // ..data..
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    PoolBuffer(eCN eClass);
    ~PoolBuffer(void) noexcept(false);
  
  
    void* aDataArea(void);
    Stz*  aLastUsableByte(void);
    bn    nOverflowBuffer(void);
    void  vOverflowBuffer(bn nOverflowBuffer);
    bn    nPoolBufferNo(void);
    void  vPoolBufferNo(bn nBufferNo);

    Footer* aFooter(void);
  
    /*----------------------------------------------------------------------------*/
    /* get buffer that an object is within                                        */
    /*----------------------------------------------------------------------------*/
    inline static PoolBuffer* pGetPoolBuffer(Object* pObject)
    {
      PoolBuffer* pPoolBuffer =
        (PoolBuffer*)(pBitAnd<PoolBuffer*>(pObject,C_OBJECT_TO_BUFFER_MASK));
    
      return pPoolBuffer;
    }
  
  };
  
  PoolBuffer* _pPoolBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
