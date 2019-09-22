/*============================================================================*/
/* Job_CreateNode.hpp -- Create Node Job                                      */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class Job_CreateNode : public Job
  {
  public:
    
    eTT    m_eTableType;   // table type
    i8     m_iColumns;     // no of columns
    
    Column m_xColumns[C_MAX_COLUMNS];   // column definitions
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_CreateNode(JobClass* aJobClass, ReqTuple* aReqTuple, i8 iForId, Session* pSession);//=C_UNKNOWN);
    ~Job_CreateNode(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    eEC ePopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  private:
    eEC  eAddTableColumns(TokenVec* pTokenVec, i8& iCurrentToken);
    bool bExtractDecimal(TokenVec* pTokenVec, i8 iColumn, i8& iCurrentToken);
    bool bExtractFloat(TokenVec* pTokenVec, i8 iColumn, i8& iCurrentToken);
    bool bExtractVarchar(TokenVec* pTokenVec, i8 iColumn, i8& iCurrentToken);
    bool bPartitionTable(TokenVec* pTokenVec, i8 iCurrentToken);
    bool bValidateCreateNode(TokenVec* pTokenVec);
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_CreateNode* _pJob_CreateNode(void* aAddr);
  
  Job* pJob_CreateNode_pre(JobClass* aJobClass, ReqTuple* aReqTuple, i8 iForId, Session* pSession);
  void vJob_CreateNode_post(Job* pJob);
  Job* pJob_CreateNode_new(JobClass* aJobClass);
  void vJob_CreateNode_delete(Job* pJob);
  void vJob_CreateNode_show(Job* pJob);
  
  JobClass* pJob_CreateNode_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
