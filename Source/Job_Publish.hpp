/*============================================================================*/
/* Job_Publish.hpp -- Publish Job                                             */
/*============================================================================*/
#pragma once

namespace JobModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Publish : public Job,
                      public JobFileName
  {
  public:
    Table*  m_pTable;  // table being purged
  
    Lock    m_oLock;   // lock on output pipe (subpublish contention)
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Publish(Interface::Session* pSession);
    ~Job_Publish(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
    void   vPublish(void);
    Table* pTable(void);
    void   vTable(Table* pTable);

  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Publish* _pJob_Publish(void* aAddr);
  
  Job* pJob_Publish_pre(Interface::Session* pSession);
  void vJob_Publish_post(Interface::Session* pSession, Job* pJob);
  Job* pJob_Publish_new(Job* pArg);
  void vJob_Publish_delete(Interface::Session* pSession, Job* pJob);
  void vJob_Publish_show(Job* pJob);
  
  JobClass* pJob_Publish_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
