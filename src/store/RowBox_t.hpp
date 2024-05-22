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

    s32 m_sBufferNm;

    i8 m_nRowSizeProper;

    Row* m_aRowCur;     // current row
    i8   m_i_RowCrs;    // current logical row number
    i8   m_i_ColCrs;    // current working column
  
    i8   m_nRowStringBytes; // string size of current row (discard)

    char m_caModel[64]; // row slab model string
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

    void vResolveRowSize(StreamDef* pStreamDef);

    Row* pAllotRow(void);

    Dtm* aColDtm(i8 i_Col);
    void vColSetNull(i8 i_Col);
    d8   dColVal(i8 i_Col);
    i8   iColVal(i8 i_Col);
    Stz* szColVal(i8 i_Col);

    bool bIsColNull(i8 i_Col);

    Row* pRowAddress(i8 i_Row);
    i8   nRows(void);
    //Row* pFirstRow(void);
    //Row* pLastRow(void);
    //Row* pNextRow(Row* pPriorRow);
  
//    void vPrintRows(StreamDef* pStreamDef);
//    void vShow(StreamDef* pStreamDef);

    void vAdvanceCursor(void);
    void vDepositColVal(d8 dColVal);
    void vDepositColVal(i8 i_Col, i8 iColVal);
    void vDepositColVal(i8 iColVal);
    void vDepositColVal(Stz* szColVal);
    void vDepositColVal(i8 i_Col, Stz* szColVal);
    char* azDepositString_begin(void);
    void vDepositString_end(i8 nSize);

    //void  vDiscardInvalidRow(void);
    //void  vDumpAllRows(void);
    //void  vDumpNst(void);
    //void  vDumpRow(i8 i_Row);
    //void  vDumpRow(i8 i_Row, Row* aRow);

    //i8    nExtraCols(void);
    //void  vInitPix(StreamDef* pStreamDef, bool bAddKeyHash);
    //NmlStr* pNmlStr(void);
    //void  vNormaliseStrings(void);
    //i8    iPtnOfRow(Row* aRow);
    //void  vPositionOnCell(i8 i_Col);
    //bool  bPtnHash(void);
    void  vReset(void);
    void  vResetRow(void);
    //i8    nRowProperBytes(void);
    Row*  pRowCurrent(void);
    //Row*  aRowByOfs(i8 iOfs);
    //Row*  aSetCurRow(i8 iRow);
    //i8    iOffsetInWsp(Row* aRow);
    //void  vSlabReport(void);
    //RowPix* pRowPix(void);
    //void  vResetRowPix(void);
    //void  vZeroRow(Row* aRow);
  
    Stz* szBufferNm(void);
    void vBufferNm(Stz* szBufferNm);

  private:
    //i8    i_RowBufferCell(i8 i_Row);
  
    Footer* aFooter(void);

  };

}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void* RowBox_t<AREA>::operator new(size_t)
{
  vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
         "fatal: RowBox class cannot be new'd\n"_Stz);

  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
