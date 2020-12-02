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
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);

    RowBox_t(void);
    RowBox_t(StreamDef* pStreamDef, bool bAddPtnHash);
   ~RowBox_t(void) noexcept(false);
  
  
    void vResolveStreamPix(StreamDef* pStreamDef);
    
    Row* aAllotNewRow(void);
  
  
    d8   dColVal(i8 i_Col);
    i8   iColVal(i8 i_Col);
    Stz* szColVal(i8 i_Col);

    bool bIsColNull(i8 i_Col);

    Row* aFirstRow(void);
    Row* aLastRow(void);
    Row* aNextRow(Row* aPriorRow);
  
//    void vPrintRows(StreamDef* pStreamDef);
//    void vShow(StreamDef* pStreamDef);
    
  private:
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
