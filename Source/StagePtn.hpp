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
    OpVec      m_oOpVec;     // set of operators
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    StagePtn(StageSpec* pStageSpec);
   ~StagePtn(void) noexcept(false);
  
    StageSpec* pStageSpec(void);
  
    Footer* aFooter(void);
  };
  
  StagePtn* _pStagePtn(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
