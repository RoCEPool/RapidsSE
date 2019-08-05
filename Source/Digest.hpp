/*============================================================================*/
/* Digest.hpp -- Operator Set Digest                                          */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class Digest : public Object
  {
  public:
  
    Job*  m_pJob;
    bool  m_bPulsed;
    Op*   m_pOpToPulse;
  
    OpVec m_oOpVec;  // set of operators
  
  private:
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Digest(Job* pJob);
   ~Digest(void) noexcept(false);
  
    void vParseCommand(TokenVec* pTknVec);
    void vSetupJob(void);

    Footer* aFooter(void);
  };
  
  Digest* _pDigest(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
