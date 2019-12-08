/*============================================================================*/
/* Record.hpp -- Record                                                       */
/*============================================================================*/
#pragma once

namespace Transform
{
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Record : public Object
  {
  private:
    s16KB      m_sRecStr;        // the record as a string
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
    
    void vAppend(Column* pCol, Datum* pDtmVal);
    void vAppend(Column* pCol, Row* pRow);
    void vAppendBcd(Column* pCol, Datum* pDtmVal);
    void vAppendBol(Column* pCol, Datum* pDtmVal);
    void vAppendDte(Column* pCol, Datum* pDtmVal);
    void vAppendFlt(Column* pCol, Datum* pDtmVal);
    void vAppendInt(Column* pCol, Datum* pDtmVal);
    void vAppendIvl(Column* pCol, Datum* pDtmVal);
    void vAppendTme(Column* pCol, Datum* pDtmVal);
    void vAppendTsp(Column* pCol, Datum* pDtmVal);
    void vAppendVch(Column* pCol, Datum* pDtmVal);

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
