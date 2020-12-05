/*============================================================================*/
/* RowBox_t.hpp -- Row Container API                                          */
/*============================================================================*/
#pragma once

namespace Rows
{
  template <i8 SIZE>
  class RowBox_t : public WspConvergent_t<SIZE,Stz,Row>
  {
  protected:

    i8 m_nRowSizeProper;
  
    StreamPix m_oStreamPix;
  
    Row* m_aRowCur;     // current row
    i8   m_i_RowCrs;    // current logical row number
    i8   m_i_ColCrs;    // current working column
  
    i8   m_nRowStringBytes; // string size of current row (discard)


    char m_caModel[32]; // row slab model string
    i8   m_iSeVersion;  // se version number
    i8   m_iSeFormat;   // se format number

    NmlStr m_oNmlStr;   // normalised string set


  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);

    RowBox_t(void);
    RowBox_t(StreamDef* pStreamDef, bool bAddPtnHash);
   ~RowBox_t(void) noexcept(false);
  
  private:
    void vInitRowBox_t(void);

  
  public:
  
    void vResolveStreamPix(StreamDef* pStreamDef);
    
    Row* aAllotNewRow(void);
  
    i8   nColumns(void);
    ColPix* axaColPix(void);
    i8    iColumn(Row* aRow, i8 iColumn);
    void  vCopyPix(StreamPix* pStreamPixSrc);
    void  vCopyRow(Row* aRowSrc, bool bSrcHasPtnHash, bool bCopyPtnHash);

    void vColDeposit(i8 i_Col, i8 iVal);
    void vColDeposit(i8 i_Col, Stz* szVal);
    Dtm* aColDtm(i8 i_Col);
    void vColSetNull(i8 i_Col);
    d8   dColVal(i8 i_Col);
    i8   iColVal(i8 i_Col);
    Stz* szColVal(i8 i_Col);

    bool bIsColNull(i8 i_Col);

    Row* pRowAddress(i8 i_Row);
    i8   nRows(void);
    Row* aFirstRow(void);
    Row* aLastRow(void);
    Row* aNextRow(Row* aPriorRow);
  
//    void vPrintRows(StreamDef* pStreamDef);
//    void vShow(StreamDef* pStreamDef);
    
    i8   iToOffset(Stz* aItem);
  
  
    void vAdvanceCursor(void);
    void vDeposit(d8 dVal);
    void vDeposit(i8 iVal);
    void vDeposit(Stz* szVal);
    char* azDepositString_begin(void);
    void vDepositString_end(i8 nSize);

    //Footer* aFooter(void);


  };

}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
void* RowBox_t<SIZE>::operator new(size_t)
{
  vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
         "fatal: RowBox class cannot be new'd\n"_Stz);

  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
