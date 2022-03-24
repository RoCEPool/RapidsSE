/*============================================================================*/
/* SP_PipeSpec.hpp - PipeworkSpec sprint helper                           */
/*============================================================================*/
#pragma once

namespace STORE
{
  class SP_PipeSpec : public SPmedium
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    SP_PipeSpec(void);
    SP_PipeSpec(PipeSpec* pPipeSpec);
   ~SP_PipeSpec(void) noexcept(false);

    void vPipeSpec(PipeSpec* pPipeSpec);
    void vPipeSpec(PipeSpec* pPipeSpec, i8 iDepth);

    Stz* sz(void);
    Stz* sz(PipeSpec* pPipeSpec);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
