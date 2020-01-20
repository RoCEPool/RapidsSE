/*============================================================================*/
/* PoolBuffer.hpp - header for buffers in buffer pool                         */
/*============================================================================*/
#pragma once

namespace Pool
{
  class PoolBuffer : public Object
  {
  protected:
    i8 m_iBufferNo;  // this buffer number
    // ..heap buffer..
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    PoolBuffer(eCN eClass);
    ~PoolBuffer(void) noexcept(false);
  
  
    void* aDataArea(void);
    Stz*  aLastUsableByte(void);
    i8    iPoolBufferNo(void);
    void  vPoolBufferNo(i8 iBufferNo);

    Footer* aFooter(void);
  
    /*----------------------------------------------------------------------------*/
    /* get pool buffer boundary that an object is within                          */
    /*----------------------------------------------------------------------------*/
    inline static PoolBuffer* pGetPoolBuffer(Object* pObject)
    {
      PoolBuffer* pPoolBuffer =
        (PoolBuffer*)(pBitAnd<PoolBuffer*>(pObject,C_OBJECT_TO_BUFFER_MASK));
    
      return pPoolBuffer;
    }
  
    /*----------------------------------------------------------------------------*/
    /* get the buffer no of the pool buffer that the object is within             */
    /*----------------------------------------------------------------------------*/
    inline static bn nPoolBufferNo(Object* pObject)
    {
      PoolBuffer* pBuffer = pGetPoolBuffer(pObject);
    
      bn nBufferNo = pBuffer->iPoolBufferNo();
    
      return nBufferNo;
    }
  
    /*----------------------------------------------------------------------------*/
    /* get the buffer no of the pool buffer that the object is within             */
    /*----------------------------------------------------------------------------*/
    inline static i8 iPoolBufferNo(Object* pObject)
    {
      PoolBuffer* pBuffer = pGetPoolBuffer(pObject);
    
      bn nBufferNo = pBuffer->iPoolBufferNo();
    
      return *nBufferNo;
    }
  
  };
  
  PoolBuffer* _pPoolBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
