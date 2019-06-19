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
/* perform new of row buffer for given address                                */
/*----------------------------------------------------------------------------*/
void* Row::operator new(size_t, void* aAddress)
{
  return aAddress;
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
  m_iBaseOffset = RowCompactor::iBaseOffset(m_pColVec);
  m_bGetAll     = true;
  for (i8 i_Col = 0; i_Col < C_MAX_COLUMNS; i_Col++)
    m_abGetCol[i_Col] = false;
  m_iLastCol    = C_UNKNOWN;
  m_i_Wsp       = 0;
  
  // - - - - - - - - - - - - - - - - - -

  m_iHash = 0;
  
#ifdef WITH_CHECK_INITIALISED
  for (i8 i_Col = 0; i_Col < C_MAX_COLUMNS; i_Col++)
    m_iaColVal[i_Col] = 0;
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
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // vch
  
  if (DM_VCH == pCol->eDatumType()) {
    
    if (pDatum->bIsNull()) {
      m_iaColVal[i_Col] = C_DATUM_NULL;
      return;
    }
    
    char* acDstString = (char*)m_xWsp.ayArea() + m_i_Wsp;
    char* acSrcString = (char*)pDatum->m_sVch.aChar();

    i8 nSrcLength = StrLib::nCopy(acDstString,acSrcString);

    m_iaColVal[i_Col] = (i8)acDstString;
    
    m_i_Wsp += (nSrcLength + 1);
    
    if (pCol->bIsPartitionKey())
      m_iHash += iHashColumn(pCol);
    
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // all others
  
  m_iaColVal[i_Col] = pDatum->m_iInt;

  if (pCol->bIsPartitionKey())
    m_iHash += iHashColumn(pCol);
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
/* hash a partion key column into running hash                                */
/*----------------------------------------------------------------------------*/
i8 Row::iHashColumn(Column* pCol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
  
  i8 i_Col = pCol->iColumnNo();
  
  i8 iHash;
  if (DM_VCH == pCol->eDatumType())
    iHash = HshLib::iHash((byte*)m_iaColVal[i_Col],sizeof(w8));
  else
    iHash = HshLib::iHash((byte*)&m_iaColVal[i_Col],sizeof(w8));
  
  return iHash;
}

/*----------------------------------------------------------------------------*/
/* reset all column values                                                    */
/*----------------------------------------------------------------------------*/
void Row::vResetValues(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
  
  m_i_Wsp = 0;
  
  // - - - - - - - - - - - - - - - - - -

  m_iHash = 0;
  
#ifdef WITH_CHECK_INITIALISED
  for (i8 i_Col = 0; i_Col < m_nColumns; i_Col++)
    m_iaColVal[i_Col] = 0;  
#endif
}

/*----------------------------------------------------------------------------*/
/* get size of row                                                            */
/*----------------------------------------------------------------------------*/
i8 Row::nSizeBytes(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROW,_FOOTER_);
  
  return sizeof(Row);
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
