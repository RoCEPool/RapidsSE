/*============================================================================*/
/* CrvBuffer.hpp -- Compact Row Version Buffer                                */
/*============================================================================*/
#pragma once

namespace ReadConsistency
{
  class CrvBuffer : public DataBuffer
  {
  public:
    SeeMe m_xSeeMe;
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    CrvBuffer(void);
   ~CrvBuffer(void) noexcept(false);

  private:
    Footer* aFooter(void);
  };
  
  CrvBuffer* _pCrvBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
