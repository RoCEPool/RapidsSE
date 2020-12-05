/*============================================================================*/
/* RowSlab.hpp -- Row Slab                                                    */
/*============================================================================*/
#pragma once

namespace Rows
{
  static const i8 C_ROWSLAB_WSP    = C_SIZE_LARGE_OBJECT - C_16KB;
  static const i8 C_ROWSINGLE_WSP  = C_MAX_RECORD;
  static const i8 C_SLAB_CELLS     = C_ROWSLAB_WSP / C_BYTES_PER_WORD;
  static const i8 C_FIRST_LGL_CELL = C_SLAB_CELLS - 1;

  static const i8 C_CELL_BOUNDARY  = C_SLAB_CELLS;

  static const i8 C_NML_STR_SPACE  = C_8KB;

}

namespace Rows
{
  class RowSlab : public Object,
                  public RowBox_t<C_WSP_ROWBUFFER>
  {
    friend class Row;

  private:
  
      
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    RowSlab(void);
   ~RowSlab(void) noexcept(false);
        
    //ColPix* axaColPix(void);
//    i8    iColumn(Row* aRow, i8 iColumn);
//    void  vCopyPix(StreamPix* pStreamPixSrc);
//    void  vCopyRow(Row* aRowSrc, bool bSrcHasPtnHash, bool bCopyPtnHash);
    void  vDiscardInvalidRow(void);
    void  vDumpAllRows(void);
    void  vDumpNst(void);
    void  vDumpRow(i8 i_Row);
    void  vDumpRow(i8 i_Row, Row* aRow);
    i8    nExtraCols(void);
    void  vInitPix(StreamDef* pStreamDef, bool bPtnHash);
    NmlStr* pNmlStr(void);
    void  vNormaliseStrings(void);
    i8    iPtnOfRow(Row* aRow);
    void  vPositionOnCell(i8 i_Col);
    bool  bPtnHash(void);
    void  vReset(void);
    i8    nRowProperBytes(void);
    Row*  aRowByOfs(i8 iOfs);
    void  vSetRow(i8 iRow);
    i8    iOffsetInWsp(Row* aRow);
    void  vSlabReport(void);
    StreamPix* aStreamPix(void);
    void  vZeroRow(Row* aRow);

  private:
    i8    i_RowSlabCell(i8 i_Row);
    m8*   amSlabCellVec(void);
  
    Footer* aFooter(void);
  
  public:
  
  
  };
  
  RowSlab* _pRowSlab(void* aAddr);
}

/*==========================================================================*/
/* end                                                                      */
/*==========================================================================*/
