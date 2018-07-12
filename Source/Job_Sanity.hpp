/*============================================================================*/
/* Job_Sanity.hpp -- Sanity Job                                               */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Sanity : public Job,
                    public JobDicPath,
                    public JobObjectName,
                    public JobResult
  {
  public:
    
    i8 m_iRegionSize; // region size of this node
    i8 m_iPartitions; // no of partition in this
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Sanity(Interface::SEitf* pSEitf);
    ~Job_Sanity(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Sanity* _pJob_Sanity(void* aAddr);
  
  Job* pJob_Sanity_pre(TokenVec* pTokenVec);
  void vJob_Sanity_post(void);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
