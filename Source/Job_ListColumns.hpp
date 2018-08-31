/*============================================================================*/
/* Job_ListColumns.hpp -- List Columns Job                                    */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_ListColumns : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_ListColumns(Interface::SEitf* pSEitf);
    ~Job_ListColumns(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_ListColumns* _pJob_ListColumns(void* aAddr);
  
  Job* pJob_ListColumns_pre(Interface::SEitf* pSEitf);
  void vJob_ListColumns_post(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_ListColumns_delete(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_ListColumns_show(Job* pJob);
  
  pNEXTJOBFUN* pJob_ListColumns_next(void** aaArg);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