RowBox_t<AREA>::RowBox_t(void)
{
  _poolAddress_(this); _footer_();
  
  vInitRowBox_t();
  
  m_nRowSizeProper = 0;
  
  _poolAddress_(this); _footer_();
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
RowBox_t<AREA>::~RowBox_t(void) noexcept(false)
{
  _poolAddress_(this); _footer_();
}

/*----------------------------------------------------------------------------*/
/* resolve row size                                                           */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void RowBox_t<AREA>::vResolveRowSize(StreamDef* pStreamDef)
{
  _poolAddress_(this); _footer_();

  m_nRowSizeProper = Row::nRowProper(pStreamDef->nColumns());

}

/*----------------------------------------------------------------------------*/
/* initialise this row box                                                    */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void RowBox_t<AREA>::vInitRowBox_t(void)
{
  _poolAddress_(this); _footer_();
  
  m_sBufferNm = C_NO_NAME;

  MemLib::vSet(m_caModel,' ',sizeof(m_caModel),CN_PDS);
  m_caModel[sizeof(m_caModel) - 1] = '\0';
  
  StrLib::vSprint(m_caModel,
                  sizeof(m_caModel),
                  nullptr,
                  "RapidsSE %s SeFormat %s"_Stz,
                  AM::szVersionString(),
                  AM::szFormatString());
  
  m_iSeVersion = AM::iVersionNumber();
  m_iSeFormat  = AM::iFormatNumber();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_aRowCur  = nullptr;
  m_i_RowCrs = C_UNKNOWN;
  m_i_ColCrs = C_UNKNOWN;

  m_nRowStringBytes = 0;
  
  _poolAddress_(this); _footer_();
}


/*----------------------------------------------------------------------------*/
/* -- public ---------------------------------------------------------------- */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot a ref set in this buffer (may return nulltpr)                        */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* allot a ref set in this buffer (may return nulltpr)                        */
/*----------------------------------------------------------------------------*/

//HACK(RowBuffer,"quick fix to check for enough space ");

template <i8 AREA>
Row* RowBox_t<AREA>::pAllotRow(void)
{
  _poolAddress_(this); _footer_();
  
  m_nRowStringBytes = 0;
  m_i_ColCrs  = 0;

  if (WspConvergent_t<AREA,Stz,Row>::nFreeBytes() < C_2KB)
    return nullptr;

  m_aRowCur = WspConvergent_t<AREA,Stz,Row>::aAllotTail(m_nRowSizeProper);

  if (! m_aRowCur)
    return nullptr;

  // zero the row
  //MemLib::vZero(m_aRowCur,m_nRowSizeProper,CN_ROW);

  new(m_aRowCur) Row();

  m_i_RowCrs++;
 
  return m_aRowCur;
}

/*----------------------------------------------------------------------------*/
/* get datum value of given column                                            */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
Dtm* RowBox_t<AREA>::aColDtm(i8 i_Col)
{
  _poolAddress_(this); _footer_();

  Dtm* aDtm = m_aRowCur->aColDtm(i_Col);
  
  return aDtm;
}

/*----------------------------------------------------------------------------*/
/* set column value to be dull                                                */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void RowBox_t<AREA>::vColSetNull(i8 i_Col)
{
  _poolAddress_(this); _footer_();

  m_aRowCur->vSetColNull(i_Col);
}

/*----------------------------------------------------------------------------*/
/* get float value of given column                                            */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
d8 RowBox_t<AREA>::dColVal(i8 i_Col)
{
  _poolAddress_(this); _footer_();

  d8 dVal = m_aRowCur->dColVal_flt(i_Col);
  
  return dVal;
}

/*----------------------------------------------------------------------------*/
/* get integer value of given column                                          */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
i8 RowBox_t<AREA>::iColVal(i8 i_Col)
{
  _poolAddress_(this); _footer_();


  i8 iVal = m_aRowCur->iColVal_int(i_Col);
  
  return iVal;
}

/*----------------------------------------------------------------------------*/
/* get stz value of given column                                              */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
Stz* RowBox_t<AREA>::szColVal(i8 i_Col)
{
  _poolAddress_(this); _footer_();

  Stz* szVal = m_aRowCur->szColVal_ofs(i_Col);
  
  return szVal;
}



/*----------------------------------------------------------------------------*/
/* is given column a null?                                                    */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
bool RowBox_t<AREA>::bIsColNull(i8 i_Col)
{
  _poolAddress_(this); _footer_();

  bool bIsNull = m_aRowCur->bIsColNull(i_Col);
  
  return bIsNull;
}



/*----------------------------------------------------------------------------*/
/* get address of an row                                                      */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
Row* RowBox_t<AREA>::pRowAddress(i8 i_Row)
{
  _poolAddress_(this); _footer_();


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
  _poolAddress_(this); _footer_();

  return m_aRowCur;
}

/*----------------------------------------------------------------------------*/
/* get number of rows                                                         */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
i8 RowBox_t<AREA>::nRows(void)
{
  _poolAddress_(this); _footer_();
  //vAssertAux0();

  return WspConvergent_t<AREA,Stz,Row>::nItems();
}

/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/* advance cursor                                                           */
/*--------------------------------------------------------------------------*/
template <i8 AREA>
void RowBox_t<AREA>::vAdvanceCursor(void)
{
  _poolAddress_(this); _footer_();

  m_i_ColCrs++;
}

/*--------------------------------------------------------------------------*/
/* deposit an d8 value into next cell                                       */
/*--------------------------------------------------------------------------*/
template <i8 AREA>
void RowBox_t<AREA>::vDepositColVal(d8 dColVal)
{
  _poolAddress_(this); _footer_();

  m_aRowCur->vColVal_flt(m_i_ColCrs,dColVal);
  
  vAdvanceCursor();

}

/*--------------------------------------------------------------------------*/
/* deposit an i8 value into next cell                                       */
/*--------------------------------------------------------------------------*/
template <i8 AREA>
void RowBox_t<AREA>::vDepositColVal(i8 iColVal)
{
  _poolAddress_(this); _footer_();

  m_aRowCur->vColVal_int(m_i_ColCrs,iColVal);

  vAdvanceCursor();
}

/*----------------------------------------------------------------------------*/
/* get datum value of given column                                            */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void RowBox_t<AREA>::vDepositColVal(i8 i_Col, i8 iColVal)
{
  _poolAddress_(this); _footer_();

  m_aRowCur->vColVal_int(i_Col,iColVal);
}

/*--------------------------------------------------------------------------*/
/* deposit an stz value into next cell                                      */
/*--------------------------------------------------------------------------*/
template <i8 AREA>
void RowBox_t<AREA>::vDepositColVal(Stz* szColVal)
{
  _poolAddress_(this); _footer_();

  char* azTgt = azDepositString_begin();
  i8 iFreeBytes = WspConvergent_t<AREA,Stz,Row>::nFreeBytes();

  i8 nLen = StrLib::nCopy(azTgt, iFreeBytes, szColVal->azChar());

  vDepositString_end(nLen + 1);
}

/*----------------------------------------------------------------------------*/
/* get datum value of given column                                            */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void RowBox_t<AREA>::vDepositColVal(i8 i_Col, Stz* szColVal)
{
  _poolAddress_(this); _footer_();

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
  _poolAddress_(this); _footer_();

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
  _poolAddress_(this); _footer_();

  WspConvergent_t<AREA,Stz,Row>::vTakeHead(nSize);
  
  m_nRowStringBytes += nSize;


}

/*----------------------------------------------------------------------------*/
/* reset this rowslab                                                         */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void RowBox_t<AREA>::vReset(void)
{
  _poolAddress_(this); _footer_();

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
  _poolAddress_(this); _footer_();

  m_i_ColCrs = 0;
  m_nRowStringBytes = 0;

  WspConvergent_t<AREA,Stz,Row>::vResetHeads();
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get name of buffer                                                         */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
Stz* RowBox_t<AREA>::szBufferNm(void)
{
  _poolAddress_(this); _footer_();

  return m_sBufferNm;
}

/*----------------------------------------------------------------------------*/
/* set name of buffer                                                         */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
void RowBox_t<AREA>::vBufferNm(Stz* szBufferNm)
{
  _poolAddress_(this); _footer_();

  m_sBufferNm = szBufferNm;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <i8 AREA>
Footer* RowBox_t<AREA>::aFooter(void)
{
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
