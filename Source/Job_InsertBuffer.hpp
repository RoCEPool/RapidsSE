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
    Table*         m_pTable;         // table pointer
    i8             m_iPartitionId;   // partition id
    i8             m_iEndInsertJobId;// EndInsert job id
    i8             m_nWriteBehindBuffers; // write behind buffers
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_InsertBuffer(Interface::Session* pSession);
    ~Job_InsertBuffer(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
    
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_InsertBuffer* _pJob_InsertBuffer(void* aAddr);
  
  Job* pJob_InsertBuffer_pre(Interface::Session* pSession);
  Job* pJob_InsertBuffer_new(Job* pArg);
  void vJob_InsertBuffer_delete(Interface::Session* pSession, Job* pJob);
  void vJob_InsertBuffer_show(Job* pJob);
  
  JobClass* pJob_InsertBuffer_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
