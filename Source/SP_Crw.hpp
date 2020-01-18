/*============================================================================*/
/* SP_Crw.hpp - Compact Row sprint helper                                     */
/*============================================================================*/
#pragma once

namespace CompactBuffers
{
  class SP_Crw : public SP_Record
  {
  public:
  
    char  m_caCrw[C_MAX_RECORD];

    // must be last
    Footer m_xFooter;
    
  public:
    SP_Crw(void);
   ~SP_Crw(void) noexcept(false);

    Stz* sz(void);
    Stz* sz(Crw* aCrw, StreamDef* pStreamDef);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
