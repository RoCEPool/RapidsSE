/*============================================================================*/
/* Row2.cpp -- Computable form of Row2                                        */
/*============================================================================*/

#include "Rows.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Row2* Rows::_pRow2(void* aAddr)
{
  Row2* pRow = reinterpret_cast<Row2*>(aAddr);
  
  return pRow;
}

/*----------------------------------------------------------------------------*/
/* perform new of row buffer for given address                                */
/*----------------------------------------------------------------------------*/
void* Row2::operator new(size_t, void* aAddress)
{
  return aAddress;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Row2::Row2(void)
: Object(CN_ROW)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Row2::~Row2(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* adjust addresses                                                           */
/*----------------------------------------------------------------------------*/
//void Row2::vAdjustAddresses(RowSingle* pCopiedFromRow)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
//
//  for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++) {
//
//    Column* pCol = m_pStreamDef->pGetColumn(i_Col);
//
//    if (DM_VCH != pCol->eDatum())
//      continue;
//
//    if (m_waColVal[i_Col].bIsNull())
//      continue;
//
//    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//    i8 nDeltaBytes = nSub(m_waColVal[i_Col].m_szVch,pCopiedFromRow);
//
//    m_waColVal[i_Col].m_szVch = aAdd<Stz*>(this,nDeltaBytes);
//  }
//}

/*----------------------------------------------------------------------------*/
/* append column value to row                                                 */
/*----------------------------------------------------------------------------*/
//void Row2::vCopy(Row* pRow)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
//
//  for (i8 i_Col = 0; i_Col < m_pStreamDef->nColumns(C_ALL); i_Col++) {
//
//    Column* pCol = m_pStreamDef->pGetColumn(i_Col);
//    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//    // vch
//
//    if (DM_VCH == pCol->eDatum()) {
//
//      if (pRow->m_waColVal[i_Col].bIsNull()) {
//        m_waColVal[i_Col].vSetNull();
//        continue;
//      }
//
//      char* acDstString = (char*)m_xWsp.ayLatter();
//      char* acSrcString = (char*)pRow->m_waColVal[i_Col].m_szVch;
//
//      i8 nSrcLength = StrLib::nCopy(acDstString,acSrcString);
//
//      m_waColVal[i_Col].m_iWrd = (i8)acDstString;
//
//      m_xWsp.vTake(nSrcLength + 1);
//
//      continue;
//    }
//
//    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//    // all others
//
//    m_waColVal[i_Col].m_iWrd = pRow->m_waColVal[i_Col].m_iWrd;
//  }
//}

/*----------------------------------------------------------------------------*/
/* append column value to row                                                 */
/*----------------------------------------------------------------------------*/
//void Row2::vCopy(StreamDef* pStreamDef, RowSingle* pRow)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
//
//  for (i8 i_Col = 0; i_Col < pStreamDef->nColumns(C_ALL); i_Col++) {
//
//    Column* pCol = pStreamDef->pGetColumn(i_Col);
//    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//    // vch
//
//    if (DM_VCH == pCol->eDatum()) {
//
//      if (pRow->m_waColVal[i_Col].bIsNull()) {
//        m_iValue[i_Col].vSetNull();
//        continue;
//      }
//
//      char* acDstString = ;
//      char* acSrcString = (char*)pRow->m_waColVal[i_Col].m_szVch;
//
//      i8 nSrcLength = StrLib::nCopy(acDstString,acSrcString);
//
//      m_waColVal[i_Col].m_iWrd = (i8)acDstString;
//
//      m_xWsp.vTake(nSrcLength + 1);
//
//      continue;
//    }
//
//    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//    // all others
//
//    m_waColVal[i_Col].m_iWrd = pRow->m_waColVal[i_Col].m_iWrd;
//  }
//}

/*----------------------------------------------------------------------------*/
/* append column value to row                                                 */
/*----------------------------------------------------------------------------*/
//void Row2::vCopy(Row* pRow, i8 i_FromCol, i8 i_IntoCol)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_)
//
//    Column* pIntoCol = m_pStreamDef->pGetColumn(i_IntoCol);
//    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//    // vch
//
//    if (DM_VCH == pIntoCol->eDatum()) {
//
//      if (pRow->m_waColVal[i_FromCol].bIsNull()) {
//        m_waColVal[i_IntoCol].vSetNull();
//        return;
//      }
//
//      char* acDstString = (char*)m_xWsp.ayLatter();
//      char* acSrcString = (char*)pRow->m_waColVal[i_FromCol].m_szVch;
//
//      i8 nSrcLength = StrLib::nCopy(acDstString,acSrcString);
//
//      m_waColVal[i_IntoCol].m_iWrd = (i8)acDstString;
//
//      m_xWsp.vTake(nSrcLength + 1);
//
//      return;
//    }
//
//    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//    // all others
//
//    m_waColVal[i_IntoCol].m_iWrd = pRow->m_waColVal[i_FromCol].m_iWrd;
//}

/*----------------------------------------------------------------------------*/
/* don't decode vch                                                           */
/*----------------------------------------------------------------------------*/
//void Row2::vDontDecodeVch(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
//
//  m_bGetAll = false;
//
//  for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++) {
//
//    Column* pCol = m_pStreamDef->pGetColumn(i_Col);
//
//    if (DM_VCH == pCol->eDatum()) {
//      m_abGetCol[i_Col] = false;
//    }
//    else
//      m_abGetCol[i_Col] = true;
//    m_iLastCol = i_Col;
//  }
//}

/*----------------------------------------------------------------------------*/
/* hash a partion key column into running hash                                */
/*----------------------------------------------------------------------------*/
//i8 Row2::iHashColumn(Column* pCol)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
//
//  i8 i_Col = pCol->iColumnNo();
//
//  i8 iHash;
//  if (DM_VCH == pCol->eDatum())
//    iHash = HshLib::iHash((byte*)m_waColVal[i_Col].m_iWrd,sizeof(w8));
//  else
//    iHash = HshLib::iHash((byte*)&m_waColVal[i_Col].m_iWrd,sizeof(w8));
//
//  return iHash;
//}

/*----------------------------------------------------------------------------*/
/* reset all column values                                                    */
/*----------------------------------------------------------------------------*/
//void Row2::vResetValues(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
//
//  m_iHash = 0;
//
//#ifdef WITH_CHECK_INITIALISED
//  for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++)
//    m_waColVal[i_Col].m_iWrd = 0;
//#endif
//
//  m_xWsp.vInit();
//}

/*----------------------------------------------------------------------------*/
/* set column value in row                                                    */
/*----------------------------------------------------------------------------*/
//void Row2::vSetValue(Datum* pDatum, Column* pCol)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
//
//  i8 i_Col = pCol->iColumnNo();
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // vch
//
//  if (DM_VCH == pCol->eDatum()) {
//
//    if (pDatum->bIsNull()) {
//      m_waColVal[i_Col].vSetNull();
//      return;
//    }
//
//    char* acDstString = (char*)m_xWsp.ayLatter();
//
//    char* acSrcString = (char*)pDatum->azChar();
//
//    i8 nSrcLength = StrLib::nCopy(acDstString,acSrcString);
//
//    m_waColVal[i_Col].m_iWrd = (i8)acDstString;
//
//    m_xWsp.vTake(nSrcLength + 1);
//
//    if (pCol->bIsPtnKey())
//      m_iHash += iHashColumn(pCol);
//
//    return;
//  }
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // all others
//
//  m_waColVal[i_Col].m_iWrd = pDatum->iInt();
//
//  if (pCol->bIsPtnKey())
//    m_iHash += iHashColumn(pCol);
//}

/*----------------------------------------------------------------------------*/
/* get size of row (needed for MiniHeap_t<ITEM>::aAdjacentItem(ITEM* aItem))  */
/*----------------------------------------------------------------------------*/
i8 Row2::nSizeBytes(StreamDef* pStreamDef)
{
  i8 nSize = pStreamDef->nColumns(0) * sizeof (m_iValue) + sizeof(Object);
  return sizeof(nSize);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Row2::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
