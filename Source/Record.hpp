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
  
    void vAppend(ColumnVec* pColVec, Row* pRow);
    
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
  
    void vRecStr(Stz* szRecStr);
    Stz* szRecStr(void);
  
    void vReset(void);

    //! REVIEW WHICH FUNCTIONS USE THESE
    static void vSprint(Column* pColumn, Stz* aDst, Stz* aSrc);
    static void vSprintQuotes(Column* pColumn, Stz* aDst, Stz* aSrc);
    static void vSprintEscaped(Column* pColumn, Stz* aDst, Stz* aSrc);
  
  private:
    Footer* aFooter(void);
  };
  
  Record* _pRecord(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
