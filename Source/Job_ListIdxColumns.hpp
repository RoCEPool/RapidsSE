/*============================================================================*/
/* Job_ListIdxColumns.hpp -- List Index Columns Job                           */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_ListIdxColumns : public Job
  {
  public:
    
    s32 m_sTableName;   // table name                  //! Alka, is this needed?
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_ListIdxColumns(Interface::SEitf* pSEitf);
    ~Job_ListIdxColumns(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_ListIdxColumns* _pJob_ListIdxColumns(void* aAddr);
  
  Job* pJob_ListIdxColumns_pre(Interface::SEitf* pSEitf);
  void vJob_ListIdxColumns_post(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_ListIdxColumns_delete(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_ListIdxColumns_show(Job* pJob);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
