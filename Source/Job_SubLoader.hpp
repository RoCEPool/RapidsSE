/*============================================================================*/
/* Job_SubLoader.hpp -- Table SubLoader Job                                   */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_SubLoader : public Job
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
    Job_LoadTable* pJob_LoadTable(void);
    void vJob_LoadTable(Job_LoadTable* pJob_LoadTable);
    void vLoaderSub(Utilities::LoaderSub* pLoaderSub);

    void vShow(void);
    Footer* aFooter(void);

  private:
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_SubLoader* _pJob_SubLoader(void* aAddr);
  
  Job* vJob_SubLoader_pre(Interface::SEitf* pSEitf);
  Job* pJob_SubLoader_new(Job* pArg);
  void vJob_SubLoader_delete(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_SubLoader_show(Job* pJob);
  
  pNEXTJOBFUN* pJob_SuubLoader_next(void** aaArg);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
