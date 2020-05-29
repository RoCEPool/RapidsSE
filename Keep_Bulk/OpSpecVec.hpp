/*============================================================================*/
/* OpSpecVec.hpp -- Op Specification Vector                                   */
/*============================================================================*/
#pragma once

namespace PipelineSpecs
{
  class OpSpecVec : public VectorNL_t<CN_OPSPECVEC,OpSpec,C_MAX_OPS>
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    OpSpecVec(void);
   ~OpSpecVec(void) noexcept(false);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
