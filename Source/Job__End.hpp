/*============================================================================*/
/* Job__End.hpp -- Job End                                                    */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job__End : public Job
  {
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job__End(void);
    ~Job__End(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job__End* _pJob__End(void* aAddr);
  
  void vJob__End_delete(Interface::SEitf* pSEitf, Job* pJob);
  
  pNEXTJOBFUN* pJob__End_next(JobClass_s** aaJobClass);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
