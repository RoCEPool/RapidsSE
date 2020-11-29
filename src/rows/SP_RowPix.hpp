/*============================================================================*/
/* SP_RowPix.hpp - RowPix sprint helper                                       */
/*============================================================================*/
#pragma once

namespace Rows
{
  class SP_RowPix : public Sprint
  {
  public:
    i8       m_nCols;
    ColPix*  m_axaColPix;
  
    char*    m_azCursor;    // append cursor for StrLib::vAppend()
    char     m_caRecord[C_MAX_RECORD];

    // must be last
    Footer m_xFooter;
    
  public:
    SP_RowPix(i8 nCols, ColPix* axaColPix);
   ~SP_RowPix(void) noexcept(false);
    
    Stz* sz(Row* aRow, bool bNewLine = false);

  private:
    void vAppendBcd(Row* aRow, i8 i_Col, ColPix xColPix);
    void vAppendBol(Row* aRow, i8 i_Col, ColPix xColPix);
    void vAppendDte(Row* aRow, i8 i_Col, ColPix xColPix);
    void vAppendFlt(Row* aRow, i8 i_Col, ColPix xColPix);
    void vAppendInt(Row* aRow, i8 i_Col, ColPix xColPix);
    void vAppendIvl(Row* aRow, i8 i_Col, ColPix xColPix);
    void vAppendTme(Row* aRow, i8 i_Col, ColPix xColPix);
    void vAppendTsp(Row* aRow, i8 i_Col, ColPix xColPix);
    void vAppendVch(Row* aRow, i8 i_Col, ColPix xColPix);

    void vPutRow(Row* aRow, bool bNewLine);
    char* azRecord(void);
    void vReset(void);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
