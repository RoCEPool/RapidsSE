/*============================================================================*/
/* Job_InsertBuffer.hpp -- Insert Buffer Job                                  */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_InsertBuffer : public Job
  {
  public:
    WorkingBuffer* m_pWorkingBuffer; // working buffer
    Table*         m_pTable;         // table pointer
    Tasks::Task*   m_pTask;          // task running this job
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_InsertBuffer(Interface::SEitf* pSEitf);
    ~Job_InsertBuffer(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
    
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_InsertBuffer* _pJob_InsertBuffer(void* aAddr);
  
  Job* pJob_InsertBuffer_pre(Interface::SEitf* pSEitf);
  Job* pJob_InsertBuffer_new(Job* pArg);
  void vJob_InsertBuffer_delete(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_InsertBuffer_show(Job* pJob);
  
  JobClass* _pJob_InsertBuffer_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
