/*============================================================================*/
/* RowCounts.cpp -- Wire Line Recv                                         */
/*============================================================================*/

#include "BASE.h"

#include "RowCounts.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, RowCounts, CN_ROWCOUNTS)
__aNewFunction(RowCounts, CN_ROWCOUNTS)
__vDeleteFunction(RowCounts, CN_ROWCOUNTS)
__aFooterFunction(RowCounts)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowCounts::RowCounts(void)
: Object(CN_ROWCOUNTS)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_ROWCOUNTS, _FOOTER_);

  m_nRowsProcessed = 0;
  m_nRowsSkipped   = 0;
  m_nRowsInvalid   = 0;
  m_nRowsInserted  = 0;
  m_nRowsUpdated   = 0;
  m_nRowsDeleted   = 0;

  _checkInitialised_(CN_ROWCOUNTS);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_ROWCOUNTS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RowCounts::~RowCounts(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_ROWCOUNTS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* repair the aggregate time                                                  */
/*----------------------------------------------------------------------------*/
void RowCounts::vRepair(Object* pObject)
{
  _static_

  RowCounts* pRowCounts = _pRowCounts(pObject);

  delete pRowCounts;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Accumulate the count of rows processed                                     */
/*----------------------------------------------------------------------------*/
void RowCounts::vAddRowsProcessed(i8 nProcessed)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOUNTS,_FOOTER_);
  m_nRowsProcessed += nProcessed;
}

/*----------------------------------------------------------------------------*/
/* Accumulate the count of rows skipped                                       */
/*----------------------------------------------------------------------------*/
void RowCounts::vAddRowsSkipped(i8 nSkipped)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOUNTS,_FOOTER_);
  m_nRowsSkipped += nSkipped;
}

/*----------------------------------------------------------------------------*/
/* Accumulate the count of rows that are invalid                              */
/*----------------------------------------------------------------------------*/
void RowCounts::vAddRowsInvalid(i8 nInvalid)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOUNTS,_FOOTER_);
  m_nRowsInvalid += nInvalid;
}

/*----------------------------------------------------------------------------*/
/* Accumulate the count of rows inserted                                      */
/*----------------------------------------------------------------------------*/
void RowCounts::vAddRowsInserted(i8 nInserted)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOUNTS,_FOOTER_);
  m_nRowsInserted += nInserted;
}

/*----------------------------------------------------------------------------*/
/* Accumulate the count of rows updated                                       */
/*----------------------------------------------------------------------------*/
void RowCounts::vAddRowsUpdated(i8 nUpdated)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOUNTS,_FOOTER_);
  m_nRowsUpdated += nUpdated;
}

/*----------------------------------------------------------------------------*/
/* Accumulate the count of rows deleted                                       */
/*----------------------------------------------------------------------------*/
void RowCounts::vAddRowsDeleted(i8 nDeleted)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOUNTS,_FOOTER_);
  m_nRowsDeleted += nDeleted;
}


/*----------------------------------------------------------------------------*/
/* Get the count of rows processed                                            */
/*----------------------------------------------------------------------------*/
i8 RowCounts::nRowsProcessed(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOUNTS,_FOOTER_);
  return m_nRowsProcessed;
}

/*----------------------------------------------------------------------------*/
/* Get the count of rows skipped                                              */
/*----------------------------------------------------------------------------*/
i8 RowCounts::nRowsSkipped(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOUNTS,_FOOTER_);
  return m_nRowsSkipped;
}

/*----------------------------------------------------------------------------*/
/* Get the count of rows found to be invalid                                  */
/*----------------------------------------------------------------------------*/
i8 RowCounts::nRowsInvalid(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOUNTS,_FOOTER_);
  return m_nRowsInvalid;
}

/*----------------------------------------------------------------------------*/
/* Get the count of rows inserted                                             */
/*----------------------------------------------------------------------------*/
i8 RowCounts::nRowsInserted(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOUNTS,_FOOTER_);
  return m_nRowsInserted;
}

/*----------------------------------------------------------------------------*/
/* Get the count of rows updated                                              */
/*----------------------------------------------------------------------------*/
i8 RowCounts::nRowsUpdated(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOUNTS,_FOOTER_);
  return m_nRowsUpdated;
}

/*----------------------------------------------------------------------------*/
/* Get the count of rows deleted                                              */
/*----------------------------------------------------------------------------*/
i8 RowCounts::nRowsDeleted(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWCOUNTS,_FOOTER_);
  return m_nRowsDeleted;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/