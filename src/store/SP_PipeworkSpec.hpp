/*============================================================================*/
/* SP_PipeworkSpec.hpp - PipeworkSpec sprint helper                           */
/*============================================================================*/
#pragma once

namespace STORE
{
  class SP_PipeworkSpec : public SPmedium
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    SP_PipeworkSpec(void);
    SP_PipeworkSpec(PipelineSpec* pPipelineSpec);
   ~SP_PipeworkSpec(void) noexcept(false);

    void vPipelineSpec(PipelineSpec* pPipelineSpec);
  
    Stz* sz(void);
    Stz* sz(PipelineSpec* pPipelineSpec);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
