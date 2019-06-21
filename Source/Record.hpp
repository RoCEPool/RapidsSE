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
    
    void vAppend(Column* pCol, DtmVal* pDtmVal);
    void vAppend(Column* pCol, Row* pRow);
    void vAppendBcd(Column* pCol, DtmVal* pDtmVal);
    void vAppendBol(Column* pCol, DtmVal* pDtmVal);
    void vAppendDte(Column* pCol, DtmVal* pDtmVal);
    void vAppendFlt(Column* pCol, DtmVal* pDtmVal);
    void vAppendInt(Column* pCol, DtmVal* pDtmVal);
    void vAppendIvl(Column* pCol, DtmVal* pDtmVal);
    void vAppendTme(Column* pCol, DtmVal* pDtmVal);
    void vAppendTsp(Column* pCol, DtmVal* pDtmVal);
    void vAppendVch(Column* pCol, DtmVal* pDtmVal);

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
