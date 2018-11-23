/*============================================================================*/
/* Job_GenData.hpp -- Generate Data Job                                       */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_GenData : public Job,
                      public JobFileName
  {
  public:
    Table*  m_pTable;  // table having data generate

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_GenData(Interface::Session* pSession);
    ~Job_GenData(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
    
    void vGenerate(void);
  
    Table* pTable(void);
    void   vTable(Table* pTable);

  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_GenData* _pJob_GenData(void* aAddr);
  
  Job* pJob_GenData_pre(Interface::Session* pSession);
  void vJob_GenData_post(Interface::Session* pSession, Job* pJob);
  Job* pJob_GenData_new(Job* pArg);
  void vJob_GenData_delete(Interface::Session* pSession, Job* pJob);
  void vJob_GenData_show(Job* pJob);
  
  JobClass* pJob_GenData_class(pFUN** aaFun);
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
