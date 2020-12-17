/*============================================================================*/
/* ThreadVec.hpp -- Vector of Tasking                                         */
/*============================================================================*/
#pragma once

namespace Tasking
{
  class ThreadVec : public VectorNL_t<CN_THREADVEC,Thread,C_MAX_THREADS>
  {
  private:
    
    Lock m_okLock;
    
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
