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
  
    i8    iBufferNo(void);
    void  vBufferNo(i8 iBufferNo);
    void* aDataArea(void);
    Stz*  aLastUsableByte(void);

    Footer* aFooter(void);
  
    /*----------------------------------------------------------------------------*/
    /* get pool buffer boundary that an object is within                          */
    /*----------------------------------------------------------------------------*/
    inline static PoolBuffer* pWithinPoolBuffer(Object* pObject)
    {
      PoolBuffer* pPoolBuffer =
        (PoolBuffer*)(pBitAnd<PoolBuffer*>(pObject,C_OBJECT_TO_BUFFER_MASK));
    
      return pPoolBuffer;
    }
  
    /*----------------------------------------------------------------------------*/
    /* get the buffer no of the pool buffer that the object is within             */
    /*----------------------------------------------------------------------------*/
    inline static i8 iWithinPoolBuffer(Object* pObject)
    {
      PoolBuffer* pBuffer = pWithinPoolBuffer(pObject);
    
      i8 i_Buffer = pBuffer->iBufferNo();
    
      return i_Buffer;
    }
  
  };
  
  PoolBuffer* _pPoolBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
