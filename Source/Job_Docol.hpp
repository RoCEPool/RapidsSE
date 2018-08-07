/*============================================================================*/
/* Job_Docol.hpp -- Docol Job                                                 */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Docol : public Job,
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
  
    Job_Docol(Interface::SEitf* pSEitf);
    ~Job_Docol(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Docol* _pJob_Docol(void* aAddr);
  
  Job* pJob_Docol_pre(Interface::SEitf* pSEitf);
  void vJob_Docol_post(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_Docol_delete(Interface::SEitf* pSEitf, Job* pJob);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
