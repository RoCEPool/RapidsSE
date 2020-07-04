/*============================================================================*/
/* Pot.hpp -- Working Pot of Buffers                                          */
/*============================================================================*/
#pragma once

namespace DataBuffers
{
  class Pot : public Object
  {
  private:
  
    HashSet  m_oHashSet; // using this as an interim step
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Pot(void);
   ~Pot(void) noexcept(false);
  
    void vAddBuffer(DataBuffer* pDataBuffer);

    DataBuffer* pCompleted(void);
    DataBuffer* pDangling(void);
  
    Footer* aFooter(void);
  };

  Pot* _pPot(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
