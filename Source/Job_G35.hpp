/*============================================================================*/
/* Job_G35.hpp -- Grid R35 Test Job                                           */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_G35 : public Job,
                  public JobDicPath,
                  public JobObjectName,
                  public JobResult
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_G35(Interface::SEitf* pSEitf);
    ~Job_G35(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_G35* _pJob_G35(void* aAddr);
  
  Job* pJob_G35_pre(Interface::SEitf* pSEitf);
  void vJob_G35_post(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_G35_delete(Interface::SEitf* pSEitf, Job* pJob);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
