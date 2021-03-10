/*============================================================================*/
/* RowBox_t.hpp -- Row Container API                                          */
/*============================================================================*/
#pragma once

#include "Definition.hpp"

#include "Row.hpp"

#include "NmlStr.hpp"

#include "Record.hpp"

namespace STORE
{
  template <i8 AREA>
  class RowBox_t : public WspConvergent_t<AREA,Stz,Row>
  {
  protected:

    i8 m_nRowSizeProper;
  
    RowPix* m_pRowPix;

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
    
    Row* pAllotNewRow(void);
    RowPix* pAllotRowPix(i8 nSizeRowPix);
  
    void vAssertAux0(void);

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
    Row* pFirstRow(void);
    Row* pLastRow(void);
    Row* pNextRow(Row* pPriorRow);
  
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
    RowPix* pRowPix(void);
    void  vResetRowPix(void);
    void  vZeroRow(Row* aRow);

  private:
    i8    i_RowBufferCell(i8 i_Row);
  };

}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void* RowBox_t<AREA>::operator new(size_t)
{
  vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
         "fatal: RowBox class cannot be new'd\n"_Stz);

  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
RowBox_t<AREA>::RowBox_t(void)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();

  vInitRowBox_t();
  
  m_nRowSizeProper = 0;
  
   
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
RowBox_t<AREA>::RowBox_t(StreamDef* pStreamDef, bool bAddPtnHash)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();

  vInitRowBox_t();

  //pAllotRowPix(sizeof(RowPix));
  pAllotRowPix(pStreamDef->pRowPix()->nDynamicSize());

  pStreamDef->vToRowPix(m_pRowPix,bAddPtnHash);
  
  m_nRowSizeProper = Row::nRowProper(pStreamDef->nColumns());
    
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
RowBox_t<AREA>::~RowBox_t(void) noexcept(false)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();

}


/*----------------------------------------------------------------------------*/
/* initialise this row box                                                    */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void RowBox_t<AREA>::vInitRowBox_t(void)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();

  MemLib::vSet(m_caModel,' ',sizeof(m_caModel),CN_ROWBUFFER);
  m_caModel[sizeof(m_caModel) - 1] = '\0';
  
  StrLib::vSprint(m_caModel,"RapidsSE %s SeFormat %s"_Stz,
                  AM::szVersionString(),
                  AM::szFormatString());
  
  m_iSeVersion = AM::iVersionNumber();
  m_iSeFormat  = AM::iFormatNumber();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  m_pRowPix  = nullptr;
  m_aRowCur  = nullptr;
  m_i_RowCrs = C_UNKNOWN;
  m_i_ColCrs = C_UNKNOWN;

  m_nRowStringBytes = 0;
  
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
}


/*----------------------------------------------------------------------------*/
/* -- public ---------------------------------------------------------------- */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot a ref set in this buffer (may return nulltpr)                        */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void RowBox_t<AREA>::vResolveRowPix(StreamDef* pStreamDef)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();

  //pAllotRowPix(sizeof(RowPix));
  pAllotRowPix(pStreamDef->pRowPix()->nDynamicSize());
  
  pStreamDef->vToRowPix(m_pRowPix,false);
  
  m_nRowSizeProper = Row::nRowProper(m_pRowPix->m_nColumns);
  
}

/*----------------------------------------------------------------------------*/
/* allot a ref set in this buffer (may return nulltpr)                        */
/*----------------------------------------------------------------------------*/

//HACK(RowBuffer,"quick fix to check for enough space ");

template <i8 AREA>
Row* RowBox_t<AREA>::pAllotNewRow(void)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

  RASSERT(m_pRowPix);
  
  m_nRowStringBytes = 0;
  m_i_ColCrs  = 0;

  if (WspConvergent_t<AREA,Stz,Row>::nFreeBytes() < C_2KB)
    return nullptr;

  m_aRowCur = WspConvergent_t<AREA,Stz,Row>::aAllotTail(m_nRowSizeProper);

  if (! m_aRowCur)
    return nullptr;

  RASSERT((void*)m_aRowCur != (void*)m_pRowPix);

  // zero the row
  MemLib::vZero(m_aRowCur,m_nRowSizeProper,CN_ROW);

  new(m_aRowCur) Row();

  m_i_RowCrs++;
 
  return m_aRowCur;
}

