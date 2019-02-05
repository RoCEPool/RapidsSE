/*============================================================================*/
/* Job_WriteRecord.hpp -- Write Record Job                                    */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_WriteRecord : public Job
  {
  public:
    i8 m_iExportJobId;  // job id of export job
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_WriteRecord(Interface::Session* pSession);
    ~Job_WriteRecord(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    static Job_WriteRecord* pNewJobWriteRecord(Interface::Session* pSession,
                                               i8 iExportJobId);
    
  public:
    void vWriteRecords(void);
    void vShow(void);
    
  public:
    Footer* aFooter(void);
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_WriteRecord* _pJob_WriteRecord(void* aAddr);
  
  Job* pJob_WriteRecord_pre(Interface::Session* pSession);
  Job* pJob_WriteRecord_new(Job* pArg);
  void vJob_WriteRecord_delete(Interface::Session* pSession, Job* pJob);
  void vJob_WriteRecord_show(Job* pJob);
  
  JobClass* pJob_WriteRecord_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
