/*============================================================================*/
/* Record.hpp -- Record                                                       */
/*============================================================================*/
#pragma once

namespace Records
{
  class Record : public SP_Record
  {
  private:
    char*      m_azAppendCursor; // cursor for StrLib::vAppend()
    StreamDef* m_pStreamDef;     // stream def
    i8         m_iSubSet;        // stream def subset
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Record(void);
    Record(StreamDef* pStreamDef, i8 iSubSet);
   ~Record(void) noexcept(false);
  
    void vAppend(Row* pRow);
    
    void vAppend(Column* pCol, Datum* pDatum);
    void vAppend(Column* pCol, Row* pRow);
    void vAppendBcd(Column* pCol, Datum* pDatum);
    void vAppendBol(Column* pCol, Datum* pDatum);
    void vAppendDte(Column* pCol, Datum* pDatum);
    void vAppendFlt(Column* pCol, Datum* pDatum);
    void vAppendInt(Column* pCol, Datum* pDatum);
    void vAppendIvl(Column* pCol, Datum* pDatum);
    void vAppendTme(Column* pCol, Datum* pDatum);
    void vAppendTsp(Column* pCol, Datum* pDatum);
    void vAppendVch(Column* pCol, Datum* pDatum);

    void vPrintRow(void);
    void vPrintPartialRow(void);
  
    void vRecStr(Stz* szRecStr);
    Stz* szRecStr(void);
  
    void vReset(void);
  
  private:
    Footer* aFooter(void);
  };
  
  Record* _pRecord(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
