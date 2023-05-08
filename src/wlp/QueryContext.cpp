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
  m_pStmtCtxList = new StatementCtxList;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_QUERYCONTEXT,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
QueryContext::~QueryContext(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_QUERYCONTEXT,_FOOTER_);
  delete m_pQueryExecuteRequest;

  m_pStmtCtxList->vDeleteAll();
  delete m_pStmtCtxList;
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
  m_nStmtParsed        = 0;
  m_nStmtSucceeded     = 0;
  m_nStmtFailed        = 0;
  m_uQueryStartNanos   = 0;
  m_pStmtCtxList->vDeleteAll();
}

/*----------------------------------------------------------------------------*/
/* Initialise                                                                 */
/*----------------------------------------------------------------------------*/
void QueryContext::vInit(com::rapidsdata::wirelineprotocol::messages::v5::QueryExecuteRequest* pQueryExecuteRequest)
{
  ASSERT(!m_pQueryExecuteRequest);
  ASSERT(m_pStmtCtxList->nItems() == 0);

  m_pQueryExecuteRequest = pQueryExecuteRequest;
  m_nStmtParsed        = 0;
  m_nStmtSucceeded     = 0;
  m_nStmtFailed        = 0;
  m_uQueryStartNanos   = Hardware::uReadClock();
  //m_pStmtCtxList

  // Parse out each statement in the batch from the query and create a StatementContext
  // for each one.
  vExtractStatements();
}

/*----------------------------------------------------------------------------*/
/* Extracts statements & creates a StatementContext for each                  */
/*----------------------------------------------------------------------------*/
void QueryContext::vExtractStatements()
{
  ASSERT(m_pQueryExecuteRequest);
  ASSERT(m_pStmtCtxList);
  ASSERT(m_pStmtCtxList->nItems() == 0);
  
  i8 nQueryLen = (i8) m_pQueryExecuteRequest->query.length();

  i8 iStmtCrs = -1;
  i8 iStartCrs = 0;
  StatementContext* pStmtCtx;

  while (++iStmtCrs < nQueryLen) {
    char c = m_pQueryExecuteRequest->query.at(iStmtCrs);

    switch (c) {

      case '/':
        // Check for SQL comments and advance over the comment block
        if (    (iStmtCrs + 1 < nQueryLen)
                && m_pQueryExecuteRequest->query.at(iStmtCrs + 1) == '*'
            ) {
          ++iStmtCrs;  // Cursor points at the first '*'

          // Advance to the "*/" or the EOS
          while (++iStmtCrs < nQueryLen) {
            char d = m_pQueryExecuteRequest->query.at(iStmtCrs);
            if (d == '*' && (iStmtCrs + 1 < nQueryLen) && m_pQueryExecuteRequest->query.at(iStmtCrs + 1) == '/') {
              break;
            }
          }
        }
        break;

      case '\'':
        // Advance over the single-quoted string
        while (++iStmtCrs < nQueryLen && m_pQueryExecuteRequest->query.at(iStmtCrs) != '\'') {}
        break;

      case '"':
        // Advance over the double-quoted string
        while (++iStmtCrs < nQueryLen && m_pQueryExecuteRequest->query.at(iStmtCrs) != '"') {}
        break;

      case '`':
        // Advance over the backtick identifier
        while (++iStmtCrs < nQueryLen && m_pQueryExecuteRequest->query.at(iStmtCrs) != '`') {}
        break;

      case ';':
        // Extract the semicolon terminated statement
        // TODO: TEMP: Don't include the terminating semicolon as this gets interpreted as another
        //       argument to the sqlCompiler procedure.
        pStmtCtx = new StatementContext((Stz*) (m_pQueryExecuteRequest->query.c_str() + iStartCrs),
                                        iStmtCrs - iStartCrs,
//                                        m_iStmtCrs - iStartCrs + 1,
                                        "abc"_Stz,    // TODO: Use a real statementId
                                        -1);          // TODO: Use a real statementKey
        m_pStmtCtxList->vAppend(pStmtCtx);

        // Another statement?
        iStartCrs = iStmtCrs + 1;
        break;

      default:
        continue;
    }
  }
}

/*----------------------------------------------------------------------------*/
/* Get the next StatementContext, or nullptr if there are no more             */
/*----------------------------------------------------------------------------*/
StatementContext* QueryContext::pPopStmtCtx()
{
  ASSERT(m_pStmtCtxList);
  return m_pStmtCtxList->pPop();
}

/*----------------------------------------------------------------------------*/
/* Get an iterator of statement context objects. Must be deleted              */
/*----------------------------------------------------------------------------*/
StatementCtxIter* QueryContext::pStmtCtxIter()
{
  ASSERT(m_pStmtCtxList);
  return m_pStmtCtxList->pNewIter();
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
/* Get the number of statement contexts in the list                           */
/*----------------------------------------------------------------------------*/
i8 QueryContext::nStmtContexts()
{
  return m_pStmtCtxList->nItems();
}

/*----------------------------------------------------------------------------*/
/* Return the text of the query (may contain a batch of statements)           */
/*----------------------------------------------------------------------------*/
Stz* QueryContext::szQueryText()
{
  return (Stz*) (m_pQueryExecuteRequest->query.c_str());
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
