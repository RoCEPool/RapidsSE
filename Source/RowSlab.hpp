/*============================================================================*/
/* RowSlab.hpp -- Row Slab                                                    */
/*============================================================================*/
#pragma once

namespace DataBuffers
{
  class RowSlab : public Object
  {
    static const i8 C_WSP_ROWSLAB = C_SIZE_LARGE_OBJECT - C_4KB;
    static const i8 C_FIRST_CELL  = (C_WSP_ROWSLAB / 8) - 1;

  private:
  
    i8  m_nItems;      // number of items in this row-slab

    i8  m_nMaxRowSize; // max size of row
    i8  m_nCols;       // number of items in this row-slab
    ColPix m_xaColPix[C_MAX_DEFCOLUMNS];
  
    WspConverge_t<C_WSP_ROWSLAB> m_xWsp; // workspace

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    RowSlab(StreamDef* pStreamDef);
   ~RowSlab(void) noexcept(false);
      
    i8    iCell(i8 i_Row, i8 i_Cell);
    ColPix* axaColPix(void);
    void  vDeposit(i8& i_Cell, i8 iVal);
    i8    nFreeSpace(void);
    bool  bNewRow(i8& i_Cell);
    i8*   aiaRowCells(i8 i_Row);
    i8    nRows(void);
    char* azString(i8& i_Cell);
    void  vTakeBytes(i8 nBytes);

  private:
    void vColHints(StreamDef* pStreamDef);
  
    Footer* aFooter(void);
  };
  
  RowSlab* _pRowSlab(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
