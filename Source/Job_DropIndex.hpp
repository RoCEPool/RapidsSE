/*============================================================================*/
/* Job_DropIndex.hpp -- Drop Index Job                                        */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_DropIndex : public Job
  {
  public:
    
    s32    m_sTableName;   // table name
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_DropIndex(Interface::SEitf* pSEitf);
    ~Job_DropIndex(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_DropIndex* _pJob_DropIndex(void* aAddr);
  
  Job* pJob_DropIndex_pre(Interface::SEitf* pSEitf);
  void vJob_DropIndex_post(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_DropIndex_delete(Interface::SEitf* pSEitf, Job* pJob);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
