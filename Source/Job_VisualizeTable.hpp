/*============================================================================*/
/* Job_VisualizeTable.hpp -- Visualize Index Job                              */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_VisualizeTable : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_VisualizeTable(Interface::Session* pSession);
    ~Job_VisualizeTable(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_VisualizeTable* _pJob_VisualizeTable(void* aAddr);
  
  Job* pJob_VisualizeTable_pre(Interface::Session* pSession);
  void vJob_VisualizeTable_delete(Interface::Session* pSession, Job* pJob);
  void vJob_VisualizeTable_show(Job* pJob);
  
  JobClass* pJob_VisualizeTable_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
