/*============================================================================*/
/* RowSlab.hpp -- Row Slab                                                    */
/*============================================================================*/
#pragma once

namespace Rows
{
  static const i8 C_WSP_ROWSLAB   = C_SIZE_LARGE_OBJECT - C_2KB;
  static const i8 C_SLAB_CELLS    = C_WSP_ROWSLAB / C_BYTES_PER_WORD;
  static const i8 C_FIRST_CELL    = C_SLAB_CELLS - 1;

  static const i8 C_NML_STR_SPACE = C_8KB;

  class RowSlab : public Object
  {

  private:
  
    char caModel[32];  // model string
    i8   m_iVersion;    //
    i8   m_iFormat;
  
    i8   m_nBytes;      // size in bytes of this row slab
    i8   m_nTrimmed;    // how many bytes trimmed  NOT USED YET
    i8   m_i_Trim;      // where trim occurred     NOT USED YET

    i8   m_nItems;      // number of items in this row-slab
    i8   m_nMaxRowSize; // max size of row
 
    i8   m_nCols;       // number of columns per row in this row-slab
    i8   m_nExtraCols;  //  of which there are nExtra metadata columns
    bool m_bPtnHash;    //
    i8   m_i_Cell;      // 
  
    ColPix m_xaColPix[C_MAX_DEFCOLUMNS]; // the column pix
  
    WspConverge_t<C_WSP_ROWSLAB> m_xWsp; // workspace
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    RowSlab(void);
   ~RowSlab(void) noexcept(false);
      
    i8    iCell(i8 i_Row, i8 i_Cell);
    m8*   amCell(i8 i_Cell);
    m8*   amCellVector(void);
    ColPix* axaColPix(void);
    i8    nCols(void);
    void  vDeposit(d8 dVal);
    void  vDeposit(i8 iVal);
    char* azDepositString(void);
    void  vDepositedString(i8 nSize);
    void  vDumpAllRows(void);
    void  vDumpRow(i8 i_Row);
    i8    nFreeSpace(void);
    void  vInitPix(StreamDef* pStreamDef, bool bPtnHash);
    bool  bNewRow(void);
    Rsb*  aRsb(i8 i_Row);
    m8*   amRowCells(i8 i_Row);
    i8    nRows(void);
    void  vSlabReport(void);

  private:
    void vColHints(StreamDef* pStreamDef);
  
    Footer* aFooter(void);
  };
  
  RowSlab* _pRowSlab(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
