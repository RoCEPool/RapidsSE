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
: RowWsp(CN_ROWSINGLE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  m_pStreamDef  = pStreamDef;
  m_nColumns    = pStreamDef->nColumns(iSubSet);
  m_iSubSet     = iSubSet;
  
  m_bGetAll     = true;
  
  m_tPerspective = C_UNKNOWN;
  for (i8 i_Col = 0; i_Col < C_MAX_COLUMNS; i_Col++)
    m_abGetCol[i_Col] = false;
  m_iLastCol    = C_UNKNOWN;
  
  // - - - - - - - - - - - - - - - - - -

  m_iHash = 0;
  
#ifdef WITH_CHECK_INITIALISED
  for (i8 i_Col = 0; i_Col < C_MAX_COLUMNS; i_Col++)
    m_waColVal[i_Col].m_iWrd = 0;
  m_xStzWsp.vZero(CN_ROWSINGLE);
  
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
/* adjust addresses                                                           */
/*----------------------------------------------------------------------------*/
void RowSingle::vAdjustAddresses(RowSingle* pCopiedFromRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++) {
    
    Column* pCol = m_pStreamDef->pGetColumn(i_Col);
    
    if (DM_VCH != pCol->eDatum())
      continue;

    if (m_waColVal[i_Col].bIsNull())
      continue;
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    i8 nDeltaBytes = nDifference(m_waColVal[i_Col].m_szVch,pCopiedFromRow);
    
    m_waColVal[i_Col].m_szVch = aAdd<Stz*>(this,nDeltaBytes);
  }
}

/*----------------------------------------------------------------------------*/
/* set column value in row                                                    */
/*----------------------------------------------------------------------------*/
void RowSingle::vAllotSetVarchar(i8 i_Col, Stz* szVal)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
      
  char* acDstString = (char*)m_xStzWsp.ayLatter();
  
  char* acSrcString = (char*)szVal;
  
  i8 nSrcLength = StrLib::nCopy(acDstString,acSrcString);
  
  m_waColVal[i_Col].m_iWrd = (i8)acDstString;
  
  m_xStzWsp.vTake(nSrcLength + 1);
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
      
      if (pRow->m_waColVal[i_Col].bIsNull()) {
        m_waColVal[i_Col].vSetNull();
        continue;
      }
      
      char* acSrcString = (char*)pRow->szStzGet(i_Col);
      vAllotSetVarchar(i_Col, (Stz*)acSrcString);
      
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // all others
    
    m_waColVal[i_Col].m_iWrd = pRow->m_waColVal[i_Col].m_iWrd;
  }
}

/*----------------------------------------------------------------------------*/
/* append column value to row                                                 */
/*----------------------------------------------------------------------------*/
void RowSingle::vCopy(Row* pRow, i8 i_FromCol, i8 i_IntoCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_)
    
    Column* pIntoCol = m_pStreamDef->pGetColumn(i_IntoCol);
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // vch
    
    if (DM_VCH == pIntoCol->eDatum()) {
      
      if (pRow->m_waColVal[i_FromCol].bIsNull()) {
        m_waColVal[i_IntoCol].vSetNull();
        return;
      }
      
      char* acSrcString = (char*)pRow->m_waColVal[i_FromCol].m_azStr;
      vAllotSetVarchar(i_IntoCol, (Stz*)acSrcString);
      
      return;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // all others
    
    m_waColVal[i_IntoCol].m_iWrd = pRow->m_waColVal[i_FromCol].m_iWrd;
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
i8 RowSingle::iHashColumn(Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  i8 i_Col = pCol->iColumnNo();
  
  i8 iHash;
  if (DM_VCH == pCol->eDatum())
    iHash = HshLib::iHash((byte*)m_waColVal[i_Col].m_iWrd,sizeof(w8));
  else
    iHash = HshLib::iHash((byte*)&m_waColVal[i_Col].m_iWrd,sizeof(w8));
  
  return iHash;
}

/*----------------------------------------------------------------------------*/
/* reset all column values                                                    */
/*----------------------------------------------------------------------------*/
void RowSingle::vResetValues(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWSINGLE,_FOOTER_);
  
  m_iHash = 0;
  
#ifdef WITH_CHECK_INITIALISED
  for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++)
    m_waColVal[i_Col].m_iWrd = 0;
#endif
  
  m_xStzWsp.vInit();
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
        m_waColVal[i_Col].vSetNull();
    }
    else {
      i8 iVal = va_arg(xVarArgs,i8);
      m_waColVal[i_Col].m_iWrd = iVal;
    }
    
    if (pCol->bIsPtnKey())
      m_iHash += iHashColumn(pCol);
  }
  
  i8 iVArgsEnd = va_arg(xVarArgs,i8);
  ASSERT(C_VARGS_END == iVArgsEnd);

  va_end(xVarArgs);
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
      m_waColVal[i_Col].vSetNull();
      return;
    }
    
    char* acDstString = (char*)m_xStzWsp.ayLatter();
    
    char* acSrcString = (char*)pDatum->azChar();
    
    i8 nSrcLength = StrLib::nCopy(acDstString,acSrcString);
    
    m_waColVal[i_Col].m_iWrd = (i8)acDstString;
    
    m_xStzWsp.vTake(nSrcLength + 1);
    
    if (pCol->bIsPtnKey())
      m_iHash += iHashColumn(pCol);
    
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // all others
  
  m_waColVal[i_Col].m_iWrd = pDatum->iInt();
  
  if (pCol->bIsPtnKey())
    m_iHash += iHashColumn(pCol);
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
      
      if (m_waColVal[i_Col].bIsNull()) {
        continue;
      }
      
      char* acSrcString = (char*)m_waColVal[i_Col].m_szVch;
      
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
    
  return (Stz*)m_xStzWsp.ayArea();
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
