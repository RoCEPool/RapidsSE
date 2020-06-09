/*============================================================================*/
/* RowSlab.hpp -- Row Slab                                                    */
/*============================================================================*/
#pragma once

namespace Rows
{
  static const i8 C_WSP_ROWSLAB    = C_SIZE_LARGE_OBJECT - C_16KB;
  static const i8 C_SLAB_CELLS     = C_WSP_ROWSLAB / C_BYTES_PER_WORD;
  static const i8 C_FIRST_LGL_CELL = C_SLAB_CELLS - 1;

  static const i8 C_NML_STR_SPACE  = C_8KB;

  class RowSlab : public Object
  {
  public:
  
    char m_caModel[32]; // model string
    i8   m_iVersion;    //
    i8   m_iFormat;
    i8   m_nBytes;      // size in bytes of this row slab
  
    i8   m_nTrimmed;    // how many bytes trimmed  NOT USED YET
    i8   m_i_Trim;      // where trim occurred     NOT USED YET

    i8   m_nItems;      // number of items in this row-slab
    i8   m_i_Cell;      // current working cell
  
    StreamPix m_oStreamPix; // condensed stream def
  
    NmlStr m_oNmlStr;   // normalised string set
    
    WspConverge_t<C_WSP_ROWSLAB> m_xWsp; // workspace
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    RowSlab(void);
   ~RowSlab(void) noexcept(false);
      
    ColPix* axaColPix(void);
    i8    nCols(void);
    void  vCopyPix(StreamPix* pStreamPixSrc);
    void  vCopyRsb(Rsb* aRsbSrc, bool bSrcHasPtnHash, bool bCopyPtnHash);
    void  vDeposit(d8 dVal);
    void  vDeposit(i8 iVal);
    void  vDeposit(Stz* szVal);
    char* azDepositString_begin(void);
    void  vDepositString_end(i8 nSize);
    void  vDumpAllRows(void);
    void  vDumpNst(void);
    void  vDumpRow(i8 i_Row);
    i8    nFreeSpace(void);
    void  vInitPix(StreamDef* pStreamDef, bool bPtnHash);
    bool  bNewRow(void);
    NmlStr* pNmlStr(void);
    void  vNormaliseStrings(void);
    bool  bPtnHash(void);
    i8    iPtnOfRsb(Rsb* aRsb);
    Rsb*  aRsbAdr(i8 i_Row);
    i8    nRows(void);
    void  vSlabReport(void);
    StreamPix* aStreamPix(void);

  private:
    m8*     amSlabCellVec(void);
    Footer* aFooter(void);
  };
  
  RowSlab* _pRowSlab(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
