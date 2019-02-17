/*============================================================================*/
/* Completer.hpp -- multi-partition buffer completer                          */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class Completer : public Object
  {
  private:
    i8 m_nPartitionsComplete;
    i8 m_nBuffersExpected;
    i8 m_nBuffersProcessed;

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t, Completer* pCompleter);
    void  operator delete(void* aAddress);
    
    Completer(void);
    ~Completer(void) noexcept(false);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
