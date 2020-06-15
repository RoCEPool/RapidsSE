/*============================================================================*/
/* MidEnd_FromVec.hpp -- vector of from stream def's                          */
/*============================================================================*/
#pragma once

namespace LogicalPlans
{
  class MidEnd_FromVec : public VectorNL_t<CN_MIDEND_FROMVEC,StreamDef,C_MAX_FROMVEC>
  {
  private:
  
    // must be last
    Footer m_xFooter;    
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    MidEnd_FromVec(void);
   ~MidEnd_FromVec(void) noexcept(false);
  
    Footer* aFooter(void);
  };

  MidEnd_FromVec* _pMidEnd_FromVec(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
