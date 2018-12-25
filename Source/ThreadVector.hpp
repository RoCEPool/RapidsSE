/*============================================================================*/
/* ThreadVector.hpp -- Vector of Threads                                      */
/*============================================================================*/
#pragma once

namespace Threads
{
  class ThreadVector : public Vector<CN_THREADVECTOR,Thread,CN_THREAD,C_THREADS,u8>
  {
  private:
    
    Lock m_oLock;
    
    // must be last
    Footer m_xFooter;    
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    ThreadVector(void);
    ~ThreadVector(void) noexcept(false);
  
    void vDropLock(void);
    void vGrabLock(void);
    
    static ThreadVector* _pThreadVector(void* aAddr);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
