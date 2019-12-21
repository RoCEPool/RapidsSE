/*============================================================================*/
/* SP_Record.hpp - Record sprint helper                                       */
/*============================================================================*/
#pragma once

namespace Records
{
  class SP_Record : public Sprint
  {
  public:
    char       m_caRecord[C_16KB];
  
    char*      m_azAppendCursor;   // cursor for StrLib::vAppend()
    StreamDef* m_pStreamDef;       // stream def
    i8         m_iSubSet;          // stream def subset

    // must be last
    Footer m_xFooter;
    
  public:
    SP_Record(void);
   ~SP_Record(void) noexcept(false);

    char* azRecord(void);
  
    void vReset(void);

    void vRow(Row* pRow);
  
    Stz* sz(void);
    Stz* sz(Row* pRow);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
