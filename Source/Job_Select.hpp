/*============================================================================*/
/* pJob_Select.hpp -- Select Statement Job                                    */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class Job_Select : public JobMark2
  {
  public:
    Digest* m_pDigest;

    Op_ToClause*   m_pToClause;  // convert to clauses
    Op_ToFilter*   m_pToFilter;  // convert to filters
    Op_ToCommand*  m_pToCommand; // back to string form
    Op_ToDo*       m_pToDo;      // give back to prompt
    ZedOp*         m_pZed;       // sink
  
    Stmt_Select*   m_pStmt_Select;


    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Select(Session* pSession, i8 iCommandId=C_UNKNOWN);
    ~Job_Select(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);

    void vShow(void);
    
    void vBegin(void);
    void vEnd(void);
    void vExecute(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Job_Select* _pJob_Select(void* aAddr);
  
  Job* pJob_Select_pre(Session* pSession, i8 iCommandId);
  void vJob_Select_delete(Session* pSession, Job* pJob);
  void vJob_Select_show(Job* pJob);
  
  JobClass* pJob_Select_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
