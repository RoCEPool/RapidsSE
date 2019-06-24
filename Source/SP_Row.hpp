/*============================================================================*/
/* SP_Row.hpp - Row sprint helper                                             */
/*============================================================================*/
#pragma once

namespace ScanSprint
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

    void vRow(Row* pRow);
  
    Stz* sz(void);
    Stz* sz(Row* pRow);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
