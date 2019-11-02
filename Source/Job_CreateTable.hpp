/*============================================================================*/
/* Job_CreateTable.hpp -- Create Table Job                                    */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class Job_CreateTable : public Job
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
  
    Job_CreateTable(JobClass* aJobClass, ReqTuple* aReqTuple);
    ~Job_CreateTable(void) noexcept(false);
    
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
    bool bValidateCreateTable(TokenVec* pTokenVec);
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_CreateTable* _pJob_CreateTable(void* aAddr);
  
  Job* pJob_CreateTable_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession);
  void vJob_CreateTable_post(Job* pJob);
  Job* pJob_CreateTable_new(JobClass* aJobClass, ReqTuple* aReqTuple);
  void vJob_CreateTable_delete(Job* pJob);
  void vJob_CreateTable_show(Job* pJob);
  
  JobClass* pJob_CreateTable_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
