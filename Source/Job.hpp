/*============================================================================*/
/* Job.hpp -- a job in the system                                             */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  class Job : public Object
  {
  private:
    i8     m_iJobId; // job id
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Job* pJob);
    void  operator delete(void*);
    
    Job(void);
    ~Job(void);
    
    i8   iJobId(void);
    void vJobId(i8 iJobId);
    i8   iJobNode(void);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    void vContentDump(void);
    
    Footer* aFooter(void);
  };
  
  Job* _pJob(void* aAddr);
}

bool bJobCompare(void* pObjectInsert, void* pObjectScan, bool& bMatch);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
