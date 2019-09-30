/*============================================================================*/
/* Stage.hpp -- Pipe Stage                                                    */
/*============================================================================*/
#pragma once

namespace Operators
{
  class Stage : public Object
  {
  public:
    StageSpec* m_pStageSpec; // specification of the stage
    OpVec      m_oOpVec;     // set of operators

    StagePtnVec m_oStagePtnVec;   // set of stage-partitions

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Stage(StageSpec* pStageSpec);
   ~Stage(void) noexcept(false);
  
    StageSpec* pStageSpec(void);
  
    Footer* aFooter(void);
  };
  
  Stage* _pStage(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
