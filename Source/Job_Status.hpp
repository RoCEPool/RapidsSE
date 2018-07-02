/*============================================================================*/
/* Job_Status.hpp -- Status Job                                               */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Status : public Job,
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
  
    Job_Status(Interface::SEitf* pSEitf);
    ~Job_Status(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Status* _pJob_Status(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
