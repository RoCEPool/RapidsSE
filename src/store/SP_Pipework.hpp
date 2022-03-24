/*============================================================================*/
/* SP_Pipework.hpp - PipeWork sprint helper                                   */
/*============================================================================*/
#pragma once

namespace STORE
{
  class SP_Pipework : public SPmedium
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    SP_Pipework(void);
    SP_Pipework(Pipeline* pPipeline, bool bDescend=false);
   ~SP_Pipework(void) noexcept(false);

    void vPipeline(Pipeline* pPipeline, bool bDescend=false);
  
    Stz* sz(void);
    Stz* sz(Pipeline* pPipeline);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
