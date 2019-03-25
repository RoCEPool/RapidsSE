/*============================================================================*/
/* Job_DumpTable.hpp -- Dump Table Job                                        */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_DumpTable : public Job
  {
  public:
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_DumpTable(Interface::Session* pSession);
    ~Job_DumpTable(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_DumpTable* _pJob_DumpTable(void* aAddr);
  
  Job* pJob_DumpTable_pre(Interface::Session* pSession);
  void vJob_DumpTable_post(Interface::Session* pSession, Job* pJob);
  void vJob_DumpTable_delete(Interface::Session* pSession, Job* pJob);
  void vJob_DumpTable_show(Job* pJob);
  
  JobClass* pJob_DumpTable_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
