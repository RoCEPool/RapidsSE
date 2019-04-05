/*============================================================================*/
/* Job_SizeTable.hpp -- Size Table Job                                        */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_SizeTable : public Job
  {
  public:
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_SizeTable(Interface::Session* pSession);
    ~Job_SizeTable(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_SizeTable* _pJob_SizeTable(void* aAddr);
  
  Job* pJob_SizeTable_pre(Interface::Session* pSession);
  void vJob_SizeTable_delete(Interface::Session* pSession, Job* pJob);
  void vJob_SizeTable_show(Job* pJob);
  
  JobClass* pJob_SizeTable_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
