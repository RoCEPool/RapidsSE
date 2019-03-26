/*============================================================================*/
/* RowDecoder.hpp -- Row Decoder                                              */
/*============================================================================*/
#pragma once

namespace Data
{  
  class RowDecoder : public Object
  {
  public:  
    RowTray*  m_pRowTray;            // tray of rows

  // must be last
  Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    RowDecoder(RowTray* pRowTray);
    ~RowDecoder(void) noexcept(false);

    w8 wDecode(i8 i_Row, Column* pCol, Stz* szBuf);
    void vPrint(i8 i_Row, Column* pCol);
    void vString(i8 i_Row, ColumnVec* pColVec, s48K& sStr);

  private:
  
    void vAppend(i8 i_Row, Column* pCol, s48K& sStr);
    void vAppendBcd(Column* pCol, w8 wDatum, Stz* szBuf, s48K& sStr);
    void vAppendBol(Column* pCol, w8 wDatum, Stz* szBuf, s48K& sStr);
    void vAppendDte(Column* pCol, w8 wDatum, Stz* szBuf, s48K& sStr);
    void vAppendFlt(Column* pCol, w8 wDatum, Stz* szBuf, s48K& sStr);
    void vAppendInt(Column* pCol, w8 wDatum, Stz* szBuf, s48K& sStr);
    void vAppendIvl(Column* pCol, w8 wDatum, Stz* szBuf, s48K& sStr);
    void vAppendTme(Column* pCol, w8 wDatum, Stz* szBuf, s48K& sStr);
    void vAppendTsp(Column* pCol, w8 wDatum, Stz* szBuf, s48K& sStr);
    void vAppendVch(Column* pCol, w8 wDatum, Stz* szBuf, s48K& sStr);
  
    //! probably indirectly return the w8...
    w8 wDecodeBcd(i8 i_Row, Column* pCol, Stz* szBuf);
    w8 wDecodeBol(i8 i_Row, Column* pCol, Stz* szBuf);
    w8 wDecodeDte(i8 i_Row, Column* pCol, Stz* szBuf);
    w8 wDecodeFlt(i8 i_Row, Column* pCol, Stz* szBuf);
    w8 wDecodeInt(i8 i_Row, Column* pCol, Stz* szBuf);
    w8 wDecodeIvl(i8 i_Row, Column* pCol, Stz* szBuf);
    w8 wDecodeTme(i8 i_Row, Column* pCol, Stz* szBuf);
    w8 wDecodeTsp(i8 i_Row, Column* pCol, Stz* szBuf);
    w8 wDecodeVch(i8 i_Row, Column* pCol, Stz* szBuf);

    i8 iGetEncoding(i8 i_Column, CacheLine* aCacheLine, i8& i_Offset);
  
    //! more work here...
    i8 iRezeroDte(WordView& wDteView, i8 iEncoding);
    i8 iRezeroTme(WordView& wTmeView, i8 iEncoding);
    i8 iRezeroTsp(WordView& wTspView, i8 iEncoding);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
