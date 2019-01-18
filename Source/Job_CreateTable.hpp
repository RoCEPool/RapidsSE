/*============================================================================*/
/* Job_CreateTable.hpp -- Create Table Job                                    */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_CreateTable : public Job
  {
  public:
    
    eTT    m_eTableType;   // table type
    i8     m_iColumns;     // no of columns
    
    struct {
      
      i8    m_iColumnNo;     // column no )within this table)
      s32   m_sColumnName;   // name of column
      eCT   m_eColumnType;   // column type
      i8    m_iColumnLength; // length of column
      i8    m_iColumnScale;  // column scale
      bool  m_bIsNullable;   // is allowed to be null
      bool  m_bIsUnique;     // is unique
      bool  m_bIsPartitionKey; // is a partition key?
      eDM   m_eDatumType;    // SE datum type
      
    } m_xColumns[C_MAX_COLUMNS];
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_CreateTable(Interface::Session* pSession);
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
  
  Job* pJob_CreateTable_pre(Interface::Session* pSession);
  void vJob_CreateTable_post(Interface::Session* pSession, Job* pJob);
  Job* pJob_CreateTable_new(Job* pArg);
  void vJob_CreateTable_delete(Interface::Session* pSession, Job* pJob);
  void vJob_CreateTable_show(Job* pJob);
  
  JobClass* pJob_CreateTable_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
