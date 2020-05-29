/*============================================================================*/
/* Job_Prompt.hpp -- Prompt Job                                               */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Prompt : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Prompt(Interface::Session* pSession);
    ~Job_Prompt(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Prompt* _pJob_Prompt(void* aAddr);
  
  Job* pJob_Prompt_new(Job* pArg);
  void vJob_Prompt_delete(Interface::Session* pSession, Job* pJob);
  void vJob_Prompt_show(Job* pJob);
  
  JobClass* pJob_Prompt_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
