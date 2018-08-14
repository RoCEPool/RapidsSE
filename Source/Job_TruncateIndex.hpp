/*============================================================================*/
/* Job_TruncateIndex.hpp -- Truncate Index Job                                */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_TruncateIndex : public Job
  {
  public:
   
    s32 m_sTableName;   // table name
    bool m_bIsDetailed; // detailed analysis
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_TruncateIndex(Interface::SEitf* pSEitf);
    ~Job_TruncateIndex(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_TruncateIndex* _pJob_TruncateIndex(void* aAddr);
  
  Job* pJob_TruncateIndex_pre(Interface::SEitf* pSEitf);
  void vJob_TruncateIndex_post(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_TruncateIndex_delete(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_TruncateIndex_show(Job* pJob);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
