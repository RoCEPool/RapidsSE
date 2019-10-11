/*============================================================================*/
/* StagePtn.hpp -- Pipe Stage-Partition                                       */
/*============================================================================*/
#pragma once

namespace Operators
{
  class StagePtn : public Object
  {
  public:
    StageSpec* m_pStageSpec; // specification of the stage
    i8         m_i_Ptn;      // which partition
    OpVec      m_oOpVec;     // set of operators
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    StagePtn(StageSpec* pStageSpec, i8 i_Ptn);
   ~StagePtn(void) noexcept(false);
  
    void       vLinkOperators(StageVec* pStageVec);
    StageSpec* pStageSpec(void);
  
    Footer* aFooter(void);
  };
  
  StagePtn* _pStagePtn(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
