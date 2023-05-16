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

    s48K  m_sStmtBuf;
    s256  m_sStmtId;
    bool  m_bGotStmtError;
    i8    m_iAffectedRows;
    eAffectedVerb m_eVerb;
    StreamDef* m_pStreamDef;  // Definition of the resultset

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
    i8   nAffectedRows();
    void vAddAffectedRows(i8 nRows);
    void vAffectedRows(i8 nRows);
    eAffectedVerb eVerb();
    void vVerb(eAffectedVerb eVerb);
    StreamDef* pStreamDef();
    void vStreamDef(StreamDef* pDef);

    Footer* aFooter(void);
  };
  
  StatementContext* _pStatementContext(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
