/*============================================================================*/
/* Job_Start.hpp -- System Start Job                                          */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Start : public Job
  {
  private:

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Start(Interface::SEitf* pSEitf);
    ~Job_Start(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:

    void vShow(void);
    Footer* aFooter(void);

  private:
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Start* _pJob_Start(void* aAddr);
  
  Job* pJob_Start_new(Job* pArg);
  void vJob_Start_delete(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_Start_show(Job* pJob);
  
  Job* pJob_Start_next(void** aaArg);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
