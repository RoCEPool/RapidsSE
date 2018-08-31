/*============================================================================*/
/* Job_Answer.hpp -- Socket Answer Job                                        */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Answer : public Job
  {
  private:

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Answer(Interface::SEitf* pSEitf);
    ~Job_Answer(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:

    void vShow(void);
    Footer* aFooter(void);

  private:
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Answer* _pJob_Answer(void* aAddr);
  
  Job* pJob_Answer_new(Job* pArg);
  void vJob_Answer_delete(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_Answer_show(Job* pJob);
  
  pNEXTJOBFUN* pJob_Answer_next(void** aaArg);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
