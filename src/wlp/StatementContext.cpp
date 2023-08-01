/*============================================================================*/
/* StatementContext.hpp - State of the current statement for the AdaptorWL    */
/*============================================================================*/

#include "BASE.h"
#include "StatementContext.hpp"

using namespace WLP;
/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(WLP,StatementContext,CN_STATEMENTCONTEXT)
__aNewFunction(StatementContext,CN_STATEMENTCONTEXT)
__vDeleteFunction(StatementContext,CN_STATEMENTCONTEXT)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
StatementContext::StatementContext(Stz* szStmt, Stz* szId)
: Object(CN_STATEMENTCONTEXT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STATEMENTCONTEXT,_FOOTER_);

  m_sStmtBuf.vCopy(szStmt);
  m_sStmtId  = szId;
  m_bGotStmtError = false;
  m_pStreamDef = nullptr;

  m_iRowsProcessed = 0;
  m_iRowsSkipped = 0;
  m_iRowsInvalid = 0;
  m_iRowsSelected = 0;
  m_iRowsInserted = 0;
  m_iRowsUpdated = 0;
  m_iRowsDeleted = 0;

  _postcondition_(_ADDRESS_,_MAGIC_,CN_STATEMENTCONTEXT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
StatementContext::~StatementContext(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STATEMENTCONTEXT,_FOOTER_);
  delete m_pStreamDef;
  m_pStreamDef = nullptr;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Return the statement text                                                  */
/*----------------------------------------------------------------------------*/
Stz* StatementContext::szStmt()
{
  return m_sStmtBuf.szStz();
}

/*----------------------------------------------------------------------------*/
/* Return the statement identifier                                            */
/*----------------------------------------------------------------------------*/
Stz* StatementContext::szStmtId()
{
  return m_sStmtId.szStz();
}

/*----------------------------------------------------------------------------*/
/* Return whether an error has been seen in the execution of this statement   */
/*----------------------------------------------------------------------------*/
bool StatementContext::bGotError()
{
  return m_bGotStmtError;
}

/*----------------------------------------------------------------------------*/
/* Return whether an error has been seen in the execution of this statement   */
/*----------------------------------------------------------------------------*/
void StatementContext::vGotError(bool bError)
{
  m_bGotStmtError = bError;
}

/*----------------------------------------------------------------------------*/
/* Get the stream definition                                                  */
/*----------------------------------------------------------------------------*/
StreamDef* StatementContext::pStreamDef()
{
  return m_pStreamDef;
}

/*----------------------------------------------------------------------------*/
/* Save the stream definition                                                 */
/*----------------------------------------------------------------------------*/
void StatementContext::vStreamDef(StreamDef* pStreamDef)
{
  m_pStreamDef = pStreamDef;
}


/*----------------------------------------------------------------------------*/
/* Return a single value for the count of rows affected by this query         */
/*----------------------------------------------------------------------------*/
i8 StatementContext::nAffectedRows()
{
  // The pattern of this needs to match eAffectedVerb()

  if (m_pStreamDef || m_iRowsSelected > 0) {
    return m_iRowsSelected;
  }

  if (m_iRowsUpdated >= m_iRowsInserted &&
      m_iRowsUpdated >= m_iRowsDeleted) {
    return m_iRowsUpdated;
  }

  if (m_iRowsInserted >= m_iRowsUpdated &&
      m_iRowsInserted >= m_iRowsDeleted) {
    return m_iRowsInserted;
  }

  if (m_iRowsDeleted >= m_iRowsInserted &&
      m_iRowsDeleted >= m_iRowsUpdated) {
    return m_iRowsDeleted;
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* Return the affected row verb                                               */
/*----------------------------------------------------------------------------*/
StatementContext::eAffectedVerb StatementContext::eVerb()
{
  // The pattern of this needs to match nAffectedRows()

  if (m_pStreamDef || m_iRowsSelected > 0) {
    return SELECTED;
  }

  if (m_iRowsUpdated >= m_iRowsInserted &&
      m_iRowsUpdated >= m_iRowsDeleted) {
    return UPDATED;
  }

  if (m_iRowsInserted >= m_iRowsUpdated &&
      m_iRowsInserted >= m_iRowsDeleted) {
    return INSERTED;
  }

  if (m_iRowsDeleted >= m_iRowsInserted &&
      m_iRowsDeleted >= m_iRowsUpdated) {
    return DELETED;
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/* Return a count of the number of rows skipped in this statement             */
/*----------------------------------------------------------------------------*/
i8 StatementContext::nRowsSkipped()
{
  return m_iRowsSkipped;
}

/*----------------------------------------------------------------------------*/
/* Return a count of the number of invalid rows in this statement             */
/*----------------------------------------------------------------------------*/
i8 StatementContext::nRowsInvalid()
{
  return m_iRowsInvalid;
}

/*----------------------------------------------------------------------------*/
/* Add to the count of rows processed by this query                           */
/*----------------------------------------------------------------------------*/
void StatementContext::vAddRowsProcessed(i8 nRows)
{
  m_iRowsProcessed += nRows;
}

/*----------------------------------------------------------------------------*/
/* Add to the count of rows skipped by this query                             */
/*----------------------------------------------------------------------------*/
void StatementContext::vAddRowsSkipped(i8 nRows)
{
  m_iRowsSkipped += nRows;
}

/*----------------------------------------------------------------------------*/
/* Add to the count of rows detected as invalid by this query                 */
/*----------------------------------------------------------------------------*/
void StatementContext::vAddRowsInvalid(i8 nRows)
{
  m_iRowsInvalid += nRows;
}

/*----------------------------------------------------------------------------*/
/* Add to the count of rows selected by this query                            */
/*----------------------------------------------------------------------------*/
void StatementContext::vAddRowsSelected(i8 nRows)
{
  m_iRowsSelected += nRows;
}

/*----------------------------------------------------------------------------*/
/* Add to the count of rows inserted by this query                            */
/*----------------------------------------------------------------------------*/
void StatementContext::vAddRowsInserted(i8 nRows)
{
  m_iRowsInserted += nRows;
}

/*----------------------------------------------------------------------------*/
/* Add to the count of rows updated by this query                             */
/*----------------------------------------------------------------------------*/
void StatementContext::vAddRowsUpdated(i8 nRows)
{
  m_iRowsUpdated += nRows;
}

/*----------------------------------------------------------------------------*/
/* Add to the count of rows deleted by this query                             */
/*----------------------------------------------------------------------------*/
void StatementContext::vAddRowsDeleted(i8 nRows)
{
  m_iRowsDeleted += nRows;
}



/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(StatementContext);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
