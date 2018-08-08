/*============================================================================*/
/* Job_DropSchema.hpp -- Drop Schema Job                                      */
/*============================================================================*/
#pragma once

namespace JobsModel
{

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_DropSchema : public Job
  {
  public:
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_DropSchema(Interface::SEitf* pSEitf);
    ~Job_DropSchema(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  Job_DropSchema* _pJob_DropSchema(void* aAddr);
  
  Job* pJob_DropSchema_pre(Interface::SEitf* pSEitf);
  void vJob_DropSchema_post(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_DropSchema_delete(Interface::SEitf* pSEitf, Job* pJob);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
