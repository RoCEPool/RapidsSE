/*============================================================================*/
/* SP_FormworkSpec.hpp - FormworkSpec sprint helper                           */
/*============================================================================*/
#pragma once

namespace STORE
{
  class SP_FormworkSpec : public SPmedium
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    SP_FormworkSpec(void);
    SP_FormworkSpec(PipelineSpec* pPipelineSpec);
   ~SP_FormworkSpec(void) noexcept(false);

    void vPipelineSpec(PipelineSpec* pPipelineSpec);
  
    Stz* sz(void);
    Stz* sz(PipelineSpec* pPipelineSpec);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
