/*============================================================================*/
/*Jobs.hpp -- Collection of the Systems' Jobs                                 */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  class Jobs : public Object
  {
  private:
  
    Lock      m_xLock;
    SkipList* m_pJobsSkipList;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Jobs* pJobs);
    void  operator delete(void*);
    
    Jobs(cmpFun fCompare);
    ~Jobs(void);
    
    void   vAddJob(Job* pJob);
    void   vDropLock(void);
    void   vGrabLock(void);
    bool   bGrabLock(void);
    bool   bIsLocked(void);
    Job*   pPopJob(void);
    void   vRemoveJob(Job* pJob);
    i8     nJobs(void);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    void vContentDump(void);
    
    Footer* aFooter(void);
  };
  
  Jobs* _pJobs(void* aAddr);
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
