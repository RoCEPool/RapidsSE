/*============================================================================*/
/* Statistics.hpp -- Statistics                                               */
/*============================================================================*/
#pragma once

namespace Measurement
{
  class Statistics : public Object
  {
  public:

    i8 m_nFileCloses;
    i8 m_nFileFlushes;
    i8 m_nFileGetcs;
    i8 m_nFileOpens;
    i8 m_nFilePrints;
    i8 m_nFileReads;
    i8 m_nFileWrites;
    i8 m_nFileSeeks;
    i8 m_nFileTells;
    i8 m_nFileUngetcs;
  
    i8 m_nFileReadBytes;
    i8 m_nFilePrintBytes;
    i8 m_nFileWriteBytes;
  
    i8 m_iaMemCopy[CN_ENUMS];
    i8 m_iaMemMove[CN_ENUMS];
    i8 m_iaMemScribble[CN_ENUMS];
    i8 m_iaMemSet[CN_ENUMS];
    i8 m_iaMemZero[CN_ENUMS];
  
    i8 m_iMemCopyTotal;
    i8 m_iMemMoveTotal;
    i8 m_iMemScribbleTotal;
    i8 m_iMemSetTotal;
    i8 m_iMemZeroTotal;
  
    i8 m_iSchEnqueueTask;
    i8 m_iSchEnqueueTaskWait;
    i8 m_iSchGetTask;
    i8 m_iSchGetTaskWait;
  
    i8 m_nSocketAccepts;
    i8 m_nSocketCloses;
    i8 m_nSocketConnects;
    i8 m_nSocketListens;
    i8 m_nSocketPeeks;
    i8 m_nSocketRecvs;
    i8 m_nSocketRecvFroms;
    i8 m_nSocketSends;
    i8 m_nSocketSendTos;
  
    i8 m_nSocketRecvBytes;
    i8 m_nSocketSendBytes;

  private:

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void operator delete(void*);
  
    Statistics(void);
    ~Statistics() noexcept(false);
  
  private:
  
    Footer* aFooter(void);
  };
  
  Statistics* _pStatistics(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
