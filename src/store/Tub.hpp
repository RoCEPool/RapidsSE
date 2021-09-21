/*============================================================================*/
/* Tub.hpp -- Chain of Buffers with dangling support                            */
/*============================================================================*/
#pragma once

namespace STORE
{
  class Tub : public Object
  {
  public:
    DataBuffer*  m_pBufferChain;
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Tub(void);
   ~Tub(void) noexcept(false);
    
  public:
  
    void vAddBuffer(DataBuffer* pDataBuffer);

    DataBuffer* pCompletedBuffer(void);
    DataBuffer* pDanglingBuffer(void);
    DataBuffer* pWorkingBuffer(void);

    Footer* aFooter(void);
  };
  
  Tub* _pTub(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