/*----------------------------------------------------------------------------*/
/* allot a row pix NOT COMPLETE                                               */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
RowPix* RowBox_t<AREA>::pAllotRowPix(i8 nSizeRowPix)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  
  RASSERT(! m_pRowPix);
  RASSERT(! nRows());

  //RASSERT(sizeof(RowPix) == nSizeRowPix);
  RASSERT( (! WspConvergent_t<AREA,Stz,Row>::ayAux0()) );
  WspConvergent_t<AREA,Stz,Row>::ayAllotAux0(nSizeRowPix);
  RASSERT( (WspConvergent_t<AREA,Stz,Row>::ayAux0()) );
  
  RowPix* pRowPix = (RowPix*)WspConvergent_t<AREA,Stz,Row>::ayAux0();
  new (pRowPix) RowPix;
  
//  Footer* aFooter = (Footer*)WspConvergent_t<AREA,Stz,Row>::ayAux0();
//  aFooter->vSetFooter();
  
  RASSERT(! m_pRowPix);

  m_pRowPix = pRowPix;

  //vAssertAux0();
  

  return m_pRowPix;
}

/*----------------------------------------------------------------------------*/
/* assert aux is still valid                                                  */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void RowBox_t<AREA>::vAssertAux0(void)
{
//  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
//
//  if (! WspConvergent_t<AREA,Stz,Row>::ayAux0()) {
//    ASSERT(! m_pRowPix);
//    ASSERT(! nRows());
//    return;
//  }
//
//  ASSERT(m_pRowPix);
//  ASSERT((void*)this == (void*)m_pRowPix);
//
////  Footer* aFooter = (Footer*)WspConvergent_t<AREA,Stz,Row>::ayAux0();
////  aFooter->vAssertFooter();
//
//  RowPix* pRowPix = (RowPix*)WspConvergent_t<AREA,Stz,Row>::ayAux0();
//
//  pRowPix->vAssertRowPix();
//
//  ASSERT(pRowPix == m_pRowPix);
//  ASSERT(pRowPix->nColumns());

}

/*----------------------------------------------------------------------------*/
/* get the condensed column pix array                                         */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
ColPix* RowBox_t<AREA>::axaColPix(void)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();
  
  return m_pRowPix->m_xaColPix;
}


/*----------------------------------------------------------------------------*/
/* copy given stream pix into the stream pix in this row                      */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void RowBox_t<AREA>::vCopyRowPix(RowPix* pRowPixSrc)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

  //pAllotRowPix(sizeof(RowPix));
  pAllotRowPix(pRowPixSrc->nDynamicSize());

  MemLib::vCopy(m_pRowPix,pRowPixSrc,sizeof(RowPix),CN_ROWBUFFER);

  m_nRowSizeProper = Row::nRowProper(pRowPixSrc->m_nColumns);
  
  vAssertAux0();

}

/*----------------------------------------------------------------------------*/
/* copy given source row into current row of this row slab                    */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void RowBox_t<AREA>::vCopyRow(Row* aRowSrc, bool bSrcHasPtnHash, bool bCopyPtnHash)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();
  
  ASSERT(m_pRowPix->m_bInitialised);
  ASSERT(m_pRowPix->m_nColumns);

  i8 iVal;
  Stz* szSrcStz;
  
  ASSERT(0 == m_i_ColCrs);
  
  i8 nCols = m_pRowPix->m_nColumns;
  
  i8 i_Col;
  
  for (i_Col = 0; i_Col < nCols; i_Col++) {
    
    eDM eDatum = (eDM)m_pRowPix->m_xaColPix[i_Col].m_uDatum;
    
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
  
}