RowBox_t<SIZE>::RowBox_t(void)
{
  //_precondition_(_ADDRESS_);
  
  vInitRowBox_t();
  
  m_nRowSizeProper = 0;
  
   
  //_postcondition_(_ADDRESS_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
RowBox_t<SIZE>::RowBox_t(StreamDef* pStreamDef, bool bAddPtnHash)
{
  //_precondition_(_ADDRESS_);

  vInitRowBox_t();

  pStreamDef->vToStreamPix(&m_oStreamPix,bAddPtnHash);

  m_nRowSizeProper = Row::nRowProper(&m_oStreamPix);
  
  //_postcondition_(_ADDRESS_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
RowBox_t<SIZE>::~RowBox_t(void) noexcept(false)
{
  //_precondition_(_ADDRESS_);

}


/*----------------------------------------------------------------------------*/
/* initialise this row box                                                    */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
void RowBox_t<SIZE>::vInitRowBox_t(void)
{
  //_precondition_(_ADDRESS_,_MAGIC_);

  MemLib::vSet(m_caModel,' ',sizeof(m_caModel),CN_ROWSLAB);
  m_caModel[sizeof(m_caModel) - 1] = '\0';
  
  StrLib::vSprint(m_caModel,"RapidsSE %s SeFormat %s"_Stz,
                  RAPIDSSE_VERSION_STRING,
                  RAPIDSSE_FORMAT_STRING);
  
  m_iSeVersion = RAPIDSSE_VERSION_NUMBER;
  m_iSeFormat  = RAPIDSSE_FORMAT_NUMBER;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_aRowCur  = nullptr;
  m_i_RowCrs = C_UNKNOWN;
  m_i_ColCrs = C_UNKNOWN;

  m_nRowStringBytes = 0;
}


/*----------------------------------------------------------------------------*/
/* -- public ---------------------------------------------------------------- */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot a ref set in this buffer (may return nulltpr)                        */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
void RowBox_t<SIZE>::vResolveStreamPix(StreamDef* pStreamDef)
{
  //_precondition_(_ADDRESS_,_MAGIC_);

  pStreamDef->vToStreamPix(&m_oStreamPix,false);
  
  m_nRowSizeProper = Row::nRowProper(&m_oStreamPix);
}

/*----------------------------------------------------------------------------*/
/* allot a ref set in this buffer (may return nulltpr)                        */
/*----------------------------------------------------------------------------*/

//HACK(RowSlab,"quick fix to check for enough space ");

template <i8 SIZE>
Row* RowBox_t<SIZE>::aAllotNewRow(void)
{
  //_precondition_(_ADDRESS_,_MAGIC_);

  RASSERT(m_oStreamPix.m_bInitialised);
  
  m_nRowStringBytes = 0;
  m_i_ColCrs  = 0;

  if (WspConvergent_t<SIZE,Stz,Row>::nFreeBytes() < C_2KB)
    return nullptr;

  

  m_aRowCur = WspConvergent_t<SIZE,Stz,Row>::ayAllotTail(m_nRowSizeProper);

  if (! m_aRowCur)
    return nullptr;

  new(m_aRowCur) Row();

  m_i_RowCrs++;
 
  return m_aRowCur;
}

/*----------------------------------------------------------------------------*/
/* get the condensed column pix array                                         */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
ColPix* RowBox_t<SIZE>::axaColPix(void)
{
  return m_oStreamPix.m_xaColPix;
}


/*----------------------------------------------------------------------------*/
/* copy given stream pix into the stream pix in this rsb                      */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
void RowBox_t<SIZE>::vCopyPix(StreamPix* pStreamPixSrc)
{
  MemLib::vCopy(&m_oStreamPix,pStreamPixSrc,sizeof(StreamPix),CN_ROWSLAB);

  m_nRowSizeProper = Row::nRowProper(&m_oStreamPix);
}

/*----------------------------------------------------------------------------*/
/* copy given source row into current row of this row slab                    */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
void RowBox_t<SIZE>::vCopyRow(Row* aRowSrc, bool bSrcHasPtnHash, bool bCopyPtnHash)
{
  i8 iVal;
  Stz* szSrcStz;
  
  ASSERT(0 == m_i_ColCrs);
  
  i8 nCols = m_oStreamPix.m_nCols;
  
  i8 i_Col;
  
  for (i_Col = 0; i_Col < nCols; i_Col++) {
    
    eDM eDatum = (eDM)m_oStreamPix.m_xaColPix[i_Col].m_uDatum;
    
    if (DM_VCH == eDatum) {
       // vch datum

      if (C_DATUM_NULL == aRowSrc->iColVal_int(i_Col)) {
        vDeposit(C_DATUM_NULL);
      }
      else {
        szSrcStz = aRowSrc->szVal_rsb(i_Col);
        vDeposit(szSrcStz);
      }
    }
    else {
      // all others
     
      iVal = aRowSrc->iColVal_int(i_Col);
      vDeposit(iVal);
    }
  }

  if (bSrcHasPtnHash && bCopyPtnHash) {
    iVal = aRowSrc->iColVal_int(i_Col);
    vDeposit(iVal);
  }
  
//  Row* aRowDst = aRowAdr(nRows() - 1);
//  SP_RowPix spPix(nCols,m_oStreamPix.m_xaColPix);
//  vTrace("CopyRow: %s\n"_Stz,spPix.sz(aRowDst));
}

/*----------------------------------------------------------------------------*/
/* return column value from rsb for given column number                       */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
i8 RowBox_t<SIZE>::iColumn(Row* aRow, i8 i_Col)
{
  i8 iVal;
  Stz* szSrcStz;
    
  eDM eDatum = (eDM)m_oStreamPix.m_xaColPix[i_Col].m_uDatum;
    
  if (DM_VCH == eDatum) {
    // vch datum
    
    if (C_DATUM_NULL == aRow->iColVal_int(i_Col)) {
      return C_DATUM_NULL;
    }
    else {
      szSrcStz = aRow->szVal_rsb(i_Col);
      return (i8)szSrcStz;
    }
  }
  else {
    // all others
    
    iVal = aRow->iColVal_int(i_Col);
    return iVal;
  }
}






/*----------------------------------------------------------------------------*/
/* get number of columns                                                      */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
i8 RowBox_t<SIZE>::nColumns(void)
{
  return m_oStreamPix.m_nCols;
}

/*----------------------------------------------------------------------------*/
/* get datum value of given column                                            */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
void RowBox_t<SIZE>::vColDeposit(i8 i_Col, i8 iValue)
{
  m_aRowCur->vColVal_int(i_Col,iValue);
}

/*----------------------------------------------------------------------------*/
/* get datum value of given column                                            */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
void RowBox_t<SIZE>::vColDeposit(i8 i_Col, Stz* szVal)
{
  Stz* szDeposited = WspConvergent_t<SIZE,Stz,Row>::szStzDeposit(szVal);
  
  i8 i_Ofs = - nDifference(m_aRowCur->aColCell(i_Col),szDeposited);
  
  m_aRowCur->vColVal_ofs(i_Col,i_Ofs);
}

/*----------------------------------------------------------------------------*/
/* get datum value of given column                                            */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
Dtm* RowBox_t<SIZE>::aColDtm(i8 i_Col)
{
  Dtm* aDtm = m_aRowCur->aColDtm(i_Col);
  
  return aDtm;
}

/*----------------------------------------------------------------------------*/
/* set column value to be dull                                                */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
void RowBox_t<SIZE>::vColSetNull(i8 i_Col)
{
  m_aRowCur->vSetColNull(i_Col);
}

/*----------------------------------------------------------------------------*/
/* get float value of given column                                            */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
d8 RowBox_t<SIZE>::dColVal(i8 i_Col)
{
  d8 dVal = m_aRowCur->dColVal_flt(i_Col);
  
  return dVal;
}

/*----------------------------------------------------------------------------*/
/* get integer value of given column                                          */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
i8 RowBox_t<SIZE>::iColVal(i8 i_Col)
{
  i8 iVal = m_aRowCur->iColVal_int(i_Col);
  
  return iVal;
}

/*----------------------------------------------------------------------------*/
/* get stz value of given column                                              */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
Stz* RowBox_t<SIZE>::szColVal(i8 i_Col)
{
  Stz* szVal = m_aRowCur->szColVal_ofs(i_Col);
  
  return szVal;
}



/*----------------------------------------------------------------------------*/
/* is given column a null?                                                    */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
bool RowBox_t<SIZE>::bIsColNull(i8 i_Col)
{
  bool bIsNull = m_aRowCur->bIsColNull(i_Col);
  
  return bIsNull;
}



/*----------------------------------------------------------------------------*/
/* get address of an rsb                                                      */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
Row* RowBox_t<SIZE>::pRowAddress(i8 i_Row)
{
  /*          v row[1]            v row[0]
       -------*----+----+----*----+----+---+----+----*
              | Ob : c0 : c1 : cN | Ob :c0 : c1 : cN |!!!hazard
       -------*----+----+----*----+----+---+----+----*
   */
    
  Row* pRowAddr = WspConvergent_t<SIZE,Stz,Row>::aTailItem(i_Row);
  
  Row* pRow = _pRow(pRowAddr);
  
  return pRow;
}

/*----------------------------------------------------------------------------*/
/* get number of rows                                                         */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
i8 RowBox_t<SIZE>::nRows(void)
{
  return WspConvergent_t<SIZE,Stz,Row>::nItems();
}



/*----------------------------------------------------------------------------*/
/* return first ref set in this buffer                                        */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
Row* RowBox_t<SIZE>::aFirstRow(void)
{
  //_precondition_(_ADDRESS_,_MAGIC_);
  
  RASSERT(m_oStreamPix.m_bInitialised);
  
  Row* aRow = WspConvergent_t<SIZE,Stz,Row>::aFirstItem();
  
  return aRow;
}

/*----------------------------------------------------------------------------*/
/* return last ref set in this buffer                                         */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
Row* RowBox_t<SIZE>::aLastRow(void)
{
  //_precondition_(_ADDRESS_,_MAGIC_);
  
  RASSERT(m_oStreamPix.m_bInitialised);

  Row* aRow = WspConvergent_t<SIZE,Stz,Row>::aFirstItem();
  Row* aLastRow = nullptr;
  
  while (aRow) {
    aLastRow = aRow;
    aRow = aNextRow(aRow);
  };
  
  return aLastRow;
}

/*----------------------------------------------------------------------------*/
/* return next ref set in this buffer                                         */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
Row* RowBox_t<SIZE>::aNextRow(Row* aPriorRow)
{
  //_precondition_(_ADDRESS_,_MAGIC_);

  RASSERT(m_oStreamPix.m_bInitialised);

  Row* aRow = WspConvergent_t<SIZE,Stz,Row>::aAdjacentItem(aPriorRow, m_nRowSizeProper);
  
  return aRow;
}








/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/* advance cursor                                                           */
/*--------------------------------------------------------------------------*/
template <i8 SIZE>
void RowBox_t<SIZE>::vAdvanceCursor(void)
{
  m_i_ColCrs++;
}


/*--------------------------------------------------------------------------*/
/* deposit an d8 value into next cell                                       */
/*--------------------------------------------------------------------------*/
template <i8 SIZE>
void RowBox_t<SIZE>::vDeposit(d8 dVal)
{

  m_aRowCur->vColVal_flt(m_i_ColCrs,dVal);
  
  vAdvanceCursor();

}

/*--------------------------------------------------------------------------*/
/* deposit an i8 value into next cell                                       */
/*--------------------------------------------------------------------------*/
template <i8 SIZE>
void RowBox_t<SIZE>::vDeposit(i8 iVal)
{

  m_aRowCur->vColVal_int(m_i_ColCrs,iVal);

  vAdvanceCursor();

}

/*--------------------------------------------------------------------------*/
/* deposit an stz value into next cell                                      */
/*--------------------------------------------------------------------------*/
template <i8 SIZE>
void RowBox_t<SIZE>::vDeposit(Stz* szVal)
{

  char* azTgt = azDepositString_begin();
  
  i8 nLen = StrLib::nCopy(azTgt,szVal->azChar());

  vDepositString_end(nLen + 1);

}

/*--------------------------------------------------------------------------*/
/* begin deposit of string                                                  */
/*--------------------------------------------------------------------------*/
template <i8 SIZE>
char* RowBox_t<SIZE>::azDepositString_begin(void)
{

  char* azStz = (char*)WspConvergent_t<SIZE,Stz,Row>::ayLatterHead();
  
  m8* aColCell = m_aRowCur->aColCell(m_i_ColCrs);
  i8 i_Ofs = - nDifference(azStz,aColCell);

  m_aRowCur->vColVal_ofs(m_i_ColCrs,i_Ofs);

  vAdvanceCursor();


  return azStz;
}

/*--------------------------------------------------------------------------*/
/* take bytes for deposited string                                          */
/*--------------------------------------------------------------------------*/
template <i8 SIZE>
void RowBox_t<SIZE>::vDepositString_end(i8 nSize)
{

  WspConvergent_t<SIZE,Stz,Row>::vTakeHead(nSize);
  
  m_nRowStringBytes += nSize;


}




/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
//template <i8 SIZE>
//Footer* RowBox_t<SIZE>::aFooter(void)
//{
//  return &m_xFooter;
//}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
