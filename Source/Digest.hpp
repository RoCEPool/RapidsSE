/*============================================================================*/
/* Digest.hpp -- Operator Set Digest                                          */
/*============================================================================*/
#pragma once

namespace Operators
{
  class Digest : public Object
  {
  public:
  
    Session* m_pSession;
    Job*     m_pJob;
    bool     m_bPulsed;
    Op*      m_pOpToPulse;
  
    bool     m_bCancelling;
  
    OpVec    m_oOpVec;    // set of operators
  
    DynamicSpec* m_pDynamicSpec; // the dynamic job specification
    StageVec     m_oStageVec;    // vector of stages
  
  private:
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Digest(Job* pJob);
   ~Digest(void) noexcept(false);
  
    void vPopulateOpVec(DynamicSpec* pDynamicSpec);
    void vSetupJob(void);

    Footer* aFooter(void);
  };
  
  Digest* _pDigest(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
