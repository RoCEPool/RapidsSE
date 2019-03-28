/*============================================================================*/
/* RowEncoder.hpp -- Row Encoder                                              */
/*============================================================================*/
#pragma once

namespace Data
{
  static const i8 C_CACHELINES = 256;
  
  typedef TrayNL_t<CN_ROWTRAY,CacheLine,C_CACHELINES> RowParts;
  
  class RowEncoder : public Object
  {
  private:
    RowParts* m_pRowParts;
    i8        m_i_CacheOffset;
  
  // must be last
  Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    RowEncoder(RowParts* pRowParts);
    ~RowEncoder(void) noexcept(false);

    void vEncode(Column* pColumn, Datum* pDatum, i8& iHash);
    RowParts* pRowParts(void);
  
  private:
    void vEncodeBcd(Column* pColumn, Datum* pDatum, i8& iHash);
    void vEncodeBol(Column* pColumn, Datum* pDatum, i8& iHash);
    void vEncodeDte(Column* pColumn, Datum* pDatum, i8& iHash);
    void vEncodeFlt(Column* pColumn, Datum* pDatum, i8& iHash);
    void vEncodeInt(Column* pColumn, Datum* pDatum, i8& iHash);
    void vEncodeIvl(Column* pColumn, Datum* pDatum, i8& iHash);
    void vEncodeTme(Column* pColumn, Datum* pDatum, i8& iHash);
    void vEncodeTsp(Column* pColumn, Datum* pDatum, i8& iHash);
    void vEncodeVch(Column* pColumn, Datum* pDatum, i8& iHash);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
