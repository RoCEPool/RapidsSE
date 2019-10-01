/*============================================================================*/
/* Pipeline.hpp -- Operator Set Pipeline                                      */
/*============================================================================*/
#pragma once

namespace Operators
{
  class Pipeline : public Object
  {
  public:
  
    Session* m_pSession;
    Job*     m_pJob;
    bool     m_bPulsed;
    Op*      m_pOpToPulse;
  
    bool     m_bCancelling;
  
    DynamicSpec* m_pDynamicSpec; // the dynamic job specification
    StageVec     m_oStageVec;    // vector of stages
  
  private:
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Pipeline(Job* pJob);
   ~Pipeline(void) noexcept(false);
  
    void vPopulateOpVec(DynamicSpec* pDynamicSpec);
    void vSetupJob(void);

    Footer* aFooter(void);
  };
  
  Pipeline* _pDigest(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
