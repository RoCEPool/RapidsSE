/*============================================================================*/
/* StatementContext.hpp - State of the current statement for the AdaptorWL    */
/*============================================================================*/
#pragma once

namespace WLP
{  
  class StatementContext : public Object
  {
    public:
    s48K  m_sStmtBuf;
    bool  m_bGotStmtError;

    Footer m_xFooter;

   public:

    void* operator new(size_t);
    void  operator delete(void*);
    
    StatementContext(void);
    ~StatementContext(void) noexcept(false);

    void vResetContext();
  
    Footer* aFooter(void);
  };
  
  StatementContext* _pStatementContext(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
