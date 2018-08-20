/*============================================================================*/
/* Job_AnalyzeIndex.hpp -- Analyze Index Job                                  */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_AnalyzeIndex : public Job
  {
  public:
   
    bool m_bIsDetailed; // detailed analysis
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_AnalyzeIndex(Interface::SEitf* pSEitf);
    ~Job_AnalyzeIndex(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_AnalyzeIndex* _pJob_AnalyzeIndex(void* aAddr);
  
  Job* pJob_AnalyzeIndex_pre(Interface::SEitf* pSEitf);
  void vJob_AnalyzeIndex_post(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_AnalyzeIndex_delete(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_AnalyzeIndex_show(Job* pJob);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
