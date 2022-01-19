/*============================================================================*/
/* ThreadScratch.hpp -- Thread Scratch area                                   */
/*============================================================================*/
#pragma once

namespace BASE
{
  class ThreadScratch : public Object
  {
  private:
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    ThreadScratch(void);
   ~ThreadScratch(void) noexcept(false);
  

  private:

    Footer* aFooter(void);
  };
  
  ThreadScratch* _pThreadScratch(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
