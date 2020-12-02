/*============================================================================*/
/* RowSingle.cpp -- Computable form of single Row                             */
/*============================================================================*/

#include "Rows.h"

//#define _ROW_BASED 1

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
RowSingle* Rows::_pRowSingle(void* aAddr)
{
  RowSingle* pRowSingle = reinterpret_cast<RowSingle*>(aAddr);
  
  return pRowSingle;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* RowSingle::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_ROWSINGLE);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* perform new of row buffer for given address                                */
/*----------------------------------------------------------------------------*/
void* RowSingle::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void RowSingle::operator delete(void* aAddr)
{
  RowSingle* pRowSingle = reinterpret_cast<RowSingle*>(aAddr);
  
  HeapMem::vFree(pRowSingle);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowSingle::RowSingle(StreamDef* pStreamDef, i8 iSubSet)
#ifdef _ROW_BASED
: Object(CN_ROWSINGLE)
#else
: RowWsp(CN_ROWSINGLE)
#endif
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  m_pStreamDef  = pStreamDef;
  m_nColumns    = pStreamDef->nColumns(iSubSet);
  m_iSubSet     = iSubSet;
  
  m_bGetAll     = true;
  
  for (i8 i_Col = 0; i_Col < C_MAX_COLUMNS; i_Col++)
    m_abGetCol[i_Col] = false;
  m_iLastCol    = C_UNKNOWN;
  
  m_aRow        = nullptr;
  
  // - - - - - - - - - - - - - - - - - -
  
  m_awaColVal = (Dtm*)m_xWsp.ayAllotTail(m_nColumns * sizeof(Dtm));
  
#ifdef WITH_CHECK_INITIALISED
  for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++)
    m_awaColVal[i_Col].m_iWrd = 0;
#else
  // m_xStzWsp
#endif
  
  _checkInitialised_(CN_ROWSINGLE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RowSingle::~RowSingle(void) noexcept(false)
{
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* set column value in row                                                    */
/*----------------------------------------------------------------------------*/
void RowSingle::vAllotSetVarchar(i8 i_Col, Stz* szVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
      
  char* acDstString = (char*)m_xWsp.ayLatterHead();
  
  char* acSrcString = (char*)szVal;
  
  i8 nSrcLength = StrLib::nCopy(acDstString,acSrcString);
  
  m_awaColVal[i_Col].m_iWrd = (i8)acDstString;
  
  m_xWsp.vTakeHead(nSrcLength + 1);
}

/*----------------------------------------------------------------------------*/
/* get address of column datum                                                */
/*----------------------------------------------------------------------------*/
Dtm* RowSingle::aColDtm(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  Dtm* aDtm = & m_awaColVal[i_Col];
  
  return aDtm;
}


/*----------------------------------------------------------------------------*/
/* get column value                                                           */
/*----------------------------------------------------------------------------*/
char* RowSingle::azColVal(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  char* azVal = m_awaColVal[i_Col].m_azStr;
  
  return azVal;
}

/*----------------------------------------------------------------------------*/
/* get column value                                                           */
/*----------------------------------------------------------------------------*/
i8 RowSingle::iColVal(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  i8 iVal = m_awaColVal[i_Col].m_iWrd;
  
  return iVal;
}

/*----------------------------------------------------------------------------*/
/* get column value                                                           */
/*----------------------------------------------------------------------------*/
d8 RowSingle::dColVal(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  d8 dVal = m_awaColVal[i_Col].m_dWrd;
  
  return dVal;
}

/*----------------------------------------------------------------------------*/
/* get column value                                                           */
/*----------------------------------------------------------------------------*/
Stz* RowSingle::szColVal(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  Stz* szVal = m_awaColVal[i_Col].m_szVch;
  
  return szVal;
}

/*----------------------------------------------------------------------------*/
/* get column value                                                           */
/*----------------------------------------------------------------------------*/
Dtm RowSingle::wColVal(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  Dtm wDtm = m_awaColVal[i_Col];
  
  return wDtm;
}

/*----------------------------------------------------------------------------*/
/* append column value to row                                                 */
/*----------------------------------------------------------------------------*/
void RowSingle::vCopy(Row* pRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  for (i8 i_Col = 0; i_Col < m_pStreamDef->nColumns(C_ALL); i_Col++) {
  
    Column* pCol = m_pStreamDef->pGetColumn(i_Col);
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // vch
    
    if (DM_VCH == pCol->eDatum()) {
      
      if (pRow->bIsColNull(i_Col)) {
        m_awaColVal[i_Col].vSetNull();
        continue;
      }
      
      char* acSrcString = (char*)pRow->szColVal_ofs(i_Col);
      vAllotSetVarchar(i_Col, (Stz*)acSrcString);
      
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // all others
    
    m_awaColVal[i_Col].m_iWrd = pRow->iColVal_int(i_Col);
  }
}

/*----------------------------------------------------------------------------*/
/* don't decode vch                                                           */
/*----------------------------------------------------------------------------*/
void RowSingle::vDontDecodeVch(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);

  m_bGetAll = false;
  
  for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++) {
    
    Column* pCol = m_pStreamDef->pGetColumn(i_Col);
    
    if (DM_VCH == pCol->eDatum()) {
      m_abGetCol[i_Col] = false;
    }
    else
      m_abGetCol[i_Col] = true;
    m_iLastCol = i_Col;
  }
}

/*----------------------------------------------------------------------------*/
/* hash a partion key column into running hash                                */
/*----------------------------------------------------------------------------*/
byte* RowSingle::ayDstByte(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  byte* ayDst = (byte*) & m_awaColVal[0].m_iWrd;
  
  return ayDst;
}

/*----------------------------------------------------------------------------*/
/* hash a partion key column into running hash                                */
/*----------------------------------------------------------------------------*/
bool RowSingle::bIsColNull(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  bool bIsNull = m_awaColVal[i_Col].bIsNull();
  
  return bIsNull;
}

/*----------------------------------------------------------------------------*/
/* reset all column values                                                    */
/*----------------------------------------------------------------------------*/
void RowSingle::vResetValues(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
    
#ifdef WITH_CHECK_INITIALISED
  for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++)
    m_awaColVal[i_Col].m_iWrd = 0;
#endif
  
  m_xWsp.vInit();
}

