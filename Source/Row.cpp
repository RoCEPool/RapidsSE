/*============================================================================*/
/* Row.cpp -- Computable form of Row                                          */
/*============================================================================*/

#include "Transform.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Row* Transform::_pRow(void* aAddr)
{
  Row* pRow = reinterpret_cast<Row*>(aAddr);
  
  return pRow;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Row::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_ROW);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Row::operator delete(void* aAddr)
{
  Row* pRow = reinterpret_cast<Row*>(aAddr);
  
  HeapMem::vFree(pRow);
}


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Row::Row(ColumnVec* pColVec)
: Object(CN_ROW)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
  
  m_pColVec     = pColVec;
  m_nColumns    = m_pColVec->nItems();
  m_iBaseOffset = RowCoder::iBaseOffset(m_pColVec);
  m_bGetAll     = true;
  m_iLastCol    = C_UNKNOWN;
  
  for (i8 i_Col = 0; i_Col < C_MAX_COLUMNS; i_Col++)
    m_abGetCol[i_Col] = false;
  m_i_Wsp       = 0;
  
#ifdef WITH_CHECK_INITIALISED
  for (i8 i_Col = 0; i_Col < C_MAX_COLUMNS; i_Col++)
    m_aiValue[i_Col] = 0;
  
  m_xWsp.vZero(CN_ROW);
  
#else
  // m_xWsp
#endif
  
  _checkInitialised_(CN_ROW);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Row::~Row(void) noexcept(false)
{
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* append column value to row                                                 */
/*----------------------------------------------------------------------------*/
void Row::vAppendValue(Data::Datum* pDatum, Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);

  i8 i_Col = pCol->iColumnNo();
  
  if (DM_VCH == pCol->eDatumType()) {
    
    char* acDstString = (char*)m_xWsp.ayArea() + m_i_Wsp;
    char* acSrcString = (char*)pDatum->m_sVch.aChar();

    i8 nSrcLength = StrLib::nCopy(acDstString,acSrcString);

    m_i_Wsp += (nSrcLength + 1);
    
    return;
  }
  
  m_aiValue[i_Col] = pDatum->m_iInt;
}

/*----------------------------------------------------------------------------*/
/* don't decode vch                                                           */
/*----------------------------------------------------------------------------*/
void Row::vDontDecodeVch(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);

  m_bGetAll = false;
  
  for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++) {
    
    Column* pCol = m_pColVec->pGet(i_Col);
    
    if (DM_VCH == pCol->eDatumType()) {
      m_abGetCol[i_Col] = false;
    }
    else
      m_abGetCol[i_Col] = true;
    m_iLastCol = i_Col;
  }
}

/*----------------------------------------------------------------------------*/
/* reset all column values                                                    */
/*----------------------------------------------------------------------------*/
void Row::vResetValues(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
  
  m_i_Wsp = 0;
  
#ifdef WITH_CHECK_INITIALISED
  for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++) {
    m_aiValue[i_Col] = 0;
  }
  
#endif
}



/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Row::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
