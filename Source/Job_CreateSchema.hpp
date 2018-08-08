/*============================================================================*/
/* Job_CreateSchema.hpp -- Create Schema Job                                  */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_CreateSchema : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_CreateSchema(Interface::SEitf* pSEitf);
    ~Job_CreateSchema(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_CreateSchema* _pJob_CreateSchema(void* aAddr);
  
  Job* pJob_CreateSchema_pre(Interface::SEitf* pSEitf);
  void vJob_CreateSchema_post(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_CreateSchema_delete(Interface::SEitf* pSEitf, Job* pJob);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
