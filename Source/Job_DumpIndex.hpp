/*============================================================================*/
/* Job_DumpIndex.hpp -- Dump Index Job                                        */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_DumpIndex : public Job
  {
  public:
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_DumpIndex(Interface::SEitf* pSEitf);
    ~Job_DumpIndex(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_DumpIndex* _pJob_DumpIndex(void* aAddr);
  
  Job* pJob_DumpIndex_pre(Interface::SEitf* pSEitf);
  void vJob_DumpIndex_post(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_DumpIndex_delete(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_DumpIndex_show(Job* pJob);
  
  JobClass_s* pJob_DumpIndex_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
