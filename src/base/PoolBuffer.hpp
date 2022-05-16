/*============================================================================*/
/* PoolBuffer.hpp - header for buffers in buffer pool                         */
/*============================================================================*/
#pragma once

static const u8 C_SIZE_POOL_BUFFER   = C_1MB;

static const u8 C_SIZE_PAGE_FILE     = C_SIZE_POOL_BUFFER * 100;

static const i8 C_SIZE_TINY_OBJECT   = C_1KB;
static const i8 C_SIZE_SMALL_OBJECT  = C_4KB;
static const i8 C_SIZE_MEDIUM_OBJECT = C_64KB;
static const i8 C_SIZE_LARGE_OBJECT  = C_SIZE_POOL_BUFFER - C_SIZE_SMALL_OBJECT;
static const i8 C_SIZE_DATA_OBJECT   = C_SIZE_POOL_BUFFER - C_SIZE_SMALL_OBJECT;

static const u8 C_MASK_TINY_OBJECT   = 0x3FF;
static const u8 C_MASK_SMALL_OBJECT  = 0xFFF;
static const u8 C_MASK_MEDIUM_OBJECT = 0xFFFF;
static const u8 C_MASK_LARGE_OBJECT  = 0xFFFFF;

static const i8 C_LOW_BUFFERS        = 256;

static const u8 C_OBJECT_TO_BUFFER_MASK = BITWISE_NOT((C_SIZE_POOL_BUFFER - 1));

static const i8 C_VIA_MALLOC         = 1;


namespace BASE
{
  class PoolBuffer : public Object
  {
  protected:
    i8 m_iBufferNo;  // this buffer number
    i8 m_nUnits;     // number of buffers
  
    // ..heap buffer..
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    PoolBuffer(eCN eClass);
   ~PoolBuffer(void) noexcept(false);
  
    i8    iBufferNo(void);
    void  vBufferNo(i8 iBufferNo);
    void* aEmbeddedBuffer(void);
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
    
      i8 i_PoolBuffer = pBuffer->iBufferNo();
    
      return i_PoolBuffer;
    }
  
  };
  
  PoolBuffer* _pPoolBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
