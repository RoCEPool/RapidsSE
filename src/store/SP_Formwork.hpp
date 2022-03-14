/*============================================================================*/
/* SP_Formwork.hpp - Formwork sprint helper                                   */
/*============================================================================*/
#pragma once

namespace STORE
{
  class SP_Formwork : public SPmedium
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    SP_Formwork(void);
    SP_Formwork(Pipeline* pPipeline, bool bDescend=false);
   ~SP_Formwork(void) noexcept(false);

    void vPipeline(Pipeline* pPipeline, bool bDescend=false);
  
    Stz* sz(void);
    Stz* sz(Pipeline* pPipeline);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