/*----------------------------------------------------------------------------*/
/* return column value from row for given column number                       */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
i8 RowBox_t<AREA>::iColumn(Row* aRow, i8 i_Col)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

  i8 iVal;
  Stz* szSrcStz;
    
  eDM eDatum = (eDM)m_pRowPix->m_xaColPix[i_Col].m_uDatum;
    
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
template <i8 AREA>
i8 RowBox_t<AREA>::nColumns(void)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

  return m_pRowPix->m_nColumns;
}

/*----------------------------------------------------------------------------*/
/* get datum value of given column                                            */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
Dtm* RowBox_t<AREA>::aColDtm(i8 i_Col)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

  Dtm* aDtm = m_aRowCur->aColDtm(i_Col);
  
  return aDtm;
}

/*----------------------------------------------------------------------------*/
/* set column value to be dull                                                */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void RowBox_t<AREA>::vColSetNull(i8 i_Col)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

  m_aRowCur->vSetColNull(i_Col);
}

/*----------------------------------------------------------------------------*/
/* get float value of given column                                            */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
d8 RowBox_t<AREA>::dColVal(i8 i_Col)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

  d8 dVal = m_aRowCur->dColVal_flt(i_Col);
  
  return dVal;
}

/*----------------------------------------------------------------------------*/
/* get integer value of given column                                          */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
i8 RowBox_t<AREA>::iColVal(i8 i_Col)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

  i8 iVal = m_aRowCur->iColVal_int(i_Col);
  
  return iVal;
}

/*----------------------------------------------------------------------------*/
/* get stz value of given column                                              */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
Stz* RowBox_t<AREA>::szColVal(i8 i_Col)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

  Stz* szVal = m_aRowCur->szColVal_ofs(i_Col);
  
  return szVal;
}



/*----------------------------------------------------------------------------*/
/* is given column a null?                                                    */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
bool RowBox_t<AREA>::bIsColNull(i8 i_Col)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

  bool bIsNull = m_aRowCur->bIsColNull(i_Col);
  
  return bIsNull;
}



/*----------------------------------------------------------------------------*/
/* get address of an row                                                      */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
Row* RowBox_t<AREA>::pRowAddress(i8 i_Row)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();  vAssertAux0();
  vAssertAux0();

  /*          v row[1]            v row[0]
       -------*----+----+----*----+----+---+----+----*
              | Ob : c0 : c1 : cN | Ob :c0 : c1 : cN |!!!hazard
       -------*----+----+----*----+----+---+----+----*
   */
    
  Row* pRowAddr = WspConvergent_t<AREA,Stz,Row>::aTailItem(i_Row);
  
  Row* pRow = _pRow(pRowAddr);
  
  return pRow;
}

/*----------------------------------------------------------------------------*/
/* get current row                                                            */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
Row* RowBox_t<AREA>::pRowCurrent(void)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

  return m_aRowCur;
}

/*----------------------------------------------------------------------------*/
/* get number of rows                                                         */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
i8 RowBox_t<AREA>::nRows(void)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  //vAssertAux0();

  return WspConvergent_t<AREA,Stz,Row>::nItems();
}

/*----------------------------------------------------------------------------*/
/* return first ref set in this buffer                                        */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
Row* RowBox_t<AREA>::pFirstRow(void)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

  RASSERT(m_pRowPix);
  
  Row* pRow = WspConvergent_t<AREA,Stz,Row>::aFirstItem();
  
  return pRow;
}

/*----------------------------------------------------------------------------*/
/* return last ref set in this buffer                                         */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
Row* RowBox_t<AREA>::pLastRow(void)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

  RASSERT(m_pRowPix);

  Row* pRow = WspConvergent_t<AREA,Stz,Row>::aFirstItem();
  Row* pLastRow = nullptr;
  
  while (pRow) {
    pLastRow = pRow;
    pRow = pNextRow(pRow);
  };
  
  return pLastRow;
}

