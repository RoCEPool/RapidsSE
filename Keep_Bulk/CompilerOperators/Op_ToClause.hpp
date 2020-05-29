/*============================================================================*/
/* Op_ToClause.hpp -- To Clause operator                                      */
/*============================================================================*/
#pragma once

namespace CompilerOperators
{
  class Op_ToClause : public Op
  {
  private:
  
    Clause_From*    m_pClause_From;
    Clause_GroupBy* m_pClause_GroupBy;
    Clause_Join*    m_pClause_Join;
    Clause_Limit*   m_pClause_Limit;
    Clause_OrderBy* m_pClause_OrderBy;
    Clause_Select*  m_pClause_Select;
    Clause_Where*   m_pClause_Where;

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Op_ToClause(OpSpec* pOpSpec, Pipeline* pPipeline);
   ~Op_ToClause(void) noexcept(false);
  
    static Op* pNew(OpSpec* pOpSpec, Pipeline* pPipeline);
    static eOR eCall(void* aThis);
    static eOR eDrain(void* aThis);

    eOR eOpLoop(void);
    eOR eDrainLoop(void);

  private:
    void vAsClauses(Stmt_Select* pStmt_Select);
    void vPipeClauses(void);

  private:
    Footer* aFooter(void);
  
  };

  Op_ToClause* _pOp_ToClause(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
