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

    void vDecode(i8 i_Row, Column* pCol, w8& wDatum, Stz* szDatum);
    void vPrint(i8 i_Row, Column* pCol);
    void vRecord(Record* pRec, i8 i_Row, ColumnVec* pColVec);

  private:
  
    void vAppend(Record* pRec, i8 i_Row, Column* pCol);
    void vAppendBcd(Record* pRec, Column* pCol, w8 wDatum, Stz* szDatum);
    void vAppendBol(Record* pRec, Column* pCol, w8 wDatum, Stz* szDatum);
    void vAppendDte(Record* pRec, Column* pCol, w8 wDatum, Stz* szDatum);
    void vAppendFlt(Record* pRec, Column* pCol, w8 wDatum, Stz* szDatum);
    void vAppendInt(Record* pRec, Column* pCol, w8 wDatum, Stz* szDatum);
    void vAppendIvl(Record* pRec, Column* pCol, w8 wDatum, Stz* szDatum);
    void vAppendTme(Record* pRec, Column* pCol, w8 wDatum, Stz* szDatum);
    void vAppendTsp(Record* pRec, Column* pCol, w8 wDatum, Stz* szDatum);
    void vAppendVch(Record* pRec, Column* pCol, w8 wDatum, Stz* szDatum);
  
    void vDecodeBcd(i8 i_Row, Column* pCol, w8& wDatum, Stz* szDatum);
    void vDecodeBol(i8 i_Row, Column* pCol, w8& wDatum, Stz* szDatum);
    void vDecodeDte(i8 i_Row, Column* pCol, w8& wDatum, Stz* szDatum);
    void vDecodeFlt(i8 i_Row, Column* pCol, w8& wDatum, Stz* szDatum);
    void vDecodeInt(i8 i_Row, Column* pCol, w8& wDatum, Stz* szDatum);
    void vDecodeIvl(i8 i_Row, Column* pCol, w8& wDatum, Stz* szDatum);
    void vDecodeTme(i8 i_Row, Column* pCol, w8& wDatum, Stz* szDatum);
    void vDecodeTsp(i8 i_Row, Column* pCol, w8& wDatum, Stz* szDatum);
    void vDecodeVch(i8 i_Row, Column* pCol, w8& wDatum, Stz* szDatum);

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
