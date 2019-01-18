/*============================================================================*/
/* Job_CopyBuffer.hpp -- Copy Buffer Job                                      */
/*============================================================================*/
#pragma once

namespace JobModel
{
  class Job_SubInserter;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_CopyBuffer : public Job,
                         public JobRow
  {
  public:
    Job_SubInserter* m_pJob_SubInserter;
    RecordBuffer*    m_pRecordBuffer;
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_CopyBuffer(Interface::Session* pSession);
    ~Job_CopyBuffer(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vCopy(void);
    void vShow(void);
    
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_CopyBuffer* _pJob_CopyBuffer(void* aAddr);
  
  Job* pJob_CopyBuffer_pre(Interface::Session* pSession);
  void vJob_CopyBuffer_post(Interface::Session* pSession, Job* pJob);
  Job* pJob_CopyBuffer_new(Job* pArg);
  void vJob_CopyBuffer_delete(Interface::Session* pSession, Job* pJob);
  void vJob_CopyBuffer_show(Job* pJob);
  
  JobClass* pJob_CopyBuffer_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
