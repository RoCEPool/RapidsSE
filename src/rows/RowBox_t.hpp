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
  
    RowPix m_oRowPix;
  
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
  
    void vResolveRowPix(StreamDef* pStreamDef);
    
    Row* aAllotNewRow(void);
  
    i8   nColumns(void);
    ColPix* axaColPix(void);
    i8    iColumn(Row* aRow, i8 iColumn);
    void  vCopyRowPix(RowPix* pRowPixSrc);
    void  vCopyRow(Row* aRowSrc, bool bSrcHasPtnHash, bool bCopyPtnHash);

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
    void vDepositColVal(d8 dColVal);
    void vDepositColVal(i8 i_Col, i8 iColVal);
    void vDepositColVal(i8 iColVal);
    void vDepositColVal(Stz* szColVal);
    void vDepositColVal(i8 i_Col, Stz* szColVal);
    char* azDepositString_begin(void);
    void vDepositString_end(i8 nSize);

    void  vDiscardInvalidRow(void);
    void  vDumpAllRows(void);
    void  vDumpNst(void);
    void  vDumpRow(i8 i_Row);
    void  vDumpRow(i8 i_Row, Row* aRow);

    //Footer* aFooter(void);
    i8    nExtraCols(void);
    void  vInitPix(StreamDef* pStreamDef, bool bPtnHash);
    NmlStr* pNmlStr(void);
    void  vNormaliseStrings(void);
    i8    iPtnOfRow(Row* aRow);
    void  vPositionOnCell(i8 i_Col);
    bool  bPtnHash(void);
    void  vReset(void);
    void  vResetRow(void);
    i8    nRowProperBytes(void);
    Row*  pRowCurrent(void);
    Row*  aRowByOfs(i8 iOfs);
    Row*  aSetCurRow(i8 iRow);
    i8    iOffsetInWsp(Row* aRow);
    void  vSlabReport(void);
    RowPix* aRowPix(void);
    void  vZeroRow(Row* aRow);

  private:
    i8    i_RowBufferCell(i8 i_Row);
    m8*   amSlabCellVec(void);


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

  pStreamDef->vToRowPix(&m_oRowPix,bAddPtnHash);

  m_nRowSizeProper = Row::nRowProper(&m_oRowPix);
  
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

  MemLib::vSet(m_caModel,' ',sizeof(m_caModel),CN_ROWBUFFER);
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
void RowBox_t<SIZE>::vResolveRowPix(StreamDef* pStreamDef)
{
  //_precondition_(_ADDRESS_,_MAGIC_);

  pStreamDef->vToRowPix(&m_oRowPix,false);
  
  m_nRowSizeProper = Row::nRowProper(&m_oRowPix);
}

/*----------------------------------------------------------------------------*/
/* allot a ref set in this buffer (may return nulltpr)                        */
/*----------------------------------------------------------------------------*/

//HACK(RowBuffer,"quick fix to check for enough space ");

template <i8 SIZE>
Row* RowBox_t<SIZE>::aAllotNewRow(void)
{
  //_precondition_(_ADDRESS_,_MAGIC_);

  RASSERT(m_oRowPix.m_bInitialised);
  
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
  return m_oRowPix.m_xaColPix;
}


/*----------------------------------------------------------------------------*/
/* copy given stream pix into the stream pix in this row                      */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
void RowBox_t<SIZE>::vCopyRowPix(RowPix* pRowPixSrc)
{
  MemLib::vCopy(&m_oRowPix,pRowPixSrc,sizeof(RowPix),CN_ROWBUFFER);

  m_nRowSizeProper = Row::nRowProper(&m_oRowPix);
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
  
  i8 nCols = m_oRowPix.m_nCols;
  
  i8 i_Col;
  
  for (i_Col = 0; i_Col < nCols; i_Col++) {
    
    eDM eDatum = (eDM)m_oRowPix.m_xaColPix[i_Col].m_uDatum;
    
    if (DM_VCH == eDatum) {
       // vch datum

      if (C_DATUM_NULL == aRowSrc->iColVal_int(i_Col)) {
        vDepositColVal(C_DATUM_NULL);
      }
      else {
        szSrcStz = aRowSrc->szColVal_ofs(i_Col);
        vDepositColVal(szSrcStz);
      }
    }
    else {
      // all others
     
      iVal = aRowSrc->iColVal_int(i_Col);
      vDepositColVal(iVal);
    }
  }

  if (bSrcHasPtnHash && bCopyPtnHash) {
    iVal = aRowSrc->iColVal_int(i_Col);
    vDepositColVal(iVal);
  }
  
//  Row* aRowDst = aRowAdr(nRows() - 1);
//  SP_RowPix spPix(nCols,m_oRowPix.m_xaColPix);
//  vTrace("CopyRow: %s\n"_Stz,spPix.sz(aRowDst));
}

