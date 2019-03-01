/*============================================================================*/
/* pJob_Walk.hpp -- walk index                                                */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Walk : public Job
  {
  public:
    Table* m_pTable;   // table being walked
    Index* m_pIndex;   // index being walked
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Walk(Interface::Session* pSession);
    ~Job_Walk(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bBegin(void);
    void vEnd(void);
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Walk* _pJob_Walk(void* aAddr);
  
  Job* pJob_Walk_pre(Interface::Session* pSession);
  void vJob_Walk_post(Interface::Session* pSession, Job* pJob);
  void vJob_Walk_delete(Interface::Session* pSession, Job* pJob);
  void vJob_Walk_show(Job* pJob);
  
  JobClass* pJob_Walk_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
