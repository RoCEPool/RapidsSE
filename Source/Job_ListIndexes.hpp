/*============================================================================*/
/* Job_ListIndexes.hpp -- List Indexes Job                                    */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_ListIndexes : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_ListIndexes(Interface::SEitf* pSEitf);
    ~Job_ListIndexes(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_ListIndexes* _pJob_ListIndexes(void* aAddr);
  
  Job* pJob_ListIndexes_pre(Interface::SEitf* pSEitf);
  void vJob_ListIndexes_post(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_ListIndexes_delete(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_ListIndexes_show(Job* pJob);
  
  JobClass* pJob_ListIndexes_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
