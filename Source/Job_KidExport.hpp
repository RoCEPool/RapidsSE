/*============================================================================*/
/* Job_KidExport.hpp -- Drag ExportBuffer to export node and write it         */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_KidExport : public Job
  {
  public:
    i8 m_iExportJobId;  // job id of export job
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_KidExport(Interface::Session* pSession);
    ~Job_KidExport(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    static void vSpawn(i8 iExportJobId,
                       ExportBuffer* pExportBuffer);

  public:
    void vWriteBuffer(Job_Export* pJob_Export);
    void vShow(void);
    
  public:
    Footer* aFooter(void);
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_KidExport* _pJob_KidExport(void* aAddr);
  
  Job* pJob_KidExport_pre(Interface::Session* pSession);
  Job* pJob_KidExport_new(Job* pArg);
  void vJob_KidExport_delete(Interface::Session* pSession, Job* pJob);
  void vJob_KidExport_show(Job* pJob);
  
  JobClass* pJob_KidExport_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
