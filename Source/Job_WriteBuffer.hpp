/*============================================================================*/
/* Job_WriteBuffer.hpp -- Write Buffer Job                                    */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_WriteBuffer : public Job
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
  
    Job_WriteBuffer(Interface::Session* pSession);
    ~Job_WriteBuffer(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
    
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_WriteBuffer* _pJob_WriteBuffer(void* aAddr);
  
  Job* pJob_WriteBuffer_pre(Interface::Session* pSession);
  Job* pJob_WriteBuffer_new(Job* pArg);
  void vJob_WriteBuffer_delete(Interface::Session* pSession, Job* pJob);
  void vJob_WriteBuffer_show(Job* pJob);
  
  JobClass* pJob_WriteBuffer_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
