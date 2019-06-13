/*============================================================================*/
/* hpRow.hpp - Row sprint helper                                              */
/*============================================================================*/
#pragma once

namespace Transform
{
  class hpRow : public Sprint
  {
  public:
  
    char m_caRow[C_16KB];
  
    // must be last
    Footer m_xFooter;
    
  public:
    hpRow(void);
    ~hpRow(void) noexcept(false);

    void vRow(Row* pRow);
  
    Stz* sz(void);
    Stz* sz(Row* pRow);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
