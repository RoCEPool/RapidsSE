/*============================================================================*/
/* Job_SubLoader.hpp -- Table SubLoader Job                                   */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_SubLoader : public Job,
                        public JobDicPath,
                        public JobObjectName,
                        public JobResult
  {
  private:
    Job_LoadTable* m_pJob_LoadTable;
    Utilities::LoaderSub* m_pLoaderSub;
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_SubLoader(Interface::SEitf* pSEitf);
    ~Job_SubLoader(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vJob_LoadTable(Job_LoadTable* pJob_LoadTable);
    void vLoaderSub(Utilities::LoaderSub* pLoaderSub);

    void vShow(void);
    Footer* aFooter(void);

  private:
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_SubLoader* _pJob_SubLoader(void* aAddr);
  
  Job* pJob_SubLoader_pre(Interface::SEitf* pSEitf);
  void vJob_SubLoader_post(Interface::SEitf* pSEitf, Job* pJob);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