/*----------------------------------------------------------------------------*/
/* return next ref set in this buffer                                         */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
Row* RowBox_t<AREA>::pNextRow(Row* pPriorRow)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

  RASSERT(m_pRowPix);

  Row* pRow = WspConvergent_t<AREA,Stz,Row>::aAdjacentTail(pPriorRow,
                                                           m_nRowSizeProper);
  
  return pRow;
}








/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/* advance cursor                                                           */
/*--------------------------------------------------------------------------*/
template <i8 AREA>
void RowBox_t<AREA>::vAdvanceCursor(void)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

  m_i_ColCrs++;
}


/*--------------------------------------------------------------------------*/
/* deposit an d8 value into next cell                                       */
/*--------------------------------------------------------------------------*/
template <i8 AREA>
void RowBox_t<AREA>::vDepositColVal(d8 dColVal)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

  m_aRowCur->vColVal_flt(m_i_ColCrs,dColVal);
  
  vAdvanceCursor();

}

/*--------------------------------------------------------------------------*/
/* deposit an i8 value into next cell                                       */
/*--------------------------------------------------------------------------*/
template <i8 AREA>
void RowBox_t<AREA>::vDepositColVal(i8 iColVal)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

  m_aRowCur->vColVal_int(m_i_ColCrs,iColVal);

  vAdvanceCursor();
}

/*----------------------------------------------------------------------------*/
/* get datum value of given column                                            */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void RowBox_t<AREA>::vDepositColVal(i8 i_Col, i8 iColVal)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

  m_aRowCur->vColVal_int(i_Col,iColVal);
}

/*--------------------------------------------------------------------------*/
/* deposit an stz value into next cell                                      */
/*--------------------------------------------------------------------------*/
template <i8 AREA>
void RowBox_t<AREA>::vDepositColVal(Stz* szColVal)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

  char* azTgt = azDepositString_begin();
  
  i8 nLen = StrLib::nCopy(azTgt,szColVal->azChar());

  vDepositString_end(nLen + 1);
}

/*----------------------------------------------------------------------------*/
/* get datum value of given column                                            */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void RowBox_t<AREA>::vDepositColVal(i8 i_Col, Stz* szColVal)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

  Stz* szDeposited = WspConvergent_t<AREA,Stz,Row>::szStzDeposit(szColVal);
  
  i8 i_Ofs = - nDifference(m_aRowCur->aColCell(i_Col),szDeposited);
  
  m_aRowCur->vColVal_ofs(i_Col,i_Ofs);
}

/*--------------------------------------------------------------------------*/
/* begin deposit of string                                                  */
/*--------------------------------------------------------------------------*/
template <i8 AREA>
char* RowBox_t<AREA>::azDepositString_begin(void)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

  char* azStz = (char*)WspConvergent_t<AREA,Stz,Row>::ayLatterHead() + 1;
  
  m8* aColCell = m_aRowCur->aColCell(m_i_ColCrs);
  i8 i_Ofs = - nDifference(azStz,aColCell);

  m_aRowCur->vColVal_ofs(m_i_ColCrs,i_Ofs);

  vAdvanceCursor();


  return azStz;
}

/*--------------------------------------------------------------------------*/
/* take bytes for deposited string                                          */
/*--------------------------------------------------------------------------*/
template <i8 AREA>
void RowBox_t<AREA>::vDepositString_end(i8 nSize)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

  WspConvergent_t<AREA,Stz,Row>::vTakeHead(nSize);
  
  m_nRowStringBytes += nSize;


}


/*----------------------------------------------------------------------------*/
/* discard current invalid/incomplete row                                     */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void RowBox_t<AREA>::vDiscardInvalidRow(void)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

  // undo the row number increments of vNewRow()
  m_i_RowCrs--;
  
  // return tail bytes allotted for cells of current row in vNewRow()
  i8 nCellBytes = nRowProperBytes();
  WspConvergent_t<AREA,Stz,Row>::vGiveTail(nCellBytes);
  
  WspConvergent_t<AREA,Stz,Row>::vDecItems();
  
  // return head bytes allotted for strings deposited in vDepositString_end()
  if (m_nRowStringBytes) {
    WspConvergent_t<AREA,Stz,Row>::vGiveHead(m_nRowStringBytes);
    m_nRowStringBytes = 0;
  }
}

