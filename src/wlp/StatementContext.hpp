/*============================================================================*/
/* StatementContext.hpp - State of the current statement for the AdaptorWL    */
/*============================================================================*/
#pragma once

namespace WLP
{  
  class StatementContext : public Object
  {
    public:
    enum eAffectedVerb {
      NONE = 0,
      SELECTED,
      INSERTED,
      UPDATED,
      DELETED,
      NENUMS
    };

    private:
    s48K  m_sStmtBuf;
    s256  m_sStmtId;
    bool  m_bGotStmtError;
    StreamDef* m_pStreamDef;  // Definition of the resultset

    i8    m_iRowsProcessed;
    i8    m_iRowsSkipped;
    i8    m_iRowsInvalid;
    i8    m_iRowsSelected;
    i8    m_iRowsInserted;
    i8    m_iRowsUpdated;
    i8    m_iRowsDeleted;

    Footer m_xFooter;

   public:

    void* operator new(size_t);
    void  operator delete(void*);
    
    StatementContext(Stz* szStmt, Stz* szId);
    ~StatementContext(void) noexcept(false);

    Stz* szStmt();
    Stz* szStmtId();
    bool bGotError();
    void vGotError(bool bError);

    StreamDef* pStreamDef();
    void vStreamDef(StreamDef* pDef);

    i8   nAffectedRows();
    eAffectedVerb eVerb();
    i8   nRowsSkipped();
    i8   nRowsInvalid();

    void vAddRowsProcessed(i8 nRows);
    void vAddRowsSkipped(i8 nRows);
    void vAddRowsInvalid(i8 nRows);
    void vAddRowsSelected(i8 nRows);
    void vAddRowsInserted(i8 nRows);
    void vAddRowsUpdated(i8 nRows);
    void vAddRowsDeleted(i8 nRows);


    Footer* aFooter(void);
  };
  
  StatementContext* _pStatementContext(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
