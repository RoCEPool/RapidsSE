/*============================================================================*/
/* Job_ListPartitions.hpp -- List Partitions Job                              */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_ListPartitions : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_ListPartitions(Interface::SEitf* pSEitf);
    ~Job_ListPartitions(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_ListPartitions* _pJob_ListPartitions(void* aAddr);
  
  Job* pJob_ListPartitions_pre(Interface::SEitf* pSEitf);
  void vJob_ListPartitions_post(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_ListPartitions_delete(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_ListPartitions_show(Job* pJob);
  
  JobClass_s* pJob_ListPartitions_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
