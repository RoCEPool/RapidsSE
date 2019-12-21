/*============================================================================*/
/* SP_Record.hpp - Record sprint helper                                       */
/*============================================================================*/
#pragma once

namespace Records
{
  class SP_Record : public Sprint
  {
  public:
  
    char m_caRecord[C_16KB];
  
    // must be last
    Footer m_xFooter;
    
  public:
    SP_Record(void);
   ~SP_Record(void) noexcept(false);

    char* azRecord(void);
    void vRow(Row* pRow);
  
    Stz* sz(void);
    Stz* sz(Row* pRow);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
