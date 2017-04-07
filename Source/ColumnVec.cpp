/*============================================================================*/
/* ColumnVec.cpp - Column Vector                                              */
/*============================================================================*/

#include "DBobjects.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
ColumnVec* DBobjects::_pColumnVec(void* aAddr)
{
  ColumnVec* pColumnVec = reinterpret_cast<ColumnVec*>(aAddr);
  
  RASSERT( !pColumnVec || CN_COLUMNVEC == pColumnVec->eClass() );
  
  return pColumnVec;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* ColumnVec::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  void* pObject = Heap::pAllocate(CN_COLUMNVEC);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void ColumnVec::operator delete(void* pDataAddress)
{
  Data* pData = reinterpret_cast<Data*>(pDataAddress);
  
  Heap::vFree(pData);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ColumnVec::ColumnVec(void)
: Object(CN_COLUMNVEC)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMNVEC,_FOOTER_);
  
  m_nColumns = 0;
  
  for (i8 iColumn = 0; iColumn < C_COLUMN_VEC; iColumn++)
    m_pColumn[iColumn] = nullptr;
  
  _setFooter_();
  
  _checkInitialised_(CN_COLUMNVEC);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_COLUMNVEC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ColumnVec::~ColumnVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMNVEC,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add a column                                                               */
/*----------------------------------------------------------------------------*/
void ColumnVec::vAddColumn(Column* pColumn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMNVEC,_FOOTER_);
  
  // check we have a free slot
  ASSERT(m_nColumns >= 0 && m_nColumns < C_COLUMN_VEC - 1);
  
  // get column number
  i8 iColumn = m_nColumns++;
  ASSERT(m_nColumns > 0 && m_nColumns < C_COLUMN_VEC);
  
  // there shouldnt already be a column
  ASSERT( ! m_pColumn[iColumn] );
  
  // set column
  m_pColumn[iColumn] = pColumn;
}

/*----------------------------------------------------------------------------*/
/* get number of actual columns                                               */
/*----------------------------------------------------------------------------*/
i8 ColumnVec::nColumns(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMNVEC,_FOOTER_);

  ASSERT(m_nColumns >= 0 && m_nColumns < C_COLUMN_VEC);
  
  return m_nColumns;
}

/*----------------------------------------------------------------------------*/
/* get a specific column                                                      */
/*----------------------------------------------------------------------------*/
Column* ColumnVec::pColumn(i8 iColumn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMNVEC,_FOOTER_);
 
  ASSERT(iColumn >= 0 && iColumn < m_nColumns);
  
  Column* pColumn = m_pColumn[iColumn];
  
  return pColumn;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* ColumnVec::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLUMNVEC);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
