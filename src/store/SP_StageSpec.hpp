/*============================================================================*/
/* SP_StageSpec.hpp - StageSpec sprint helper                                 */
/*============================================================================*/
#pragma once

namespace STORE
{
  class SP_StageSpec : public SPmedium
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    SP_StageSpec(void);
    SP_StageSpec(StageSpec* pStageSpec);
   ~SP_StageSpec(void) noexcept(false);

    void vStageSpec(StageSpec* pStageSpec);
  
    Stz* sz(void);
    Stz* sz(StageSpec* pStageSpec);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
