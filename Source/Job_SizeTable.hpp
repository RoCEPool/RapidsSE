/*============================================================================*/
/* Job_SizeTable.hpp -- Size Table Job                                        */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_SizeTable : public Job
  {
  public:
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_SizeTable(Interface::SEitf* pSEitf);
    ~Job_SizeTable(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_SizeTable* _pJob_SizeTable(void* aAddr);
  
  Job* pJob_SizeTable_pre(Interface::SEitf* pSEitf);
  void vJob_SizeTable_post(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_SizeTable_delete(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_SizeTable_show(Job* pJob);
  
  Job* pJob_SizeTable_next(void** aaArg);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
