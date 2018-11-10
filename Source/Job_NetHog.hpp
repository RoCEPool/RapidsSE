/*============================================================================*/
/* Job_NetHog.hpp -- Network Hog Job                                          */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_NetHog : public Job,
                        public JobFileName
  {
  public:
    Table*     m_pTable;       // table being load
    Formatters m_xFormatters;  // record formatters
  
    LoadMap*   m_pLoadMap;     // load map
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_NetHog(Interface::Session* pSession);
    ~Job_NetHog(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vDumpMap(void);
    bool bFindChunk(void);
    bool bPopulateJob(TokenVec* pTokenVec);
  
    Table* pTable(void);
    void   vTable(Table* pTable);

    void vShow(void);
    
  private:
    bool bExtractDateFormat(Stz* dateFormat);
    
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_NetHog* _pJob_NetHog(void* aAddr);
  
  Job* pJob_NetHog_pre(Interface::Session* pSession);
  void vJob_NetHog_post(Interface::Session* pSession, Job* pJob);
  Job* pJob_NetHog_new(Job* pArg);
  void vJob_NetHog_delete(Interface::Session* pSession, Job* pJob);
  void vJob_NetHog_show(Job* pJob);
  
  JobClass* pJob_NetHog_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
