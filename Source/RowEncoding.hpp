/*============================================================================*/
/* RowEncoding.hpp -- Row Encoding                                            */
/*============================================================================*/
#pragma once

namespace Data
{
  class RowEncoding : public Object,
                      private RowCoder
  {
  public:
    ColumnVec* m_pColVec;
    i8         m_nColumns;
  
    // - - - - - - - - - - - - - - - - - -
  
    struct {
      eMC m_eMetaCode;
      Dtm m_wDataCode;
      i8  m_nDataBytes;
      i8  m_nOffset;
    } xEncode[C_MAX_COLUMNS];

    Wsp_t<C_MAX_RECORD> m_xWsp;  // sz workspace
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);

    RowEncoding(ColumnVec* pColVec);
    ~RowEncoding(void) noexcept(false);
  
    void vEncode(Row* pRow);
    void vEncodeBol(Row* pRow, Column* pCol);
    void vEncodeBcd(Row* pRow, Column* pCol);
    void vEncodeDte(Row* pRow, Column* pCol);
    void vEncodeFlt(Row* pRow, Column* pCol);
    void vEncodeInt(Row* pRow, Column* pCol);
    void vEncodeIvl(Row* pRow, Column* pCol);
    void vEncodeTme(Row* pRow, Column* pCol);
    void vEncodeTsp(Row* pRow, Column* pCol);
    void vEncodeVch(Row* pRow, Column* pCol);

    void vCodeBol(Dtm wBol, eMC& eMetaCode_, Dtm& wDataCode_, i8& nDataBytes_);
    void vCodeBcd(Dtm wBcd, eMC& eMetaCode_, Dtm& wDataCode_, i8& nDataBytes_);
    void vCodeDte(Dtm wDte, eMC& eMetaCode_, Dtm& wDataCode_, i8& nDataBytes_);
    void vCodeFlt(Dtm wFlt, eMC& eMetaCode_, Dtm& wDataCode_, i8& nDataBytes_);
    void vCodeInt(Dtm wInt, eMC& eMetaCode_, Dtm& wDataCode_, i8& nDataBytes_);
    void vCodeIvl(Dtm wIvl, eMC& eMetaCode_, Dtm& wDataCode_, i8& nDataBytes_);
    void vCodeTme(Dtm wTme, eMC& eMetaCode_, Dtm& wDataCode_, i8& nDataBytes_);
    void vCodeTsp(Dtm wTsp, eMC& eMetaCode_, Dtm& wDataCode_, i8& nDataBytes_);
    void vCodeVch(Dtm wVch, eMC& eMetaCode_, Dtm& wDataCode_, i8& nDataBytes_);
  
    void vReset(void);

  private:
    Footer* aFooter(void);
  };
  
  RowEncoding* _pRowEncoding(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
