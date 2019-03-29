/*============================================================================*/
/* RowDecoder.hpp -- Row Decoder                                              */
/*============================================================================*/
#pragma once

namespace Data
{  
  class RowDecoder : public Object
  {
  public:  
    RowTray*  m_pRowTray; // tray of rows

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    RowDecoder(RowTray* pRowTray);
    ~RowDecoder(void) noexcept(false);

    void vDecode(i8 i_Row, Column* pCol, Datum* pDatum);

  private:
  
    void vDecodeBcd(i8 i_Row, Column* pCol, Datum* pDatum);
    void vDecodeBol(i8 i_Row, Column* pCol, Datum* pDatum);
    void vDecodeDte(i8 i_Row, Column* pCol, Datum* pDatum);
    void vDecodeFlt(i8 i_Row, Column* pCol, Datum* pDatum);
    void vDecodeInt(i8 i_Row, Column* pCol, Datum* pDatum);
    void vDecodeIvl(i8 i_Row, Column* pCol, Datum* pDatum);
    void vDecodeTme(i8 i_Row, Column* pCol, Datum* pDatum);
    void vDecodeTsp(i8 i_Row, Column* pCol, Datum* pDatum);
    void vDecodeVch(i8 i_Row, Column* pCol, Datum* pDatum);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
