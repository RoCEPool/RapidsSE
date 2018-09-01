/*============================================================================*/
/* Job_Space.hpp -- Space Job                                                 */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Space : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Space(Interface::SEitf* pSEitf);
    ~Job_Space(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Space* _pJob_Space(void* aAddr);
  
  Job* pJob_Space_pre(Interface::SEitf* pSEitf);
  void vJob_Space_post(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_Space_delete(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_Space_show(Job* pJob);
  
  JobClass* pJob_Space_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
