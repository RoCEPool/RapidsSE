/*============================================================================*/
/* StagePtnVec.hpp -- Stage Partition Vector                                  */
/*============================================================================*/
#pragma once

namespace Operators
{
  class StagePtnVec : public VectorNL_t<CN_STAGESPECVEC,StageSpec,C_MAX_STAGES>
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    StagePtnVec(void);
   ~StagePtnVec(void) noexcept(false);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
