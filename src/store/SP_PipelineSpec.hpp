/*============================================================================*/
/* SP_PipelineSpec.hpp - PipelineSpec sprint helper                           */
/*============================================================================*/
#pragma once

namespace STORE
{
  class SP_PipelineSpec : public SPmedium
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    SP_PipelineSpec(void);
   ~SP_PipelineSpec(void) noexcept(false);

    void vPipelineSpec(PipelineSpec* pPipelineSpec);
  
    Stz* sz(void);
    Stz* sz(PipelineSpec* pPipelineSpec);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
