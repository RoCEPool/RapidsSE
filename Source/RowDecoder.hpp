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
    w8 wDecodeBcd(i8 i_Row, Column* pCol, Stz* szBuf);
    w8 wDecodeBol(i8 i_Row, Column* pCol, Stz* szBuf);
    w8 wDecodeDte(i8 i_Row, Column* pCol, Stz* szBuf);
    w8 wDecodeFlt(i8 i_Row, Column* pCol, Stz* szBuf);
    w8 wDecodeInt(i8 i_Row, Column* pCol, Stz* szBuf);
    w8 wDecodeIvl(i8 i_Row, Column* pCol, Stz* szBuf);
    w8 wDecodeTme(i8 i_Row, Column* pCol, Stz* szBuf);
    w8 wDecodeTsp(i8 i_Row, Column* pCol, Stz* szBuf);
    w8 wDecodeVch(i8 i_Row, Column* pCol, Stz* szBuf);

    void vStringBcd(Column* pCol, w8 wDatum, Stz* szBuf, s48K& sStr);
    void vStringBol(Column* pCol, w8 wDatum, Stz* szBuf, s48K& sStr);
    void vStringDte(Column* pCol, w8 wDatum, Stz* szBuf, s48K& sStr);
    void vStringFlt(Column* pCol, w8 wDatum, Stz* szBuf, s48K& sStr);
    void vStringInt(Column* pCol, w8 wDatum, Stz* szBuf, s48K& sStr);
    void vStringIvl(Column* pCol, w8 wDatum, Stz* szBuf, s48K& sStr);
    void vStringTme(Column* pCol, w8 wDatum, Stz* szBuf, s48K& sStr);
    void vStringTsp(Column* pCol, w8 wDatum, Stz* szBuf, s48K& sStr);
    void vStringVch(Column* pCol, w8 wDatum, Stz* szBuf, s48K& sStr);

    static i8 iDteDecoding(WordView& wDteView, i8 iEncoding);
    static i8 iTmeDecoding(WordView& wTmeView, i8 iEncoding);
    static i8 iTspDecoding(WordView& wTspView, i8 iEncoding);
  
    static i8 iGetEncoding(i8 i_Column, CacheLine* aCacheLine, i8& i_Offset);
   
    static void vPrint(Column* pCol, WordView& wDatum);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
