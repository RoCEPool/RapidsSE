/*============================================================================*/
/* Job_TableSizes.hpp -- Table sizes Job                                      */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_TableSizes : public Job
  {
  public:
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_TableSizes(Interface::SEitf* pSEitf);
    ~Job_TableSizes(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_TableSizes* _pJob_TableSizes(void* aAddr);
  
  Job* pJob_TableSizes_pre(Interface::SEitf* pSEitf);
  void vJob_TableSizes_post(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_TableSizes_delete(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_TableSizes_show(Job* pJob);
  
  Job* pJob_TableSizes_next(void** aaArg);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
