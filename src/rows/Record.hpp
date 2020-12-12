/*============================================================================*/
/* Record.hpp - Record sprint helper                                       */
/*============================================================================*/
#pragma once

namespace Rows
{
  class Record : public Object
  {
  public:
    char       m_caRecord[C_MAX_RECORD];
  
    char*      m_azCursor;    // append cursor for StrLib::vAppend()
    StreamDef* m_pStreamDef;  // stream def
    i8         m_iSubSet;     // stream def subset

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Record(void);
    Record(StreamDef* pStreamDef, i8 iSubSet);
    Record(RowSingle* pRowSingle);
   ~Record(void) noexcept(false);
  
  
    char* azRecord(void);
    void vReset(void);
    Stz* sz(void);
    Stz* sz(Row* pRow);
    Stz* sz(RowSingle* pRowSingle);
  
    void vToRecord(RowSingle* pRowSingle);
  
    // from Record
  
    void vAppend(RowSingle* pRow);

    void vPrintAsCsv(void);
    void vPrintAsRow(void);

    void vRecStr(Stz* szRecStr);
    Stz* szRecStr(void);

  private:
    void vAddCol_col(Column* pCol, i8 iVal);

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

    Footer* aFooter(void);
  };

  Record* _pRecord(void* aAddr);

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
