/*============================================================================*/
/* QueryContext.hpp - Maintains state of the query for the AdaptorWL          */
/*============================================================================*/
#pragma once

// Forward declarations (outside namespace WLP scope)
namespace com { namespace rapidsdata { namespace wirelineprotocol { namespace messages { namespace v5 {
  struct MessageTypes;
  struct DataType;
  class QueryExecuteRequest;
}}}}}

namespace WLP
{
  // Forward declarations and typedefs
  class StatementContext;
  typedef ListExo_t<CN_STATEMENTCTXLIST, StatementContext> StatementCtxList;
  typedef ListExoIter_t<StatementContext> StatementCtxIter;

  class QueryContext : public Object
  {
    private:
    com::rapidsdata::wirelineprotocol::messages::v5::QueryExecuteRequest* m_pQueryExecuteRequest;
    i8  m_nStmtParsed;
    i8  m_nStmtSucceeded;
    i8  m_nStmtFailed;
    u8  m_uQueryStartNanos;
    StatementCtxList* m_pStmtCtxList;

    Footer m_xFooter;

   public:

    void* operator new(size_t);
    void  operator delete(void*);
    
    QueryContext(void);
    ~QueryContext(void) noexcept(false);

    void vReset();
    void vInit(com::rapidsdata::wirelineprotocol::messages::v5::QueryExecuteRequest* pQueryExecuteRequest);
    StatementContext* pPopStmtCtx();
    StatementCtxIter* pStmtCtxIter();

    void vIncStmtParsed();
    i8   nStmtParsed();
    void vIncStmtSucceeded();
    i8   nStmtSucceeded();
    void vIncStmtFailed();
    i8   nStmtFailed();
    i8   uQueryStartNanos();
    i8   nStmtContexts();

    // Information from the QueryExecuteRequest
    Stz* szQueryText();
    bool bContinueOnError();
    i8   iRowsPerRowDataResponse();
    bool bMustNotReturnRows();
    i8   iMaxRowsToReturn();

  private:
    void vExtractStatements();

    Footer* aFooter(void);
  };
  
  QueryContext* _pQueryContext(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
