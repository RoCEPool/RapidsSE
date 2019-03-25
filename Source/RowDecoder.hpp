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

    void vAsString(s48K& sDataString, ColumnVec* pColumnVec);
    
    w8 wDecode(Column* pColumn, Stz* szBuf);
  
    void vPrint(Column* pColumn);
  
  private:
    w8 wDecodeBcd(Column* pColumn, Stz* szBuf);
    w8 wDecodeBol(Column* pColumn, Stz* szBuf);
    w8 wDecodeDte(Column* pColumn, Stz* szBuf);
    w8 wDecodeFlt(Column* pColumn, Stz* szBuf);
    w8 wDecodeInt(Column* pColumn, Stz* szBuf);
    w8 wDecodeIvl(Column* pColumn, Stz* szBuf);
    w8 wDecodeTme(Column* pColumn, Stz* szBuf);
    w8 wDecodeTsp(Column* pColumn, Stz* szBuf);
    w8 wDecodeVch(Column* pColumn, Stz* szBuf);
  
    static i8 iDteDecoding(WordView& wDteView, i8 iEncoding);
    static i8 iTmeDecoding(WordView& wTmeView, i8 iEncoding);
    static i8 iTspDecoding(WordView& wTspView, i8 iEncoding);
  
    static i8 iGetEncoding(i8 i_Column, CacheLine* aCacheLine, i8& i_Offset);
   
    static void vPrint(Column* pColumn, WordView& wDatum);
  
    Footer*      aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
