/*============================================================================*/
/* RowBox.cpp -- Container of a single Row                                    */
/*============================================================================*/

#include "Rows.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
RowBox* Rows::_pRowBox(void* aAddr)
{
  RowBox* pRowBox = reinterpret_cast<RowBox*>(aAddr);
  
  return pRowBox;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* RowBox::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_ROWBOX);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* perform new of row buffer for given address                                */
/*----------------------------------------------------------------------------*/
void* RowBox::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void RowBox::operator delete(void* aAddr)
{
  RowBox* pRowBox = reinterpret_cast<RowBox*>(aAddr);
  
  HeapMem::vFree(pRowBox);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowBox::RowBox(StreamDef* pStreamDef, i8 iSubSet)
: RowWsp(CN_ROWBOX)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBOX,_FOOTER_);
  
  m_pStreamDef  = pStreamDef;
  m_nColumns    = pStreamDef->nColumns(iSubSet);
  m_iSubSet     = iSubSet;
  
  m_bGetAll     = true;
  
  for (i8 i_Col = 0; i_Col < C_MAX_COLUMNS; i_Col++)
    m_abGetCol[i_Col] = false;
  m_iLastCol    = C_UNKNOWN;
  
  // - - - - - - - - - - - - - - - - - -
  
  m_awaColVal = (Dtm*)m_xWsp.ayAllotTail(m_nColumns * sizeof(Dtm));
  
#ifdef WITH_CHECK_INITIALISED
  for (i8 i_Col = 0; i_Col < C_MAX_COLUMNS; i_Col++)
    m_awaColVal[i_Col].m_iWrd = 0;
  m_xStzWsp.vZero(CN_ROWBOX);
  
#else
  // m_xStzWsp
#endif
  
  _checkInitialised_(CN_ROWBOX);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWBOX,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RowBox::~RowBox(void) noexcept(false)
{
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* adjust addresses                                                           */
/*----------------------------------------------------------------------------*/
void RowBox::vAdjustAddresses(RowBox* pCopiedFromRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBOX,_FOOTER_);
  
  for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++) {
    
    Column* pCol = m_pStreamDef->pGetColumn(i_Col);
    
    if (DM_VCH != pCol->eDatum())
      continue;

    if (m_awaColVal[i_Col].bIsNull())
      continue;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    i8 nDeltaBytes = nDifference(m_awaColVal[i_Col].m_szVch,pCopiedFromRow);
    
    m_awaColVal[i_Col].m_szVch = aAdd<Stz*>(this,nDeltaBytes);
  }
}

/*----------------------------------------------------------------------------*/
/* set column value in row                                                    */
/*----------------------------------------------------------------------------*/
void RowBox::vAllotSetVarchar(i8 i_Col, Stz* szVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBOX,_FOOTER_);
      
  char* acDstString = (char*)m_xWsp.ayLatterHead();
  
  char* acSrcString = (char*)szVal;
  
  i8 nSrcLength = StrLib::nCopy(acDstString,acSrcString);
  
  m_awaColVal[i_Col].m_iWrd = (i8)acDstString;
  
  m_xWsp.vTakeHead(nSrcLength + 1);
}

/*----------------------------------------------------------------------------*/
/* get address of column datum                                                */
/*----------------------------------------------------------------------------*/
Dtm* RowBox::aColDtm(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBOX,_FOOTER_);
  
  Dtm* aDtm = & m_awaColVal[i_Col];
  
  return aDtm;
}


/*----------------------------------------------------------------------------*/
/* get column value                                                           */
/*----------------------------------------------------------------------------*/
char* RowBox::azColVal(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBOX,_FOOTER_);
  
  char* azVal = m_awaColVal[i_Col].m_azStr;
  
  return azVal;
}

/*----------------------------------------------------------------------------*/
/* get column value                                                           */
/*----------------------------------------------------------------------------*/
i8 RowBox::iColVal(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBOX,_FOOTER_);
  
  i8 iVal = m_awaColVal[i_Col].m_iWrd;
  
  return iVal;
}

/*----------------------------------------------------------------------------*/
/* get column value                                                           */
/*----------------------------------------------------------------------------*/
d8 RowBox::dColVal(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBOX,_FOOTER_);
  
  d8 dVal = m_awaColVal[i_Col].m_dWrd;
  
  return dVal;
}

/*----------------------------------------------------------------------------*/
/* get column value                                                           */
/*----------------------------------------------------------------------------*/
Stz* RowBox::szColVal(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBOX,_FOOTER_);
  
  Stz* szVal = m_awaColVal[i_Col].m_szVch;
  
  return szVal;
}

/*----------------------------------------------------------------------------*/
/* get column value                                                           */
/*----------------------------------------------------------------------------*/
Dtm RowBox::wColVal(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBOX,_FOOTER_);
  
  Dtm wDtm = m_awaColVal[i_Col];
  
  return wDtm;
}

/*----------------------------------------------------------------------------*/
/* append column value to row                                                 */
/*----------------------------------------------------------------------------*/
void RowBox::vCopy(Row* pRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBOX,_FOOTER_);
  
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
void RowBox::vDontDecodeVch(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBOX,_FOOTER_);

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
byte* RowBox::ayDstByte(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBOX,_FOOTER_);
  
  byte* ayDst = (byte*) & m_awaColVal[0].m_iWrd;
  
  return ayDst;
}

/*----------------------------------------------------------------------------*/
/* hash a partion key column into running hash                                */
/*----------------------------------------------------------------------------*/
bool RowBox::bIsColNull(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBOX,_FOOTER_);
  
  bool bIsNull = m_awaColVal[i_Col].bIsNull();
  
  return bIsNull;
}

/*----------------------------------------------------------------------------*/
/* reset all column values                                                    */
/*----------------------------------------------------------------------------*/
void RowBox::vResetValues(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBOX,_FOOTER_);
    
#ifdef WITH_CHECK_INITIALISED
  for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++)
    m_awaColVal[i_Col].m_iWrd = 0;
#endif
  
  m_xWsp.vInit();
}

/*----------------------------------------------------------------------------*/
/* set all the column of this row single                                      */
/*----------------------------------------------------------------------------*/
void RowBox::vSet(i8 iVArgsBegin, ...)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBOX,_FOOTER_);
  
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
void RowBox::vSetNull(i8 i_Col)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBOX,_FOOTER_);
  
  m_awaColVal[i_Col].m_iWrd = C_DATUM_NULL;
}

/*----------------------------------------------------------------------------*/
/* set column value in row                                                    */
/*----------------------------------------------------------------------------*/
void RowBox::vSetValue(Datum* pDatum, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBOX,_FOOTER_);
  
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
i8 RowBox::nSizeBytes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBOX,_FOOTER_);
  
  return sizeof(RowBox);
}

/*----------------------------------------------------------------------------*/
/* get size of row data                                                       */
/*----------------------------------------------------------------------------*/
i8 RowBox::nSizeRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBOX,_FOOTER_);
  
  i8 nSizeBytes = m_pStreamDef->nColumns(C_ALL) * sizeof(i8);
  
  return nSizeBytes;
}

/*----------------------------------------------------------------------------*/
/* get size of row data                                                       */
/*----------------------------------------------------------------------------*/
i8 RowBox::nSizeVch(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBOX,_FOOTER_);
  
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
Stz* RowBox::szStzArea(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBOX,_FOOTER_);
    
  return (Stz*)m_xWsp.ayArea();
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* RowBox::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWBOX);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
