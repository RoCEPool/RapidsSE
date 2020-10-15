/*============================================================================*/
/* RowSlab.hpp -- Row Slab                                                    */
/*============================================================================*/
#pragma once

namespace Rows
{
  static const i8 C_WSP_ROWSLAB    = C_SIZE_LARGE_OBJECT - C_16KB;
  static const i8 C_SLAB_CELLS     = C_WSP_ROWSLAB / C_BYTES_PER_WORD;
  static const i8 C_FIRST_LGL_CELL = C_SLAB_CELLS - 1;

  static const i8 C_CELL_BOUNDARY  = C_SLAB_CELLS;

  static const i8 C_NML_STR_SPACE  = C_8KB;

  class RowSlab : public Object
  {
    friend class Rsb;

  private:

    char m_caModel[32]; // model string
    i8   m_iVersion;    //
    i8   m_iFormat;
    i8   m_nBytes;      // size in bytes of this row slab
  
    i8   m_nTrimmed;    // how many bytes trimmed  NOT USED YET
    i8   m_i_Trim;      // where trim occurred     NOT USED YET

    i8   m_nItems;      // number of items in this row-slab
    i8   m_i_RowLgl;    // current logical row number
  
    m8*  m_amCellCrs;   // current working cell

    StreamPix m_oStreamPix; // condensed stream def
 
    SingleWord m__PadToCacheLine[7]; // padding to cache line

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
    i8    i_Cell0(i8 i_Row);
    m8*   amCell0(i8 i_Row);
    i8    nCols(void);
    i8    iColumn(Rsb* aRsb, i8 iColumn);
    void  vCopyPix(StreamPix* pStreamPixSrc);
    void  vCopyRsb(Rsb* aRsbSrc, bool bSrcHasPtnHash, bool bCopyPtnHash);
    void  vDiscardInvalidRow(void);
    void  vDumpAllRows(void);
    void  vDumpNst(void);
    void  vDumpRow(i8 i_Row);
    void  vDumpRow(i8 i_Row, Rsb* aRsb);
    i8    nExtraCols(void);
    i8    nFreeSpace(void);
    void  vInitPix(StreamDef* pStreamDef, bool bPtnHash);
    bool  bNewRow(Rsb** paRsb);
    bool  bNewRow(void);
    NmlStr* pNmlStr(void);
    void  vNormaliseStrings(void);
    i8    iPtnOfRsb(Rsb* aRsb);
    void  vPositionOnCell(i8 i_Col);
    bool  bPtnHash(void);
    void  vReset(void);
    Rsb*  aRsbAdr(i8 i_Row);
    Rsb*  aRsbByOfs(i8 iOfs);
    i8    iOfsOfRsb(Rsb* aRsb);
    i8    nRows(void);
    void  vSlabReport(void);
    StreamPix* aStreamPix(void);
    void  vZeroRsb(Rsb* aRsb);

  private:
    m8*     amSlabCellVec(void);
    Footer* aFooter(void);
  
  public:
  
  /*----------------------------------------------------------------------------*/
  /* advance cursor                                                             */
  /*----------------------------------------------------------------------------*/
  inline void vAdvanceCursor(void)
  {
    m_amCellCrs++;
  };

  /*----------------------------------------------------------------------------*/
  /* get address of current cell                                                */
  /*----------------------------------------------------------------------------*/
  inline m8* aCurrentCell(void)
  {

    return m_amCellCrs;
  };


  /*----------------------------------------------------------------------------*/
  /*----------------------------------------------------------------------------*/
  
  /*----------------------------------------------------------------------------*/
  /* deposit an d8 value into next cell                                         */
  /*----------------------------------------------------------------------------*/
  inline void vDeposit(d8 dVal)
  {
    _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  
    *(d8*)m_amCellCrs = dVal;
    
    vAdvanceCursor();

    _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  }

  /*----------------------------------------------------------------------------*/
  /* deposit an i8 value into next cell                                         */
  /*----------------------------------------------------------------------------*/
  inline void vDeposit(i8 iVal)
  {
    _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

    *(i8*)m_amCellCrs = iVal;
  
    vAdvanceCursor();
  
    _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  }

  /*----------------------------------------------------------------------------*/
  /* deposit an stz value into next cell                                        */
  /*----------------------------------------------------------------------------*/
  inline void vDeposit(Stz* szVal)
  {
    _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

    char* azTgt = azDepositString_begin();
    
    i8 nLen = StrLib::nCopy(azTgt,szVal->azChar());

    vDepositString_end(nLen + 1);

    _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  }

  /*----------------------------------------------------------------------------*/
  /* begin deposit of string                                                    */
  /*----------------------------------------------------------------------------*/
  inline char* azDepositString_begin(void)
  {
    _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

    char* azStz = (char*)m_xWsp.ayLatterHead();
    
    i8 i_Ofs = - nDifference(azStz,m_amCellCrs);
    *(i8*)m_amCellCrs = i_Ofs;
  
    vAdvanceCursor();

    _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

    return azStz;
  }

  /*----------------------------------------------------------------------------*/
  /* take bytes for deposited string                                            */
  /*----------------------------------------------------------------------------*/
  inline void vDepositString_end(i8 nSize)
  {
     _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

    m_xWsp.vTakeHead(nSize);
    
    _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  }


  };
  
  RowSlab* _pRowSlab(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
