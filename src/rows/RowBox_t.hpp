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
  
    Row* m_aRow;      // current row
  
  
    // from RowSlab
  
    char m_caModel[32]; // row slab model string
    i8   m_iSeVersion;  // se version number
    i8   m_iSeFormat;   // se format number

  
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
  
  
    void vColDeposit(i8 i_Col, i8 iVal);
    void vColDeposit(i8 i_Col, Stz* szVal);
    Dtm* aColDtm(i8 i_Col);
    void vColSetNull(i8 i_Col);
    d8   dColVal(i8 i_Col);
    i8   iColVal(i8 i_Col);
    Stz* szColVal(i8 i_Col);

    bool bIsColNull(i8 i_Col);

    Row* aFirstRow(void);
    Row* aLastRow(void);
    Row* aNextRow(Row* aPriorRow);
  
//    void vPrintRows(StreamDef* pStreamDef);
//    void vShow(StreamDef* pStreamDef);
    
    i8   iToOffset(Stz* aItem);
  
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
  
  m_aRow = nullptr;
  
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
template <i8 SIZE>
Row* RowBox_t<SIZE>::aAllotNewRow(void)
{
  //_precondition_(_ADDRESS_,_MAGIC_);

  RASSERT(m_oStreamPix.m_bInitialised);

  m_aRow = WspConvergent_t<SIZE,Stz,Row>::ayAllotTail(m_nRowSizeProper);

  if (! m_aRow)
    return nullptr;

  new(m_aRow) Row();

  return m_aRow;
}


/*----------------------------------------------------------------------------*/
/* get datum value of given column                                            */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
void RowBox_t<SIZE>::vColDeposit(i8 i_Col, i8 iValue)
{
  m_aRow->vColVal_int(i_Col,iValue);
}

/*----------------------------------------------------------------------------*/
/* get datum value of given column                                            */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
void RowBox_t<SIZE>::vColDeposit(i8 i_Col, Stz* szVal)
{
  Stz* szDeposited = WspConvergent_t<SIZE,Stz,Row>::szStzDeposit(szVal);
  
  i8 i_Ofs = - nDifference(m_aRow->aColCell(i_Col),szDeposited);
  
  m_aRow->vColVal_ofs(i_Col,i_Ofs);
}

/*----------------------------------------------------------------------------*/
/* get datum value of given column                                            */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
Dtm* RowBox_t<SIZE>::aColDtm(i8 i_Col)
{
  Dtm* aDtm = m_aRow->aColDtm(i_Col);
  
  return aDtm;
}

/*----------------------------------------------------------------------------*/
/* set column value to be dull                                                */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
void RowBox_t<SIZE>::vColSetNull(i8 i_Col)
{
  m_aRow->vSetColNull(i_Col);
}

/*----------------------------------------------------------------------------*/
/* get float value of given column                                            */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
d8 RowBox_t<SIZE>::dColVal(i8 i_Col)
{
  d8 dVal = m_aRow->dColVal_flt(i_Col);
  
  return dVal;
}

/*----------------------------------------------------------------------------*/
/* get integer value of given column                                          */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
i8 RowBox_t<SIZE>::iColVal(i8 i_Col)
{
  i8 iVal = m_aRow->iColVal_int(i_Col);
  
  return iVal;
}

/*----------------------------------------------------------------------------*/
/* get stz value of given column                                              */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
Stz* RowBox_t<SIZE>::szColVal(i8 i_Col)
{
  Stz* szVal = m_aRow->szColVal_ofs(i_Col);
  
  return szVal;
}



/*----------------------------------------------------------------------------*/
/* is given column a null?                                                    */
/*----------------------------------------------------------------------------*/
template <i8 SIZE>
bool RowBox_t<SIZE>::bIsColNull(i8 i_Col)
{
  bool bIsNull = m_aRow->bIsColNull(i_Col);
  
  return bIsNull;
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
