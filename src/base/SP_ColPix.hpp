/*============================================================================*/
/* SP_ColPix.hpp - Column Pix Sprint Helper                                   */
/*============================================================================*/
#pragma once

namespace BASE
{
  class SP_ColPix : public SP
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    SP_ColPix(void);
   ~SP_ColPix(void) noexcept(false);
  
    Stz* sz(ColPix* pColPix);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
