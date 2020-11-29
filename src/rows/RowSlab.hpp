/*============================================================================*/
/* RowSlab.hpp -- Row Slab                                                    */
/*============================================================================*/
#pragma once

namespace Rows
{
  static const i8 C_ROWSLAB_WSP    = C_SIZE_LARGE_OBJECT - C_16KB;
  static const i8 C_ROWSOLO_WSP    = C_MAX_RECORD;
  static const i8 C_SLAB_CELLS     = C_ROWSLAB_WSP / C_BYTES_PER_WORD;
  static const i8 C_FIRST_LGL_CELL = C_SLAB_CELLS - 1;

  static const i8 C_CELL_BOUNDARY  = C_SLAB_CELLS;

  static const i8 C_NML_STR_SPACE  = C_8KB;




  template<int WSP_SIZE>
  class RowWsp : public Object
  {
  protected:
  
    i8 m_nItems;  // number of items in this row wsp

    SingleWord m__PadToCacheLine[5]; // padding to cache line

    WspConverge_t<WSP_SIZE> m_xWsp; // workspace of row(s)

  public:
    RowWsp(eCN eClass);
  
    // must be last
    Footer m_xFooter;
  };

}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template<int WSP_SIZE>
RowWsp<WSP_SIZE>::RowWsp(eCN eClass)
: Object(eClass)
{
  _precondition_(_ADDRESS_,_MAGIC_,eClass,_FOOTER_);

  m_nItems = 0;

  _postcondition_(_ADDRESS_,_MAGIC_,eClass,_FOOTER_);
}


// /////////////////////////////////////////////////////////////////////////////


namespace Rows
{
  class RowSlab : public RowWsp<C_ROWSLAB_WSP>
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
    m8*  m_amCellCrs;   // current working cell
    i8   m_nRowStringBytes; // number string bytes for current row

    NmlStr m_oNmlStr;   // normalised string set
      
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
    i8    iColumn(Row* aRow, i8 iColumn);
    void  vCopyPix(StreamPix* pStreamPixSrc);
    void  vCopyRsb(Row* aRowSrc, bool bSrcHasPtnHash, bool bCopyPtnHash);
    void  vDiscardInvalidRow(void);
    void  vDumpAllRows(void);
    void  vDumpNst(void);
    void  vDumpRow(i8 i_Row);
    void  vDumpRow(i8 i_Row, Row* aRow);
    i8    nExtraCols(void);
    i8    nFreeSpace(void);
    void  vInitPix(StreamDef* pStreamDef, bool bPtnHash);
    bool  bNewRow(Row** paRsb);
    bool  bNewRow(void);
    NmlStr* pNmlStr(void);
    void  vNormaliseStrings(void);
    i8    iPtnOfRsb(Row* aRow);
    void  vPositionOnCell(i8 i_Col);
    bool  bPtnHash(void);
    void  vReset(void);
    i8    nRowProperBytes(void);
    Row*  aRowAdr(i8 i_Row);
    Row*  aRowByOfs(i8 iOfs);
    void  vSetRow(i8 iRow);
    i8    iOfsOfRsb(Row* aRow);
    i8    nRows(void);
    void  vSlabReport(void);
    StreamPix* aStreamPix(void);
    void  vZeroRsb(Row* aRow);

  private:
    m8*     amSlabCellVec(void);
    Footer* aFooter(void);
  
  public:
  
  /*--------------------------------------------------------------------------*/
  /* advance cursor                                                           */
  /*--------------------------------------------------------------------------*/
  inline void vAdvanceCursor(void)
  {
    m_amCellCrs++;
    m_i_ColCrs++;
  };


  /*--------------------------------------------------------------------------*/
  /*--------------------------------------------------------------------------*/
  
  /*--------------------------------------------------------------------------*/
  /* deposit an d8 value into next cell                                       */
  /*--------------------------------------------------------------------------*/
  inline void vDeposit(d8 dVal)
  {
    _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  
    //*(d8*)m_amCellCrs = dVal;
    m_aRowCur->vColVal_flt(m_i_ColCrs,dVal);
    
    vAdvanceCursor();

    _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  }

  /*--------------------------------------------------------------------------*/
  /* deposit an i8 value into next cell                                       */
  /*--------------------------------------------------------------------------*/
  inline void vDeposit(i8 iVal)
  {
    _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

    //*(i8*)m_amCellCrs = iVal;
    m_aRowCur->vColVal_int(m_i_ColCrs,iVal);

    vAdvanceCursor();
  
    _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  }

  /*--------------------------------------------------------------------------*/
  /* deposit an stz value into next cell                                      */
  /*--------------------------------------------------------------------------*/
  inline void vDeposit(Stz* szVal)
  {
    _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

    char* azTgt = azDepositString_begin();
    
    i8 nLen = StrLib::nCopy(azTgt,szVal->azChar());

    vDepositString_end(nLen + 1);

    _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  }

  /*--------------------------------------------------------------------------*/
  /* begin deposit of string                                                  */
  /*--------------------------------------------------------------------------*/
  inline char* azDepositString_begin(void)
  {
    _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

    char* azStz = (char*)m_xWsp.ayLatterHead();
    
    //i8 i_Ofs = - nDifference(azStz,m_amCellCrs);
    m8* aColCell = m_aRowCur->aColCell(m_i_ColCrs);
    i8 i_Ofs = - nDifference(azStz,aColCell);

    //*(i8*)m_amCellCrs = i_Ofs;
    m_aRowCur->vColVal_ofs(m_i_ColCrs,i_Ofs);

    vAdvanceCursor();

    _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

    return azStz;
  }

  /*--------------------------------------------------------------------------*/
  /* take bytes for deposited string                                          */
  /*--------------------------------------------------------------------------*/
  inline void vDepositString_end(i8 nSize)
  {
     _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);

    m_xWsp.vTakeHead(nSize);
    
    m_nRowStringBytes += nSize;
  
    _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWSLAB,_FOOTER_);
  }


  };
  
  RowSlab* _pRowSlab(void* aAddr);
}

/*==========================================================================*/
/* end                                                                      */
/*==========================================================================*/
