/*============================================================================*/
/* pJob_Select.hpp -- Select Statement Job                                    */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class Job_Select : public Job
  {
  public:
    Pipeline* m_pPipeline;

    Op_ToClause*   m_pToClause;  // convert to clauses
    Op_ToFilter*   m_pToFilter;  // convert to filters
    Op_ToCommand*  m_pToCommand; // back to string form
    Op_ToDo*       m_pToDo;      // give back to prompt
    
    //! ZEDOP has been removed !!!
    //ZedOp*         m_pZed;       // sink
  
    Stmt_Select*   m_pStmt_Select;


    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Select(JobClass* aJobClass, ReqTuple* aReqTuple);
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
  
  Job* pJob_Select_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession);
  void vJob_Select_delete(Job* pJob);
  void vJob_Select_show(Job* pJob);
  
  JobClass* pJob_Select_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
