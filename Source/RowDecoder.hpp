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
    
    w8 wDecode(Column* pColumn);
    
    void vPrint(Column* pColumn);
  
  private:
    w8 wDecodeBcd(Column* pColumn, i8 iEncoding, i8 i_Offset);
    w8 wDecodeBol(Column* pColumn, i8 iEncoding, i8 i_Offset);
    w8 wDecodeDte(Column* pColumn, i8 iEncoding, i8 i_Offset);
    w8 wDecodeFlt(Column* pColumn, i8 iEncoding, i8 i_Offset);
    w8 wDecodeInt(Column* pColumn, i8 iEncoding, i8 i_Offset);
    w8 wDecodeIvl(Column* pColumn, i8 iEncoding, i8 i_Offset);
    w8 wDecodeTme(Column* pColumn, i8 iEncoding, i8 i_Offset);
    w8 wDecodeTsp(Column* pColumn, i8 iEncoding, i8 i_Offset);
    w8 wDecodeVch(Column* pColumn, i8 iEncoding, i8 i_Offset);
  
    static i8 iDteDecoding(WordView& wTspView, i8 iEncoding);
    static i8 iTmeDecoding(WordView& wTspView, i8 iEncoding);
    static i8 iTspDecoding(WordView& wTspView, i8 iEncoding);
  
    static i8 iGetEncoding(i8 i_Column, CacheLine* aCacheLine, i8& i_Offset);
   
    static void vPrint(Column* pColumn, WordView& wDatum);
  
    Footer*      aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
