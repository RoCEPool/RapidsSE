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
  
  bool bJob_CreateIndex_pre(Prompt::SePrompt* pPrompt);
  void vJob_CreateIndex_post(Prompt::SePrompt* pPrompt, Job* pJob);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
