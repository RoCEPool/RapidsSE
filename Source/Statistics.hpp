/*============================================================================*/
/* Statistics.hpp -- Statistics                                               */
/*============================================================================*/
#pragma once

namespace Measurement
{
  class Statistics : public Object
  {
  public:

  struct {
    i8 m_nCloses;
    i8 m_nFlushes;
    i8 m_nGetcs;
    i8 m_nOpens;
    i8 m_nPrints;
    i8 m_nReads;
    i8 m_nWrites;
    i8 m_nSeeks;
    i8 m_nTells;
    i8 m_nUngetcs;
   
    i8 m_nReadBytes;
    i8 m_nPrintBytes;
    i8 m_nWriteBytes;
  } File;

  struct {
    i8 m_iaCopy[CN_ENUMS];
    i8 m_iaMove[CN_ENUMS];
    i8 m_iaScribble[CN_ENUMS];
    i8 m_iaSet[CN_ENUMS];
    i8 m_iaZero[CN_ENUMS];
  
    i8 m_iCopyTotal;
    i8 m_iMoveTotal;
    i8 m_iScribbleTotal;
    i8 m_iSetTotal;
    i8 m_iZeroTotal;
  } Mem;
  
  struct {
    i8 m_iEnqueueTask;
    i8 m_iEnqueueTaskWait;
    i8 m_iGetTask;
    i8 m_iGetTaskWait;
  } Sch;
  
  struct {
    i8 m_nAccepts;
    i8 m_nCloses;
    i8 m_nConnects;
    i8 m_nListens;
    i8 m_nPeeks;
    i8 m_nRecvs;
    i8 m_nRecvFroms;
    i8 m_nSends;
    i8 m_nSendTos;
  
    i8 m_nRecvBytes;
    i8 m_nSendBytes;
  } Socket;
  
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