/*----------------------------------------------------------------------------*/
/* dump given row                                                             */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void RowBox_t<AREA>::vDumpAllRows(void)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

  i8 nRows = this->nRows();
  vTrace("RowBuffer of %lld rows...\n"_Stz,nRows);
  for (i8 i_Row = 0; i_Row < nRows; i_Row++)
    vDumpRow(i_Row);
}

/*----------------------------------------------------------------------------*/
/* dump normalised string table                                               */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void RowBox_t<AREA>::vDumpNst(void)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

  m_oNmlStr.vDump();
}

/*----------------------------------------------------------------------------*/
/* dump given row                                                             */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void RowBox_t<AREA>::vDumpRow(i8 i_Row)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

  Row* aRow = this->pRowAddress(i_Row);
  vDumpRow(i_Row,aRow);
}

/*----------------------------------------------------------------------------*/
/* dump given row                                                             */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void RowBox_t<AREA>::vDumpRow(i8 i_Row, Row* aRow)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

  HV_Record hvRecord(pRowPix());

  Stz* szRow = hvRecord->sz(aRow);
  vTrace("[%lld] %s\n"_Stz,i_Row,szRow);
}

/*----------------------------------------------------------------------------*/
/* get number of extra columns                                                */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
i8 RowBox_t<AREA>::nExtraCols(void)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

  return m_pRowPix->m_nExtraCols;
}



/*----------------------------------------------------------------------------*/
/* initilize col pix                                                          */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void RowBox_t<AREA>::vInitPix(StreamDef* pStreamDef, bool bPtnHash)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

  //pAllotRowPix(sizeof(RowPix));
  pAllotRowPix(pStreamDef->pRowPix()->nDynamicSize());

  pStreamDef->vToRowPix(m_pRowPix,bPtnHash);
  
  m_nRowSizeProper = Row::nRowProper(m_pRowPix->m_nColumns);
}

/*----------------------------------------------------------------------------*/
/* get normalised string set                                                  */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
NmlStr* RowBox_t<AREA>::pNmlStr(void)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

  return & m_oNmlStr;
}

/*----------------------------------------------------------------------------*/
/* normalize repeating strings                                                */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void RowBox_t<AREA>::vNormaliseStrings(void)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

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

    eDM eDatum = (eDM)m_pRowPix->m_xaColPix[i_Col].m_uDatum;
    
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
template <i8 AREA>
i8 RowBox_t<AREA>::iOffsetInWsp(Row* aRow)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

  i8 iOfs = WspConvergent_t<AREA,Stz,Row>::iOffsetOfItem(aRow);
  
  return iOfs;
}

/*----------------------------------------------------------------------------*/
/* position on given cell in current row                                      */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void RowBox_t<AREA>::vPositionOnCell(i8 i_Col)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

  m_i_ColCrs = i_Col;
}

/*----------------------------------------------------------------------------*/
/* is there a ptn hash?                                                       */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
bool RowBox_t<AREA>::bPtnHash(void)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

  return m_pRowPix->m_bPtnHash;
}

/*----------------------------------------------------------------------------*/
/* get the partition of row                                                   */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
i8 RowBox_t<AREA>::iPtnOfRow(Row* aRow)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

  i8 i_HshCol = m_pRowPix->m_nColumns - 1;
    
  i8 i_Ptn = aRow->iColVal_int(i_HshCol);
  
  return i_Ptn;
}

/*----------------------------------------------------------------------------*/
/* reset this rowslab                                                         */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void RowBox_t<AREA>::vReset(void)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

  //vTrace("Reset: %p \n"_Stz,this);

  m_i_RowCrs  = C_UNKNOWN;
  m_aRowCur   = nullptr;
  m_i_ColCrs  = C_UNKNOWN;

  WspConvergent_t<AREA,Stz,Row>::vWspInit();
}

