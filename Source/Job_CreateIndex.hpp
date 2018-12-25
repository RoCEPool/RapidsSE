/*============================================================================*/
/* Job_CreateIndex.hpp -- Create Index Job                                    */
/*============================================================================*/
#pragma once

namespace JobModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_CreateIndex : public Job
  {
  public:
    
    bool   m_bIsUnique;     // is unique index
    i8     m_iColumns;     // no of columns
    s32    m_sColumnName[C_MAX_COLUMNS];   // name of column
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_CreateIndex(Interface::Session* pSession);
    ~Job_CreateIndex(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  private:
    bool bValidateCreateIndex(TokenVec* pTokenVec, i8 iNumTokens);
    
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_CreateIndex* _pJob_CreateIndex(void* aAddr);
  
  Job* pJob_CreateIndex_pre(Interface::Session* pSession);
  void vJob_CreateIndex_post(Interface::Session* pSession, Job* pJob);
  void vJob_CreateIndex_delete(Interface::Session* pSession, Job* pJob);
  void vJob_CreateIndex_show(Job* pJob);
  
  JobClass* pJob_CreateIndex_class(pFUN** aaFun);
  JobClass* pJob_CreateIndexUnique_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
