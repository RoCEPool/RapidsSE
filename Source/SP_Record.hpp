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
  
    void vAppendDtm(Column* pCol, Datum* pDatum);
    void vAppendBcd(Column* pCol, Datum* pDatum);
    void vAppendBol(Column* pCol, Datum* pDatum);
    void vAppendDte(Column* pCol, Datum* pDatum);
    void vAppendFlt(Column* pCol, Datum* pDatum);
    void vAppendInt(Column* pCol, Datum* pDatum);
    void vAppendIvl(Column* pCol, Datum* pDatum);
    void vAppendTme(Column* pCol, Datum* pDatum);
    void vAppendTsp(Column* pCol, Datum* pDatum);
    void vAppendVch(Column* pCol, Datum* pDatum);

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
