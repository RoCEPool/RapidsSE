/*============================================================================*/
/* SP_PipeSpec.hpp - PipeSpec sprint helper                           */
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
    SP_PipeSpec(VertexSpec* pPipeSpec);
   ~SP_PipeSpec(void) noexcept(false);

    void vPipeSpec(VertexSpec* pPipeSpec);
    void vPipeSpec(VertexSpec* pPipeSpec, i8 iDepth);

    Stz* sz(void);
    Stz* sz(VertexSpec* pPipeSpec);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
