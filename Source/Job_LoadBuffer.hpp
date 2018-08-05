/*============================================================================*/
/* Job_LoadBuffer.hpp -- Load Buffer Job                                      */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_LoadBuffer : public Job,
                         public JobDicPath,
                         public JobObjectName,
                         public JobResult
  {
  private:
    i8 m_nBuffersLoadable;  // buffers to be loaded
    i8 m_iPartition;        // which partition buffer is to be loaded into

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
  
    i8   iPartition(void);
    void vPartition(i8 iPartition);

    void vShow(void);
  
    Footer* aFooter(void);

  private:
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_LoadBuffer* _pJob_LoadBuffer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
