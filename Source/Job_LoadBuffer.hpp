/*============================================================================*/
/* Job_LoadBuffer.hpp -- Load Buffer Job                                      */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_LoadBuffer : public Job
  {
  private:
    i8 m_iLoadJobId;        // the job id of the load table job
    Table* m_pTable;        // which table
    i8 m_iPartition;        // which partition buffer is to be loaded into
  
    i8 m_nBuffersLoadable;  // buffers to be loaded

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_LoadBuffer(Interface::SEitf* pSEitf);
    ~Job_LoadBuffer(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    i8   nBuffersLoadable(void);
    void vBuffersLoadable(i8 nBuffersLoadable);
  
    i8   iLoadJobId(void);
    void vLoadJobId(i8 iLoadJobId);
  
    i8   iPartition(void);
    void vPartition(i8 iPartition);

    Table* pTable(void);
    void vTable(Table* pTable);
  
    void vShow(void);
  
    Footer* aFooter(void);

  private:
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_LoadBuffer* _pJob_LoadBuffer(void* aAddr);
  
  Job* vJob_LoadBuffer_pre(Interface::SEitf* pSEitf);
  void vJob_LoadBuffer_delete(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_LoadBuffer_show(Job* pJob);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
