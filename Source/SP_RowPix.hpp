/*============================================================================*/
/* SP_RowPix.hpp - RowPix sprint helper                                       */
/*============================================================================*/
#pragma once

namespace Load
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
    
    Stz* sz(i8* aiaRowCells);
  
  private:
  
    void vAppendBcd(ColPix xColPix, i8 iVal);
    void vAppendBol(ColPix xColPix, i8 iVal);
    void vAppendDte(ColPix xColPix, i8 iVal);
    void vAppendFlt(ColPix xColPix, i8 iVal);
    void vAppendFlt(i8* aiaRowCells, i8 i_Cell);
    void vAppendInt(ColPix xColPix, i8 iVal);
    void vAppendIvl(ColPix xColPix, i8 iVal);
    void vAppendTme(ColPix xColPix, i8 iVal);
    void vAppendTsp(ColPix xColPix, i8 iVal);
    void vAppendVch(ColPix xColPix, i8 iVal);

    void vPutRow(i8* aiaRowCells);
  
    char* azRecord(void);
    void vReset(void);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
