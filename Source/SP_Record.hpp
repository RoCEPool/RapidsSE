/*============================================================================*/
/* SP_Record.hpp - Record sprint helper                                       */
/*============================================================================*/
#pragma once

namespace Records
{
  class SP_Record : public Sprint
  {
  public:
    char       m_caRecord[C_MAX_RECORD];
  
    char*      m_azCursor;    // append cursor for StrLib::vAppend()
    StreamDef* m_pStreamDef;  // stream def
    i8         m_iSubSet;     // stream def subset

    // must be last
    Footer m_xFooter;
    
  public:
    SP_Record(void);
    SP_Record(StreamDef* pStreamDef);
    SP_Record(RowSingle* pRow);
   ~SP_Record(void) noexcept(false);
  
    void vAddCol(Column* pCol, i8 iVal);
  
    void vAppendCol(Column* pCol, RowSingle* pRow);
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
    Stz* sz(void);
    Stz* sz(Row* aRow);
    Stz* sz(RowSingle* pRow);
  
    void vToRecord(RowSingle* pRow);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
