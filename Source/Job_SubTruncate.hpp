/*============================================================================*/
/* pJob_SubTruncate.hpp -- Sub Truncate Job                                   */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_SubTruncate : public Job
  {
  public:
   
    i8 m_iTruncateJobId; //job id of the truncate job that spawned this sub truncate
    i8 m_iSlot;          // slot this sub truncate truncates
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_SubTruncate(Interface::SEitf* pSEitf);
    ~Job_SubTruncate(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_SubTruncate* _pJob_SubTruncate(void* aAddr);
  
  Job* pJob_SubTruncate_pre(Interface::SEitf* pSEitf);
  void vJob_SubTruncate_post(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_SubTruncate_delete(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_SubTruncate_show(Job* pJob);
  
  pNEXTJOBFUN* pJob_SubTruncate_next(void** aaArg);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
