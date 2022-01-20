/*============================================================================*/
/* ThreadScratch.hpp -- Thread Scratch area                                   */
/*============================================================================*/
#pragma once

namespace BASE
{
  class ThreadScratch : public Object
  {
  private:
  
    Lock m_okLock;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    ThreadScratch(void);
   ~ThreadScratch(void) noexcept(false);
  
    void vDropLock(void);
    void vGrabLock(void);

  private:

    Footer* aFooter(void);
  };
  
  ThreadScratch* _pThreadScratch(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
