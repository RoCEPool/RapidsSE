/*============================================================================*/
/* QueryContext.hpp - Maintains state of the query for the AdaptorWL          */
/*============================================================================*/

#include "BASE.h"
#include "QueryContext.hpp"
#include "StatementContext.hpp"
#include "QueryExecuteRequest_types.h"

using namespace WLP;
/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(WLP,QueryContext,CN_QUERYCONTEXT)
__aNewFunction(QueryContext,CN_QUERYCONTEXT)
__vDeleteFunction(QueryContext,CN_QUERYCONTEXT)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
QueryContext::QueryContext(void)
: Object(CN_QUERYCONTEXT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUERYCONTEXT,_FOOTER_);

  m_pQueryExecuteRequest = nullptr;
  m_nStmtParsed      = 0;
  m_nStmtSucceeded   = 0;
  m_nStmtFailed      = 0;
  m_uQueryStartNanos = 0;
  m_bAbortEarly      = false;

  _postcondition_(_ADDRESS_,_MAGIC_,CN_QUERYCONTEXT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
QueryContext::~QueryContext(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUERYCONTEXT,_FOOTER_);
  delete m_pQueryExecuteRequest;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Reset                                                                      */
/*----------------------------------------------------------------------------*/
void QueryContext::vReset()
{
  delete m_pQueryExecuteRequest;
  m_pQueryExecuteRequest = nullptr;
  m_nStmtParsed          = 0;
  m_nStmtSucceeded       = 0;
  m_nStmtFailed          = 0;
  m_uQueryStartNanos     = 0;
  m_bAbortEarly          = false;
}

/*----------------------------------------------------------------------------*/
/* Initialise                                                                 */
/*----------------------------------------------------------------------------*/
void QueryContext::vInit(com::rapidsdata::wirelineprotocol::messages::v5::QueryExecuteRequest* pQueryExecuteRequest)
{
  ASSERT(!m_pQueryExecuteRequest);

  m_pQueryExecuteRequest = pQueryExecuteRequest;
  m_nStmtParsed          = 0;
  m_nStmtSucceeded       = 0;
  m_nStmtFailed          = 0;
  m_uQueryStartNanos     = Hardware::uReadClock();
  m_bAbortEarly          = false;
}

/*----------------------------------------------------------------------------*/
/* Increment the count of statements parsed                                   */
/*----------------------------------------------------------------------------*/
void QueryContext::vIncStmtParsed()
{
  ++m_nStmtParsed;
}

/*----------------------------------------------------------------------------*/
/* Get the count of statements parsed                                         */
/*----------------------------------------------------------------------------*/
i8 QueryContext::nStmtParsed()
{
  return m_nStmtParsed;
}

/*----------------------------------------------------------------------------*/
/* Increment the count of statements succeeded                                */
/*----------------------------------------------------------------------------*/
void QueryContext::vIncStmtSucceeded()
{
  ++m_nStmtSucceeded;
}

/*----------------------------------------------------------------------------*/
/* Get the count of statements succeeded                                      */
/*----------------------------------------------------------------------------*/
i8 QueryContext::nStmtSucceeded()
{
  return m_nStmtSucceeded;
}

/*----------------------------------------------------------------------------*/
/* Increment the count of statements failed                                   */
/*----------------------------------------------------------------------------*/
void QueryContext::vIncStmtFailed()
{
  ++m_nStmtFailed;
}

/*----------------------------------------------------------------------------*/
/* Get the count of statements failed                                         */
/*----------------------------------------------------------------------------*/
i8 QueryContext::nStmtFailed()
{
  return m_nStmtFailed;
}

/*----------------------------------------------------------------------------*/
/* Get the query start time in nanoseconds                                    */
/*----------------------------------------------------------------------------*/
i8 QueryContext::uQueryStartNanos()
{
  return m_uQueryStartNanos;
}

/*----------------------------------------------------------------------------*/
/* Set the abort early flag                                                   */
/*----------------------------------------------------------------------------*/
void QueryContext::vAbortEarly(bool bAbortEarly)
{
  // If one statement in a batch (query) fails, do we abort the remaining statements?
  m_bAbortEarly = bAbortEarly;
}

/*----------------------------------------------------------------------------*/
/* Get the abort early flag                                                   */
/*----------------------------------------------------------------------------*/
bool QueryContext::bAbortEarly()
{
  // If one statement in a batch (query) fails, do we abort the remaining statements?
  return m_bAbortEarly;
}

/*----------------------------------------------------------------------------*/
/* Return the text of the query (may contain a batch of statements)           */
/*----------------------------------------------------------------------------*/
Stz* QueryContext::szQueryText()
{
  return (m_pQueryExecuteRequest ? (Stz*) (m_pQueryExecuteRequest->query.c_str()) : nullptr);
}

/*----------------------------------------------------------------------------*/
/* Return true if a batch of statements should be aborted if one fails        */
/*----------------------------------------------------------------------------*/
bool QueryContext::bContinueOnError()
{
  return m_pQueryExecuteRequest->continueBatchOnError;
}

/*----------------------------------------------------------------------------*/
/* Return the max number of rows to be sent per RowDataResponse               */
/*----------------------------------------------------------------------------*/
i8 QueryContext::iRowsPerRowDataResponse()
{
  return m_pQueryExecuteRequest->rowsPerRowDataResponse;
}

/*----------------------------------------------------------------------------*/
/* Return true if the client is not expecting a resultset to be returned      */
/*----------------------------------------------------------------------------*/
bool QueryContext::bMustNotReturnRows()
{
  return m_pQueryExecuteRequest->mustNotReturnRows;
}

/*----------------------------------------------------------------------------*/
/* Maximum number of rows to be returned per statement, or -1 for unlimited   */
/*----------------------------------------------------------------------------*/
i8 QueryContext::iMaxRowsToReturn()
{
  return m_pQueryExecuteRequest->maxRowsToReturn;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(QueryContext);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
