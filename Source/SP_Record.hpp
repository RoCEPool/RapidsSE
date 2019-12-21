/*============================================================================*/
/* SP_Record.hpp - Reocrd sprint helper                                       */
/*============================================================================*/
#pragma once

namespace Records
{
  class SP_Record : public Sprint
  {
  public:
  
    char m_caRow[C_16KB];
  
    // must be last
    Footer m_xFooter;
    
  public:
    SP_Record(void);
   ~SP_Record(void) noexcept(false);

    void vRow(Row* pRow);
  
    Stz* sz(void);
    Stz* sz(Row* pRow);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
