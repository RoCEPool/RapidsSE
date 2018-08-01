/*============================================================================*/
/* Job_LoadTable.hpp -- Load Table Job                                        */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_LoadTable : public Job,
                        public JobDicPath,
                        public JobObjectName,
                        public JobResult,
                        public JobFileName
  {
  public:
    Utilities::Loader* m_pLoader; // table loader
    Formatters m_xFormatters; // record formatters
    
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
    bool bPopulateJob(TokenVec* pTokenVec);
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
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
