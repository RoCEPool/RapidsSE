/*============================================================================*/
/* RowSingle.cpp -- Computable form of single Row                             */
/*============================================================================*/

#include "Rows.h"


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
#ifdef _ROWBOX_BASED
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
    
  // - - - - - - - - - - - - - - - - - -
  
#ifdef _ROWBOX_BASED
  
  Row* aRow = aAllotNewRow();

#else
  
  m_awaColVal = (Dtm*)m_xWsp.ayAllotTail(m_nColumns * sizeof(Dtm));
  
#ifdef WITH_CHECK_INITIALISED
  for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++)
    m_awaColVal[i_Col].m_iWrd = 0;
#else
  // m_xStzWsp
#endif
  
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
#ifdef _ROWBOX_BASED

#else // ???????????????????????????????????????????????????????????????????????

void RowSingle::vAllotSetVarchar(i8 i_Col, Stz* szVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
      
  char* acDstString = (char*)m_xWsp.ayLatterHead();
  
  char* acSrcString = (char*)szVal;
  
  i8 nSrcLength = StrLib::nCopy(acDstString,acSrcString);
  
  m_awaColVal[i_Col].m_iWrd = (i8)acDstString;
  
  m_xWsp.vTakeHead(nSrcLength + 1);
}

#endif

/*----------------------------------------------------------------------------*/
/* get address of column datum                                                */
/*----------------------------------------------------------------------------*/
#ifdef _ROWBOX_BASED

Dtm* RowSingle::aColDtm(i8 i_Col)                                        // DONE
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  Dtm* aDtm = RowBox_t<C_ROWSINGLE_WSP>::aColDtm(i_Col);
  
  return aDtm;
}

#else // ???????????????????????????????????????????????????????????????????????

Dtm* RowSingle::aColDtm(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  Dtm* aDtm = & m_awaColVal[i_Col];
  
  return aDtm;
}

#endif

/*----------------------------------------------------------------------------*/
/* get column value                                                           */
/*----------------------------------------------------------------------------*/
#ifdef _ROWBOX_BASED

Dtm RowSingle::wColDtm(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  Dtm* aDtm = RowBox_t<C_ROWSINGLE_WSP>::aColDtm(i_Col);

  return * aDtm;;
}

#else // ???????????????????????????????????????????????????????????????????????

Dtm RowSingle::wColDtm(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);

  Dtm wDtm = m_awaColVal[i_Col];

  return wDtm;
}

#endif

/*----------------------------------------------------------------------------*/
/* get column value                                                           */
/*----------------------------------------------------------------------------*/
#ifdef _ROWBOX_BASED

char* RowSingle::azColVal(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  char* azVal = (char*)szColVal(i_Col);
  
  return azVal;
}

#else // ???????????????????????????????????????????????????????????????????????

char* RowSingle::azColVal(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  char* azVal = m_awaColVal[i_Col].m_azStr;
  
  return azVal;
}

#endif

/*----------------------------------------------------------------------------*/
/* get column value                                                           */
/*----------------------------------------------------------------------------*/
#ifdef _ROWBOX_BASED

i8 RowSingle::iColVal(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  i8 iVal = RowBox_t<C_ROWSINGLE_WSP>::iColVal(i_Col);

  return iVal;
}

#else // ???????????????????????????????????????????????????????????????????????

i8 RowSingle::iColVal(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  i8 iVal = m_awaColVal[i_Col].m_iWrd;
  
  return iVal;
}

#endif

/*----------------------------------------------------------------------------*/
/* get column value                                                           */
/*----------------------------------------------------------------------------*/
#ifdef _ROWBOX_BASED

d8 RowSingle::dColVal(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  d8 dVal = RowBox_t<C_ROWSINGLE_WSP>::dColVal(i_Col);
  
  return dVal;
}

#else // ???????????????????????????????????????????????????????????????????????

d8 RowSingle::dColVal(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  d8 dVal = m_awaColVal[i_Col].m_dWrd;
  
  return dVal;
}

#endif

/*----------------------------------------------------------------------------*/
/* get column value                                                           */
/*----------------------------------------------------------------------------*/
#ifdef _ROWBOX_BASED

Stz* RowSingle::szColVal(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  Stz* szVal = RowBox_t<C_ROWSINGLE_WSP>::szColVal(i_Col);
  
  return szVal;
}

#else // ???????????????????????????????????????????????????????????????????????

Stz* RowSingle::szColVal(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  Stz* szVal = m_awaColVal[i_Col].m_szVch;
  
  return szVal;
}

#endif

/*----------------------------------------------------------------------------*/
/* append column value to row                                                 */
/*----------------------------------------------------------------------------*/
#ifdef _ROWBOX_BASED

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

#else // ???????????????????????????????????????????????????????????????????????

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

