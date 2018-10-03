/*============================================================================*/
/* Job_LoadTable.hpp -- Load Table Job                                        */
/*============================================================================*/
#pragma once

namespace Utilities
{
  class LoadChunk;
  class LoadMap;
}

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_LoadTable : public Job,
                        public JobFileName
  {
  public:
    Utilities::LoadTable* m_pLoadTable; // table loader
    Formatters m_xFormatters;           // record formatters
  
    Table* m_pTable;                    // table being load
    Utilities:: LoadMap*  m_pLoadMap;   // load map

    Atomic m_ziValidRows;               // valid rows loaded
    Atomic m_ziInvalidRows;             // invalid rows not loaded
    
    Tasks::Task* m_pTask; // task running this job
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_LoadTable(Interface::SEitf* pSEitf);
    ~Job_LoadTable(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vDumpMap(void);
    bool bFindChunk(void);
    bool bPopulateJob(TokenVec* pTokenVec);
  
    Table*   pTable(void);
    void     vTable(Table* pTable);

    Tasks::Task* pTask(void);
    void vTask(Tasks::Task* pTask);

    void vShow(void);
    
  private:
    bool bExtractDateFormat(Stz* dateFormat);
    
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_LoadTable* _pJob_LoadTable(void* aAddr);
  
  Job* pJob_LoadTable_pre(Interface::SEitf* pSEitf);
  void vJob_LoadTable_post(Interface::SEitf* pSEitf, Job* pJob);
  Job* pJob_LoadTable_new(Job* pArg);
  void vJob_LoadTable_delete(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_LoadTable_show(Job* pJob);
  
  JobClass* pJob_LoadTable_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
