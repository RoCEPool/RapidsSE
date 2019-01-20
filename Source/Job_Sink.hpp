/*============================================================================*/
/* Job_Sink.hpp -- Sink Job                                                   */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Sink : public Job,
                   public JobFilePath
  {
  public:
    IoChunk*  m_pIoChunk;
    ByteRate* m_pByteRate;
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Sink(Interface::Session* pSession);
    ~Job_Sink(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
    bool bBegin(void);
    bool bChunk(void);
    void vEnd(void);

  public:
    Footer* aFooter(void);
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Sink* _pJob_Sink(void* aAddr);
  
  Job* pJob_Sink_pre(Interface::Session* pSession);
  void vJob_Sink_post(Interface::Session* pSession, Job* pJob);
  Job* pJob_Sink_new(Job* pArg);
  void vJob_Sink_delete(Interface::Session* pSession, Job* pJob);
  void vJob_Sink_show(Job* pJob);
  
  JobClass* pJob_Sink_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
