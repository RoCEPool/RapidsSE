/*============================================================================*/
/* SP_Pipeline.hpp - Pipeline sprint helper                                   */
/*============================================================================*/
#pragma once

namespace STORE
{
  class SP_Pipeline : public SPmedium
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    SP_Pipeline(void);
    SP_Pipeline(Pipeline* pPipeline);
   ~SP_Pipeline(void) noexcept(false);

    void vPipeline(Pipeline* pPipeline);
  
    Stz* sz(void);
    Stz* sz(Pipeline* pPipeline);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
