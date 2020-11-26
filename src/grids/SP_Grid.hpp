/*============================================================================*/
/* SP_Grid.hpp - Grid sprint helper                                           */
/*============================================================================*/
#pragma once

namespace Grids
{
  class SP_Grid : public SP
  {
  public:

    // must be last
    Footer m_xFooter;
    
  public:
    SP_Grid(void);
   ~SP_Grid(void) noexcept(false);
  
    Stz* sz(Grid* pGrid);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