/*----------------------------------------------------------------------------*/
/* set all the column of this row single                                      */
/*----------------------------------------------------------------------------*/
void RowSingle::vSet(i8 iVArgsBegin, ...)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  ASSERT(C_VARGS_BEGIN == iVArgsBegin);
  
  va_list xVarArgs;
  
  va_start(xVarArgs,iVArgsBegin);

  for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++) {
    
    Column* pCol = m_pStreamDef->pGetColumn(i_Col);
    
    if (DM_VCH == pCol->eDatum()) {
      Stz* szVal = va_arg(xVarArgs,Stz*);
      if (szVal)
        vAllotSetVarchar(i_Col,szVal);
      else
        m_awaColVal[i_Col].vSetNull();
    }
    else {
      i8 iVal = va_arg(xVarArgs,i8);
      m_awaColVal[i_Col].m_iWrd = iVal;
    }
    
  }
  
  i8 iVArgsEnd = va_arg(xVarArgs,i8);
  ASSERT(C_VARGS_END == iVArgsEnd);

  va_end(xVarArgs);
}

/*----------------------------------------------------------------------------*/
/* set column value to be null                                                */
/*----------------------------------------------------------------------------*/
void RowSingle::vSetNull(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  m_awaColVal[i_Col].m_iWrd = C_DATUM_NULL;
}

/*----------------------------------------------------------------------------*/
/* set column value in row                                                    */
/*----------------------------------------------------------------------------*/
void RowSingle::vSetValue(Datum* pDatum, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  i8 i_Col = pCol->iColumnNo();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // vch
  
  if (DM_VCH == pCol->eDatum()) {
    
    if (pDatum->bIsNull()) {
      m_awaColVal[i_Col].vSetNull();
      return;
    }
    
    char* acDstString = (char*)m_xWsp.ayLatterHead();
    
    char* acSrcString = (char*)pDatum->azChar();
    
    i8 nSrcLength = StrLib::nCopy(acDstString,acSrcString);
    
    m_awaColVal[i_Col].m_iWrd = (i8)acDstString;
    
    m_xWsp.vTakeHead(nSrcLength + 1);
        
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // all others
  
  m_awaColVal[i_Col].m_iWrd = pDatum->iInt();
}

/*----------------------------------------------------------------------------*/
/* get size of row (needed for MiniHeap_t<ITEM>::aAdjacentItem(ITEM* aItem))  */
/*----------------------------------------------------------------------------*/
i8 RowSingle::nSizeBytes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  return sizeof(RowSingle);
}

/*----------------------------------------------------------------------------*/
/* get size of row data                                                       */
/*----------------------------------------------------------------------------*/
i8 RowSingle::nSizeRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  i8 nSizeBytes = m_pStreamDef->nColumns(C_ALL) * sizeof(i8);
  
  return nSizeBytes;
}

/*----------------------------------------------------------------------------*/
/* get size of row data                                                       */
/*----------------------------------------------------------------------------*/
i8 RowSingle::nSizeVch(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  i8 nSizeVch = 0;
  
  for (i8 i_Col = 0; i_Col < m_pStreamDef->nColumns(C_ALL); i_Col++) {
    
    Column* pCol = m_pStreamDef->pGetColumn(i_Col);
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // vch
    
    if (DM_VCH == pCol->eDatum()) {
      
      if (m_awaColVal[i_Col].bIsNull()) {
        continue;
      }
      
      char* acSrcString = (char*)m_awaColVal[i_Col].m_szVch;
      
      nSizeVch += StrLib::nSize(acSrcString);
      
      continue;
    }

  }
  
  return nSizeVch;
}

/*----------------------------------------------------------------------------*/
/* get string area                                                            */
/*----------------------------------------------------------------------------*/
Stz* RowSingle::szStzArea(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);

  return (Stz*)m_xWsp.ayArea();
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* RowSingle::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