#endif

/*----------------------------------------------------------------------------*/
/* don't decode vch                                                           */
/*----------------------------------------------------------------------------*/
#ifdef _ROWBOX_BASED

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

#else // ???????????????????????????????????????????????????????????????????????

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

#endif

/*----------------------------------------------------------------------------*/
/* hash a partion key column into running hash                                */
/*----------------------------------------------------------------------------*/
#ifdef _ROWBOX_BASED

byte* RowSingle::ayDstByte(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  byte* ayDst = (byte*) & m_awaColVal[0].m_iWrd;
  
  return ayDst;
}

#else // ???????????????????????????????????????????????????????????????????????

byte* RowSingle::ayDstByte(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  byte* ayDst = (byte*) & m_awaColVal[0].m_iWrd;
  
  return ayDst;
}

#endif

/*----------------------------------------------------------------------------*/
/* hash a partion key column into running hash                                */
/*----------------------------------------------------------------------------*/
#ifdef _ROWBOX_BASED

bool RowSingle::bIsColNull(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  bool bIsNull = m_awaColVal[i_Col].bIsNull();
  
  return bIsNull;
}

#else // ???????????????????????????????????????????????????????????????????????

bool RowSingle::bIsColNull(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  bool bIsNull = m_awaColVal[i_Col].bIsNull();
  
  return bIsNull;
}

#endif

/*----------------------------------------------------------------------------*/
/* reset all column values                                                    */
/*----------------------------------------------------------------------------*/
#ifdef _ROWBOX_BASED

void RowSingle::vResetValues(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
    
#ifdef WITH_CHECK_INITIALISED
  for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++)
    m_awaColVal[i_Col].m_iWrd = 0;
#endif
  
  m_xWsp.vInit();
}

#else // ???????????????????????????????????????????????????????????????????????

void RowSingle::vResetValues(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
    
#ifdef WITH_CHECK_INITIALISED
  for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++)
    m_awaColVal[i_Col].m_iWrd = 0;
#endif
  
  m_xWsp.vInit();
}

#endif

/*----------------------------------------------------------------------------*/
/* set all the column of this row single                                      */
/*----------------------------------------------------------------------------*/
#ifdef _ROWBOX_BASED

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

#else // ???????????????????????????????????????????????????????????????????????

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

#endif

/*----------------------------------------------------------------------------*/
/* set column value to be null                                                */
/*----------------------------------------------------------------------------*/
#ifdef _ROWBOX_BASED

void RowSingle::vSetNull(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  m_awaColVal[i_Col].m_iWrd = C_DATUM_NULL;
}

#else // ???????????????????????????????????????????????????????????????????????

void RowSingle::vSetNull(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  m_awaColVal[i_Col].m_iWrd = C_DATUM_NULL;
}

#endif

/*----------------------------------------------------------------------------*/
/* set column value in row                                                    */
/*----------------------------------------------------------------------------*/
#ifdef _ROWBOX_BASED

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

#else // ???????????????????????????????????????????????????????????????????????

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

#endif

/*----------------------------------------------------------------------------*/
/* get size of row (needed for MiniHeap_t<ITEM>::aAdjacentItem(ITEM* aItem))  */
/*----------------------------------------------------------------------------*/
#ifdef _ROWBOX_BASED

i8 RowSingle::nSizeBytes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  return sizeof(RowSingle);
}

#else // ???????????????????????????????????????????????????????????????????????

i8 RowSingle::nSizeBytes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  return sizeof(RowSingle);
}

#endif

/*----------------------------------------------------------------------------*/
/* get size of row data                                                       */
/*----------------------------------------------------------------------------*/
#ifdef _ROWBOX_BASED

i8 RowSingle::nSizeRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  i8 nSizeBytes = m_pStreamDef->nColumns(C_ALL) * sizeof(i8);
  
  return nSizeBytes;
}

#else // ???????????????????????????????????????????????????????????????????????

i8 RowSingle::nSizeRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  i8 nSizeBytes = m_pStreamDef->nColumns(C_ALL) * sizeof(i8);
  
  return nSizeBytes;
}

#endif

/*----------------------------------------------------------------------------*/
/* get size of row data                                                       */
/*----------------------------------------------------------------------------*/
#ifdef _ROWBOX_BASED

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

#else // ???????????????????????????????????????????????????????????????????????

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

#endif

/*----------------------------------------------------------------------------*/
/* get string area                                                            */
/*----------------------------------------------------------------------------*/
#ifdef _ROWBOX_BASED

Stz* RowSingle::szStzArea(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);

  return (Stz*)m_xWsp.ayArea();
}

#else // ???????????????????????????????????????????????????????????????????????

Stz* RowSingle::szStzArea(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);

  return (Stz*)m_xWsp.ayArea();
}

#endif

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
