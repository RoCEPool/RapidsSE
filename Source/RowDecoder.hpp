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

  
    static void vAsString(s48K& sDataString,
                          ColumnVec* pColumnVec, CacheLine* aCacheLine);
    
    static w8 wDecode(Column* pColumn);
  
    static w8 wDecode(Column* pColumn, CacheLine* aCacheLine);
  
    static void vPrint(Column* pColumn, CacheLine* aCacheLine);
  
  private:
    static w8 wDecodeBcd(Column* pColumn, CacheLine* aCacheLine);
    static w8 wDecodeBol(Column* pColumn, CacheLine* aCacheLine);
    static w8 wDecodeDte(Column* pColumn, CacheLine* aCacheLine);
    static w8 wDecodeFlt(Column* pColumn, CacheLine* aCacheLine);
    static w8 wDecodeInt(Column* pColumn, CacheLine* aCacheLine);
    static w8 wDecodeIvl(Column* pColumn, CacheLine* aCacheLine);
    static w8 wDecodeTme(Column* pColumn, CacheLine* aCacheLine);
    static w8 wDecodeTsp(Column* pColumn, CacheLine* aCacheLine);
    static w8 wDecodeVch(Column* pColumn, CacheLine* aCacheLine);
  
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
