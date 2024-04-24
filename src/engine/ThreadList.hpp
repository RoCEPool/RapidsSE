/*============================================================================*/
/* ThreadList.hpp -- SkipList of Threads                                      */
/*============================================================================*/
#pragma once
#include "Lock.hpp"
#include "SkipListNL.hpp"

namespace ENGINE
{
  class ThreadList : public Object
  {
  private:
    
    Lock m_okLock;
    SkipListNL* m_pSkipList;
    
    // must be last
    Footer m_xFooter;    
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ThreadList(void);
    ~ThreadList(void) noexcept(false);
  
    void vDropLock(void);
    void vGrabLock(void);

    void vIterate(const std::function<void(Thread* pListObj)>& vfVisitorFn);
    void vInsert(Thread* pThread);
    void vRemove(Thread* pThread);
    i8   nItems(void);
    
    static ThreadList* _pThreadList(void* aAddr);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
