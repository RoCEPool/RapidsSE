/*============================================================================*/
/* SP_Stream.hpp - Stream sprint helper                                       */
/*============================================================================*/
#pragma once

namespace STORE
{
  class SP_Stream : public SPmedium
  {
  public:
  
    // must be last
    Footer m_xFooter;
    
  public:
    SP_Stream(void);
    SP_Stream(Stream* pStream, bool bDescend=false);
   ~SP_Stream(void) noexcept(false);

    void vStream(Stream* pStream, bool bDescend=false);
  
    Stz* sz(void);
    Stz* sz(Stream* pStream);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
