/*============================================================================*/
/* Job_CreateIndex.hpp -- Create Index Job                                    */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_CreateIndex : public Job,
                          public JobDicPath,
                          public JobObjectName,
                          public JobResult
  {
  public:
    
    s32    m_sTableName;   // table name
    bool   m_bIsUnique;     // is unique index
    i8     m_iColumns;     // no of columns
    s32    m_sColumnName[C_MAX_COLUMNS];   // name of column
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_CreateIndex(Interface::SEitf* pSEitf);
    ~Job_CreateIndex(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_CreateIndex* _pJob_CreateIndex(void* aAddr);
  
  Job* pJob_CreateIndex_pre(TokenVec* pTokenVec);
  void vJob_CreateIndex_post(void);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
