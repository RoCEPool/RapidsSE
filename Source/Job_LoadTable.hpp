/*============================================================================*/
/* Job_LoadTable.hpp -- Load Table Job                                        */
/*============================================================================*/
#pragma once

namespace JobModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_LoadTable : public Job,
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
  
    Job_LoadTable(Interface::Session* pSession);
    ~Job_LoadTable(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bBegin(void);
    bool bChunk(void);
    void vDumpMap(void);
    void vEnd(void);
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
  Job_LoadTable* _pJob_LoadTable(void* aAddr);
  
  Job* pJob_LoadTable_pre(Interface::Session* pSession);
  void vJob_LoadTable_post(Interface::Session* pSession, Job* pJob);
  Job* pJob_LoadTable_new(Job* pArg);
  void vJob_LoadTable_delete(Interface::Session* pSession, Job* pJob);
  void vJob_LoadTable_show(Job* pJob);
  
  JobClass* pJob_LoadTable_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
