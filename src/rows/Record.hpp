/*============================================================================*/
/* Record.hpp - Record sprint helper                                       */
/*============================================================================*/
#pragma once

namespace Rows
{
  class Record : public StackObject
  {
  public:
    char       m_caRecord[C_MAX_RECORD];
  
    char*      m_azCursor;    // append cursor for StrLib::vAppend()
    StreamDef* m_pStreamDef;  // stream def
    i8         m_iSubSet;     // stream def subset

    // must be last
    Footer m_xFooter;
    
  public:
    Record(void);
    Record(StreamDef* pStreamDef, i8 iSubSet);
    Record(RowSingle* pRowSingle);
   ~Record(void) noexcept(false);
  
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
    Stz* sz(Row* pRow);
    Stz* sz(RowSingle* pRowSingle);
  
    void vToRecord(RowSingle* pRow);
  
    // from Record
  
    void vAppend(RowSingle* pRow);

    void vPrintRow(void);
    void vPrintPartialRow(void);

    void vRecStr(Stz* szRecStr);
    Stz* szRecStr(void);

    Footer* aFooter(void);
  };

  Record* _pRecord(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