/*----------------------------------------------------------------------------*/
/* reset this row                                                             */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void RowBox_t<AREA>::vResetRow(void)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

  WspConvergent_t<AREA,Stz,Row>::vResetHeads();
}

/*----------------------------------------------------------------------------*/
/* get bytes needed to hold row cells proper (i.e. without strings)           */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
i8 RowBox_t<AREA>::nRowProperBytes(void)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

  i8 nBytes = (m_pRowPix->m_nColumns + 1 /* dummy */ ) * C_BYTES_PER_WORD;
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get address of an row by its offset                                        */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
Row* RowBox_t<AREA>::aRowByOfs(i8 i_Ofs)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

  //Row* aRow = aAdd<Row*>(WspConvergent_t<AREA,Stz,Row>::ayArea(),i_Ofs);
  Row* aRow = WspConvergent_t<AREA,Stz,Row>::aItemAtOffset(i_Ofs);

  return aRow;
}

/*----------------------------------------------------------------------------*/
/* get address of an row by its offset and set as current                     */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
Row* RowBox_t<AREA>::aSetCurRow(i8 i_Row)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

  m_i_RowCrs = i_Row;
  m_aRowCur = pRowAddress(i_Row);
  return m_aRowCur;
}

/*----------------------------------------------------------------------------*/
/* dump slap and report                                                       */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void RowBox_t<AREA>::vSlabReport(void)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

  s32 sSlab_magn;
  MagLib::vByteMag(AREA, sSlab_magn);
  s32 sUsed_magn;
  MagLib::vByteMag(WspConvergent_t<AREA,Stz,Row>::nUsedBytes(),sUsed_magn);
  s32 sFree_magn;
  MagLib::vByteMag(WspConvergent_t<AREA,Stz,Row>::nFreeBytes(),sFree_magn);

  vTrace("RowBuffer space %s, used %s, free %s %lld%%; "
         "rows %lld, %lld(B) per row\n"_Stz,
         
         sSlab_magn.szStz(),
         sUsed_magn.szStz(),
         sFree_magn.szStz(),
         (WspConvergent_t<AREA,Stz,Row>::nFreeBytes() * 100) / AREA,
         
         nRows(),
         WspConvergent_t<AREA,Stz,Row>::nUsedBytes() / nRows()
         );
}

/*----------------------------------------------------------------------------*/
/* get the condensed stream def                                               */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
RowPix* RowBox_t<AREA>::pRowPix(void)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

  return m_pRowPix;
}

/*----------------------------------------------------------------------------*/
/* get the condensed stream def                                               */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void RowBox_t<AREA>::vResetRowPix(void)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();

  m_pRowPix = (RowPix*)WspConvergent_t<AREA,Stz,Row>::ayAux0();
  vAssertAux0();
}

/*----------------------------------------------------------------------------*/
/* mem zero the given row                                                     */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void RowBox_t<AREA>::vZeroRow(Row *aRow)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();
  
  RASSERT((void*)aRow != (void*)m_pRowPix);

  i8 nCellBytes = m_pRowPix->m_nColumns * C_BYTES_PER_WORD;

  MemLib::vZero(aRow->aCellArray(),nCellBytes,CN_ROWBUFFER);
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get slab cell for given row                                                */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
i8 RowBox_t<AREA>::i_RowBufferCell(i8 i_Row)
{
  //RowBox_t<AREA>::m_xFooter.vAssertFooter();
  vAssertAux0();

  i8 i_Cell_slab = (i_Row + 1) * (m_pRowPix->m_nColumns + 1 /* Object */ );
  
  i8 i_CellHazard = AREA / C_BYTES_PER_WORD;
  
  i8 i_Cell_row  = i_CellHazard - i_Cell_slab;
  
  return i_Cell_row;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
//template <i8 AREA>
//Footer* RowBox_t<AREA>::aFooter(void)
//{
//  return &m_xFooter;
//}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
