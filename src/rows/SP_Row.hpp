/*============================================================================*/
/* SP_Row.hpp - Row sprint helper                                             */
/*============================================================================*/
#pragma once

namespace Rows
{
  class SP_Row : public Sprint
  {
  public:
  
    char m_caRow[C_16KB];
  
    // must be last
    Footer m_xFooter;
    
  public:
    SP_Row(void);
   ~SP_Row(void) noexcept(false);

    void vRow(RowSingle* pRow);
  
    Stz* sz(void);
    Stz* sz(RowSingle* pRow);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