/*----------------------------------------------------------------------------*/
/* return column value from row for given column number                       */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
i8 RowBox_t<SIZE>::iColumn(Row* aRow, i8 i_Col)
{
  i8 iVal;
  Stz* szSrcStz;
    
  eDM eDatum = (eDM)m_oRowPix.m_xaColPix[i_Col].m_uDatum;
    
  if (DM_VCH == eDatum) {
    // vch datum
    
    if (C_DATUM_NULL == aRow->iColVal_int(i_Col)) {
      return C_DATUM_NULL;
    }
    else {
      szSrcStz = aRow->szColVal_ofs(i_Col);
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
  return m_oRowPix.m_nCols;
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
/* get address of an row                                                      */
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
/* get current row                                                            */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
Row* RowBox_t<SIZE>::pRowCurrent(void)
{
  return m_aRowCur;
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
  
  RASSERT(m_oRowPix.m_bInitialised);
  
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
  
  RASSERT(m_oRowPix.m_bInitialised);

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

  RASSERT(m_oRowPix.m_bInitialised);

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
void RowBox_t<SIZE>::vDepositColVal(d8 dColVal)
{

  m_aRowCur->vColVal_flt(m_i_ColCrs,dColVal);
  
  vAdvanceCursor();

}

/*--------------------------------------------------------------------------*/
/* deposit an i8 value into next cell                                       */
/*--------------------------------------------------------------------------*/
template <i8 SIZE>
void RowBox_t<SIZE>::vDepositColVal(i8 iColVal)
{

  m_aRowCur->vColVal_int(m_i_ColCrs,iColVal);

  vAdvanceCursor();
}

/*----------------------------------------------------------------------------*/
/* get datum value of given column                                            */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
void RowBox_t<SIZE>::vDepositColVal(i8 i_Col, i8 iColVal)
{
  m_aRowCur->vColVal_int(i_Col,iColVal);
}

/*--------------------------------------------------------------------------*/
/* deposit an stz value into next cell                                      */
/*--------------------------------------------------------------------------*/
template <i8 SIZE>
void RowBox_t<SIZE>::vDepositColVal(Stz* szColVal)
{

  char* azTgt = azDepositString_begin();
  
  i8 nLen = StrLib::nCopy(azTgt,szColVal->azChar());

  vDepositString_end(nLen + 1);
}

/*----------------------------------------------------------------------------*/
/* get datum value of given column                                            */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
void RowBox_t<SIZE>::vDepositColVal(i8 i_Col, Stz* szColVal)
{
  Stz* szDeposited = WspConvergent_t<SIZE,Stz,Row>::szStzDeposit(szColVal);
  
  i8 i_Ofs = - nDifference(m_aRowCur->aColCell(i_Col),szDeposited);
  
  m_aRowCur->vColVal_ofs(i_Col,i_Ofs);
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
/* discard current invalid/incomplete row                                     */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
void RowBox_t<SIZE>::vDiscardInvalidRow(void)
{
  
  // undo the row number increments of vNewRow()
  m_i_RowCrs--;
  
  // return tail bytes allotted for cells of current row in vNewRow()
  i8 nCellBytes = nRowProperBytes();
  WspConvergent_t<SIZE,Stz,Row>::vGiveTail(nCellBytes);
  
  WspConvergent_t<SIZE,Stz,Row>::vDecItems();
  
  // return head bytes allotted for strings deposited in vDepositString_end()
  if (m_nRowStringBytes) {
    WspConvergent_t<SIZE,Stz,Row>::vGiveHead(m_nRowStringBytes);
    m_nRowStringBytes = 0;
  }
}

/*----------------------------------------------------------------------------*/
/* dump given row                                                             */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
void RowBox_t<SIZE>::vDumpAllRows(void)
{
  
  i8 nRows = this->nRows();
  vTrace("RowBuffer of %lld rows...\n"_Stz,nRows);
  for (i8 i_Row = 0; i_Row < nRows; i_Row++)
    vDumpRow(i_Row);
}

/*----------------------------------------------------------------------------*/
/* dump normalised string table                                               */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
void RowBox_t<SIZE>::vDumpNst(void)
{

  m_oNmlStr.vDump();
}

/*----------------------------------------------------------------------------*/
/* dump given row                                                             */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
void RowBox_t<SIZE>::vDumpRow(i8 i_Row)
{
  
  Row* aRow = this->pRowAddress(i_Row);
  vDumpRow(i_Row,aRow);
}

/*----------------------------------------------------------------------------*/
/* dump given row                                                             */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
void RowBox_t<SIZE>::vDumpRow(i8 i_Row, Row* aRow)
{
  
  SP_RowPix spRowPix(this->nColumns(),this->axaColPix());
  
  Stz* szRow = spRowPix.sz(aRow);
  vTrace("[%lld] %s\n"_Stz,i_Row,szRow);
}

/*----------------------------------------------------------------------------*/
/* get number of extra columns                                                */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
i8 RowBox_t<SIZE>::nExtraCols(void)
{
  
  return m_oRowPix.m_nExtraCols;
}



/*----------------------------------------------------------------------------*/
/* initilize col pix                                                          */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
void RowBox_t<SIZE>::vInitPix(StreamDef* pStreamDef, bool bPtnHash)
{
        
  pStreamDef->vToRowPix(&m_oRowPix,bPtnHash);
  
  m_nRowSizeProper = Row::nRowProper(&m_oRowPix);
}

/*----------------------------------------------------------------------------*/
/* get normalised string set                                                  */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
NmlStr* RowBox_t<SIZE>::pNmlStr(void)
{

  return & m_oNmlStr;
}

/*----------------------------------------------------------------------------*/
/* normalize repeating strings                                                */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
void RowBox_t<SIZE>::vNormaliseStrings(void)
{

  //m_oNmlStr.vDump();

  i8 nTimes = 0;
  i8 nRows  = this->nRows();
  i8 nCols  = this->nColumns();
  
  if (nCols < 14)
    return;
  
  if (nRows < 250)
    return;
  
  //vTrace("NormaliseStrings %lld rows\n"_Stz,nRows);
  
//for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
  for (i8 i_Col = 13; i_Col < 14; i_Col++) {

    eDM eDatum = (eDM)m_oRowPix.m_xaColPix[i_Col].m_uDatum;
    
    if (DM_VCH != eDatum)
      continue;
    

    for (i8 i_OuterRow = 0; i_OuterRow < 100; i_OuterRow++) {
      
      Row* aOuterRow = pRowAddress(i_OuterRow);
      
      if (C_DATUM_NULL == aOuterRow->iColVal_int(i_Col))
        continue;

      Stz* szOuterStz = aOuterRow->szColVal_ofs(i_Col);
      //vTrace("Outer: %s\n"_Stz,szOuterStz);
      
      i8 nLength = szOuterStz->nLength();
      if (nLength < 8)
        continue;
      
      for (i8 i_InnerRow = i_OuterRow + 1;
           i_InnerRow < i_OuterRow + 99;
           i_InnerRow++) {

        //vTrace("NormaliseStrings outer %lld inner %lld\n"_Stz,i_OuterRow,i_InnerRow);
        
        Row* aInnerRow = pRowAddress(i_InnerRow);

        if (C_DATUM_NULL == aInnerRow->iColVal_int(i_Col))
          continue;

        Stz* szInnerStz = aInnerRow->szColVal_ofs(i_Col);
        //vTrace("Inner: %s\n"_Stz,szInnerStz);

        if (! szInnerStz->bEqual(szOuterStz))
          continue;

        //vTrace("Testing : %lld - %lld : %s\n"_Stz,i_OuterRow,i_InnerRow,szOuterStz);
        if (! m_oNmlStr.szFind(szOuterStz)) {
          m_oNmlStr.szAdd(szOuterStz);
          if (++nTimes >= 3)
            goto L_Exit;
        }
        
        break;
        
      } // each inner row
      
    } // each outer row

  } // each col
  
L_Exit:
  ; //m_oNmlStr.vDump();
}

/*----------------------------------------------------------------------------*/
/* get the offset of row                                                      */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
i8 RowBox_t<SIZE>::iOffsetInWsp(Row* aRow)
{
  
  i8 iOfs = WspConvergent_t<C_WSP_ROWBUFFER,Stz,Row>::iOffsetInWsp((byte*)aRow);
  
  return iOfs;
}

/*----------------------------------------------------------------------------*/
/* position on given cell in current row                                      */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
void RowBox_t<SIZE>::vPositionOnCell(i8 i_Col)
{
  
  m_i_ColCrs = i_Col;
}

/*----------------------------------------------------------------------------*/
/* is there a ptn hash?                                                       */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
bool RowBox_t<SIZE>::bPtnHash(void)
{

  return m_oRowPix.m_bPtnHash;
}

/*----------------------------------------------------------------------------*/
/* get the partition of row                                                   */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
i8 RowBox_t<SIZE>::iPtnOfRow(Row* aRow)
{

  i8 i_HshCol = m_oRowPix.m_nCols - 1;
    
  i8 i_Ptn = aRow->iColVal_int(i_HshCol);
  
  return i_Ptn;
}

/*----------------------------------------------------------------------------*/
/* reset this rowslab                                                         */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
void RowBox_t<SIZE>::vReset(void)
{
  
  m_i_RowCrs  = C_UNKNOWN;
  m_aRowCur   = nullptr;
  m_i_ColCrs  = C_UNKNOWN;

  WspConvergent_t<SIZE,Stz,Row>::vReset();
}

/*----------------------------------------------------------------------------*/
/* reset this row                                                             */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
void RowBox_t<SIZE>::vResetRow(void)
{
  WspConvergent_t<SIZE,Stz,Row>::vReset();
}

/*----------------------------------------------------------------------------*/
/* get bytes needed to hold row cells proper (i.e. without strings)           */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
i8 RowBox_t<SIZE>::nRowProperBytes(void)
{
  i8 nBytes = (m_oRowPix.m_nCols + 1 /* dummy */ ) * C_BYTES_PER_WORD;
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get address of an row by its offset                                        */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
Row* RowBox_t<SIZE>::aRowByOfs(i8 i_Ofs)
{
  
  Row* aRow = aAdd<Row*>(WspConvergent_t<SIZE,Stz,Row>::ayArea(),i_Ofs);
  
  return aRow;
}

/*----------------------------------------------------------------------------*/
/* get address of an row by its offset and set as current                     */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
Row* RowBox_t<SIZE>::aSetCurRow(i8 i_Row)
{
  
  m_i_RowCrs = i_Row;
  m_aRowCur = pRowAddress(i_Row);
  return m_aRowCur;
}

/*----------------------------------------------------------------------------*/
/* dump slap and report                                                       */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
void RowBox_t<SIZE>::vSlabReport(void)
{
 
  s32 sSlab_magn;
  MagLib::vByteMag(SIZE, sSlab_magn);
  s32 sUsed_magn;
  MagLib::vByteMag(WspConvergent_t<SIZE,Stz,Row>::nUsed(),sUsed_magn);
  s32 sFree_magn;
  MagLib::vByteMag(WspConvergent_t<SIZE,Stz,Row>::nFree(),sFree_magn);

  vTrace("RowBuffer space %s, used %s, free %s %lld%%; "
         "rows %lld, %lld(B) per row\n"_Stz,
         
         sSlab_magn.szStz(),
         sUsed_magn.szStz(),
         sFree_magn.szStz(),
         (WspConvergent_t<SIZE,Stz,Row>::nFree() * 100) / SIZE,
         
         nRows(),
         WspConvergent_t<SIZE,Stz,Row>::nUsed() / nRows()
         );
}

/*----------------------------------------------------------------------------*/
/* get the condensed stream def                                               */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
RowPix* RowBox_t<SIZE>::aRowPix(void)
{

  return & m_oRowPix;
}

/*----------------------------------------------------------------------------*/
/* mem zero the given row                                                     */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
void RowBox_t<SIZE>::vZeroRow(Row *aRow)
{

  i8 nCellBytes = m_oRowPix.m_nCols * C_BYTES_PER_WORD;

  MemLib::vZero(aRow->aCellArray(),nCellBytes,CN_ROWBUFFER);
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get slab cell for given row                                                */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
i8 RowBox_t<SIZE>::i_RowBufferCell(i8 i_Row)
{
  
  i8 i_Cell_slab = (i_Row + 1) * (m_oRowPix.m_nCols + 1 /* Object */ );
  
  i8 i_CellHazard = SIZE / C_BYTES_PER_WORD;
  
  i8 i_Cell_row  = i_CellHazard - i_Cell_slab;
  
  return i_Cell_row;
}

/*----------------------------------------------------------------------------*/
/* get the cell vector for this slab                                          */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
m8* RowBox_t<SIZE>::amSlabCellVec(void)
{

  m8* amCellArea = (m8*)WspConvergent_t<SIZE,Stz,Row>::ayArea();
  
  return amCellArea;
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
