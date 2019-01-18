/*============================================================================*/
/* Job_DropTable.hpp -- Drop Table Job                                        */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_DropTable : public Job
  {
  public:
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_DropTable(Interface::Session* pSession);
    ~Job_DropTable(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_DropTable* _pJob_DropTable(void* aAddr);
  
  Job* pJob_DropTable_pre(Interface::Session* pSession);
  void vJob_DropTable_post(Interface::Session* pSession, Job* pJob);
  void vJob_DropTable_delete(Interface::Session* pSession, Job* pJob);
  void vJob_DropTable_show(Job* pJob);
  
  JobClass* pJob_DropTable_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
