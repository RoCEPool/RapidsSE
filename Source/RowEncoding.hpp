/*============================================================================*/
/* RowEncoding.hpp -- Row Encoding                                            */
/*============================================================================*/
#pragma once

namespace Data
{
  class RowEncoding : public Object
  {
  public:
    ColumnVec* m_pColVec;
    i8         m_nColumns;
  
    // - - - - - - - - - - - - - - - - - -
  
    Dtm m_waColVal[C_MAX_COLUMNS]; // column value array
    Wsp_t<C_MAX_RECORD> m_xWsp;    // sz workspace
  
    struct {
      eMC m_eEncoding;
      i8  m_nEncodedBytes;
    } xEncode[C_MAX_COLUMNS];

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

    void vCodeBol(Dtm wBol, Dtm wEncBol_, eMC& eMetaCode_, i8& nBytes_);
    void vCodeBcd(Dtm wBcd, Dtm wEncBcd_, eMC& eMetaCode_, i8& nBytes_);
    void vCodeDte(Dtm wDte, Dtm wEncDte_, eMC& eMetaCode_, i8& nBytes_);
    void vCodeFlt(Dtm wFlt, Dtm wEncFlt_, eMC& eMetaCode_, i8& nBytes_);
    void vCodeInt(Dtm wInt, Dtm wEncInt_, eMC& eMetaCode_, i8& nBytes_);
    void vCodeIvl(Dtm wIvl, Dtm wEncIvl_, eMC& eMetaCode_, i8& nBytes_);
    void vCodeTme(Dtm wTme, Dtm wEncTme_, eMC& eMetaCode_, i8& nBytes_);
    void vCodeTsp(Dtm wTsp, Dtm wEncTsp_, eMC& eMetaCode_, i8& nBytes_);
    void vCodeVch(Dtm wVch, Dtm wEncVch_, eMC& eMetaCode_, i8& nBytes_);

    void vReset(void);

  private:
    Footer* aFooter(void);
  };
  
  RowEncoding* _pRowEncoding(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
