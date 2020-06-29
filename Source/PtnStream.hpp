/*============================================================================*/
/* PtnStream.hpp -- Partition Stream                                          */
/*============================================================================*/
#pragma once

namespace Partitioning
{
  class PtnStream : public Object
  {
  private:
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    PtnStream(void);
   ~PtnStream(void) noexcept(false);
    
  private:
    Footer* aFooter(void);
  };
  
  PtnStream* _pPtnStream(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
