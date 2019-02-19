/*============================================================================*/
/* Job_Export.hpp -- Export Table Job                                         */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Export : public Job,
                     public JobFilePath
  {
  public:
    Table* m_pTable;   // table being exported
  
    Lock   m_okLock;   // lock over the following members

    File   m_oCsvFile; // output csv file
  
    Completer m_oWritten; // track writing of all export buffers
  
    Tapper m_oAuxAck; // handle the auxilary ack from Kid Export job
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Export(Interface::Session* pSession);
    ~Job_Export(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bBegin(void);
    void vEnd(void);
    bool bIncWritten(i8 nBuffersExpected);
    bool bPopulateJob(TokenVec* pTokenVec);
    void vWriteExportBuffer(ExportBuffer* pExportBuffer);
  
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Export* _pJob_Export(void* aAddr);
  
  Job* pJob_Export_pre(Interface::Session* pSession);
  void vJob_Export_post(Interface::Session* pSession, Job* pJob);
  Job* pJob_Export_new(Job* pArg);
  void vJob_Export_delete(Interface::Session* pSession, Job* pJob);
  void vJob_Export_show(Job* pJob);
  
  JobClass* pJob_Export_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
