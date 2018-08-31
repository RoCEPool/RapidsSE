/*============================================================================*/
/* Job_SizeIndex.hpp -- Size Index Job                                        */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_SizeIndex : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_SizeIndex(Interface::SEitf* pSEitf);
    ~Job_SizeIndex(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_SizeIndex* _pJob_SizeIndex(void* aAddr);
  
  Job* pJob_SizeIndex_pre(Interface::SEitf* pSEitf);
  void vJob_SizeIndex_post(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_SizeIndex_delete(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_SizeIndex_show(Job* pJob);
  
  pNEXTJOBFUN* pJob_SizeIndex_next(JobClass_s** aaJobClass);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
