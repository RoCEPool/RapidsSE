/*============================================================================*/
/* Job_Boot.hpp -- System Boot Job                                            */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Boot : public Job
  {
  private:

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Boot(Interface::SEitf* pSEitf);
    ~Job_Boot(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:

    void vShow(void);
    Footer* aFooter(void);

  private:
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Boot* _pJob_Boot(void* aAddr);
  
  Job* pJob_Boot_new(Job* pArg);
  void vJob_Boot_delete(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_Boot_show(Job* pJob);
  
  JobClass_s* pJob_Boot_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
