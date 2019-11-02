/*============================================================================*/
/* pJob_SubExporter.hpp -- Sub Exporter Job                                   */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_SubExporter : public Job
  {
  public:
    Job_Export*  m_pJobExport;     // export job that spawned this sub export
  
    i8           m_nCrwBuffers;
    i8           m_iCrwBuffer;
  
    CrwBuffer*   m_pCrwBuffer;
  
    i8 m_nExportBuffers;
    ExportBuffer* m_pExportBuffer;
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_SubExporter(JobClass* aJobClass, ReqTuple* aReqTuple);
    ~Job_SubExporter(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
    bool bBegin(void);
    bool bChunk(void);
    void vEnd(void);

    void vShow(void);
  
  private:
    void vSpawnKidExport(bool bLast);
    void vSprintRows(void);
  
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_SubExporter* _pJob_SubExporter(void* aAddr);
  
  Job* pJob_SubExporter_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession);
  void vJob_SubExporter_delete(Job* pJob);
  void vJob_SubExporter_show(Job* pJob);
  
  JobClass* pJob_SubExporter_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
