/*============================================================================*/
/* ThreadVec.hpp -- Vector of Threads                                         */
/*============================================================================*/
#pragma once

namespace Threads
{
  class ThreadVec : public VectorNL_t<CN_THREADVEC,Thread,CN_THREAD,C_THREADS,u8>
  {
  private:
    
    Lock m_oLock;
    
    // must be last
    Footer m_xFooter;    
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    ThreadVec(void);
    ~ThreadVec(void) noexcept(false);
  
    void vDropLock(void);
    void vGrabLock(void);
    
    static ThreadVec* _pThreadVec(void* aAddr);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
