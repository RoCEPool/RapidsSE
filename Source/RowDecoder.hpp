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

    void vDecode(i8 i_Row, Column* pCol, w8& wDatum, Stz* szBuf);
    void vPrint(i8 i_Row, Column* pCol);
    void vString(s48K& sStr, i8 i_Row, ColumnVec* pColVec);

  private:
  
    void vAppend(s48K& sStr, i8 i_Row, Column* pCol);
    void vAppendBcd(s48K& sStr, Column* pCol, w8 wDatum, Stz* szBuf);
    void vAppendBol(s48K& sStr, Column* pCol, w8 wDatum, Stz* szBuf);
    void vAppendDte(s48K& sStr, Column* pCol, w8 wDatum, Stz* szBuf);
    void vAppendFlt(s48K& sStr, Column* pCol, w8 wDatum, Stz* szBuf);
    void vAppendInt(s48K& sStr, Column* pCol, w8 wDatum, Stz* szBuf);
    void vAppendIvl(s48K& sStr, Column* pCol, w8 wDatum, Stz* szBuf);
    void vAppendTme(s48K& sStr, Column* pCol, w8 wDatum, Stz* szBuf);
    void vAppendTsp(s48K& sStr, Column* pCol, w8 wDatum, Stz* szBuf);
    void vAppendVch(s48K& sStr, Column* pCol, w8 wDatum, Stz* szBuf);
  
    void vDecodeBcd(i8 i_Row, Column* pCol, w8& wDatum, Stz* szBuf);
    void vDecodeBol(i8 i_Row, Column* pCol, w8& wDatum, Stz* szBuf);
    void vDecodeDte(i8 i_Row, Column* pCol, w8& wDatum, Stz* szBuf);
    void vDecodeFlt(i8 i_Row, Column* pCol, w8& wDatum, Stz* szBuf);
    void vDecodeInt(i8 i_Row, Column* pCol, w8& wDatum, Stz* szBuf);
    void vDecodeIvl(i8 i_Row, Column* pCol, w8& wDatum, Stz* szBuf);
    void vDecodeTme(i8 i_Row, Column* pCol, w8& wDatum, Stz* szBuf);
    void vDecodeTsp(i8 i_Row, Column* pCol, w8& wDatum, Stz* szBuf);
    void vDecodeVch(i8 i_Row, Column* pCol, w8& wDatum, Stz* szBuf);

    i8 iGetEncoding(i8 i_Column, CacheLine* aCacheLine, i8& i_Offset);
  
    //! more work here...
    w8 wRestoreDte(w8 wDteStripped, i8 iEncoding);
    w8 wRestoreTme(w8 wTmeStripped, i8 iEncoding);
    w8 wRestoreTsp(w8 wTspStripped, i8 iEncoding);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
