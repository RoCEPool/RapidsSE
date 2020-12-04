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

//#define _ROWBOX_BASED 1

namespace Rows
{
  class RowSlab :
                  public Object,

#ifdef _ROWBOX_BASED
                  public RowBox_t<C_WSP_ROWBUFFER>
#else
                  public WspConvergent_t<C_WSP_ROWBUFFER,Stz,Row>

#endif

  {
    friend class Row;

  private:

    char m_caModel[32]; // row slab model string
    i8   m_iSeVersion;  // se version number
    i8   m_iSeFormat;   // se format number
  
    StreamPix m_oStreamPix; // condensed stream def
  
    i8   m_i_RowCrs;    // current logical row number
    Row* m_aRowCur;     // current row
    i8   m_i_ColCrs;    // current working column
    i8   m_nRowStringBytes; // number string bytes for current row

    NmlStr m_oNmlStr;   // normalised string set
      
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    RowSlab(void);
   ~RowSlab(void) noexcept(false);
        
    bool  bAllotRow(Row** paRow);
    bool  bAllotRow(void);
    ColPix* axaColPix(void);
     i8    nCols(void);
    i8    iColumn(Row* aRow, i8 iColumn);
    void  vCopyPix(StreamPix* pStreamPixSrc);
    void  vCopyRow(Row* aRowSrc, bool bSrcHasPtnHash, bool bCopyPtnHash);
    void  vDiscardInvalidRow(void);
    void  vDumpAllRows(void);
    void  vDumpNst(void);
    void  vDumpRow(i8 i_Row);
    void  vDumpRow(i8 i_Row, Row* aRow);
    i8    nExtraCols(void);
    i8    nFreeSpace(void);
    void  vInitPix(StreamDef* pStreamDef, bool bPtnHash);
    NmlStr* pNmlStr(void);
    void  vNormaliseStrings(void);
    i8    iPtnOfRow(Row* aRow);
    void  vPositionOnCell(i8 i_Col);
    bool  bPtnHash(void);
    void  vReset(void);
    i8    nRowProperBytes(void);
    Row*  aRowAdr(i8 i_Row);
    Row*  aRowByOfs(i8 iOfs);
    void  vSetRow(i8 iRow);
    i8    iOfsOfRow(Row* aRow);
    i8    nRows(void);
    void  vSlabReport(void);
    StreamPix* aStreamPix(void);
    void  vZeroRow(Row* aRow);

  private:
    i8    i_RowSlabCell(i8 i_Row);
    m8*   amSlabCellVec(void);
  
    Footer* aFooter(void);
  
  public:
  
  /*--------------------------------------------------------------------------*/
  /* advance cursor                                                           */
  /*--------------------------------------------------------------------------*/
  inline void vAdvanceCursor(void)
  {
    m_i_ColCrs++;
  };


  /*--------------------------------------------------------------------------*/
  /*--------------------------------------------------------------------------*/
  
  /*--------------------------------------------------------------------------*/
  /* deposit an d8 value into next cell                                       */
  /*--------------------------------------------------------------------------*/
  inline void vDeposit(d8 dVal)
  {
    _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB);
  
    m_aRowCur->vColVal_flt(m_i_ColCrs,dVal);
    
    vAdvanceCursor();

    _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB);
  }

  /*--------------------------------------------------------------------------*/
  /* deposit an i8 value into next cell                                       */
  /*--------------------------------------------------------------------------*/
  inline void vDeposit(i8 iVal)
  {
    _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB);

    m_aRowCur->vColVal_int(m_i_ColCrs,iVal);

    vAdvanceCursor();
  
    _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB);
  }

  /*--------------------------------------------------------------------------*/
  /* deposit an stz value into next cell                                      */
  /*--------------------------------------------------------------------------*/
  inline void vDeposit(Stz* szVal)
  {
    _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB);

    char* azTgt = azDepositString_begin();
    
    i8 nLen = StrLib::nCopy(azTgt,szVal->azChar());

    vDepositString_end(nLen + 1);

    _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB);
  }

  /*--------------------------------------------------------------------------*/
  /* begin deposit of string                                                  */
  /*--------------------------------------------------------------------------*/
  inline char* azDepositString_begin(void)
  {
    _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB);

    char* azStz = (char*)ayLatterHead();
    
    m8* aColCell = m_aRowCur->aColCell(m_i_ColCrs);
    i8 i_Ofs = - nDifference(azStz,aColCell);

    m_aRowCur->vColVal_ofs(m_i_ColCrs,i_Ofs);

    vAdvanceCursor();

    _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB);

    return azStz;
  }

  /*--------------------------------------------------------------------------*/
  /* take bytes for deposited string                                          */
  /*--------------------------------------------------------------------------*/
  inline void vDepositString_end(i8 nSize)
  {
     _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB);

    vTakeHead(nSize);
    
    m_nRowStringBytes += nSize;
  
    _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB);
  }


  };
  
  RowSlab* _pRowSlab(void* aAddr);
}

/*==========================================================================*/
/* end                                                                      */
/*==========================================================================*/
