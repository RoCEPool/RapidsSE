/*============================================================================*/
/* Record.hpp -- Record                                                       */
/*============================================================================*/
#pragma once

namespace Data
{
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Record : public Object
  {
  private:
    s16KB      m_sRecStr;        // the record as a string
    char*      m_azAppendCursor; // cursor for StrLib::vAppend()
    ColumnVec* m_pColumnVec;     // column vector of table
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Record(void);
    Record(ColumnVec* pColumnVec);
    ~Record(void) noexcept(false);
  
    void vAppend(Row* pRow);
    
    void vAppend(Column* pCol, Datum* pDatum);
    void vAppend(Column* pCol, Row* pRow);
    void vAppendBcd(Column* pCol, Dtm wDtm);
    void vAppendBol(Column* pCol, Dtm wDtm);
    void vAppendDte(Column* pCol, Dtm wDtm);
    void vAppendFlt(Column* pCol, Dtm wDtm);
    void vAppendInt(Column* pCol, Dtm wDtm);
    void vAppendIvl(Column* pCol, Dtm wDtm);
    void vAppendTme(Column* pCol, Dtm wDtm);
    void vAppendTsp(Column* pCol, Dtm wDtm);
    void vAppendVch(Column* pCol, Datum* pDatum);

    void vPrintRow(void);
  
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
