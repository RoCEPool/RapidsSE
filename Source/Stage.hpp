/*============================================================================*/
/* Stage.hpp -- Pipe Stage                                                    */
/*============================================================================*/
#pragma once

namespace Operators
{
  class Stage : public Object
  {
  private:
    StageSpec* m_pStageSpec; // specification of the stage
    OpVec      m_oOpVec;     // set of operators
  
    // must be last
    Footer m_xFooter;

  public:
    Stage(StageSpec* pStageSpec);
   ~Stage(void) noexcept(false);
  
    StageSpec* pStageSpec(void);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
