/*============================================================================*/
/* Statistics.hpp -- Statistics                                               */
/*============================================================================*/
#pragma once

namespace Measurement
{
  class Statistics : public Object
  {
  public:
  
  i8 m_nMonitorTake;

  struct {
    i8 m_nBeginFetch;
    i8 m_nBeginInsert;
    i8 m_nBeginSubInserter;
    i8 m_nCheckVersion;
    i8 m_nCopyBuffer;
    i8 m_nDeleteSession;
    i8 m_nEndInsert;
    i8 m_nEndJobs;
    i8 m_nEndSubInserter;
    i8 m_nExecuteCommand;
    i8 m_nFetchNextBuffer;
    i8 m_nGetLoadResult;
    i8 m_nGetRecordBuffer;
    i8 m_nGetRow;
    i8 m_nGetRowCount;
    i8 m_nGetTableColumnRow;
    i8 m_nGetTableColumnRowCount;
    i8 m_nInsertRow;
    i8 m_nIsInvalidSession;
    i8 m_nNewSession;
    i8 m_nNextRow;
    i8 m_nPoolOpen;
    i8 m_nTranslateError;
  } Api;
  
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
    i8 m_nCreates;
    i8 m_nDrops;
  } Index;
  
  struct {
    i8 m_nAppendBuffers;
    i8 m_nAppendRows;
    i8 m_nBuffersPurged;
  } IndexTree;
  
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
  } Memory;
    
  struct {
    i8 m_nBroadcasts;
    i8 m_nDestroys;
    i8 m_nInits;
    i8 m_nSignals;
    i8 m_nTimedWaits;
    i8 m_nTimeOuts;
    i8 m_nUnblocks;
    i8 m_nWaits;
  } Mutex;
  
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
  
  struct {
    i8 m_nCreates;
    i8 m_nDrops;
    i8 m_nLoads;
  } Table;
  
  struct {
    i8 m_nAcks;
    i8 m_nExecuting;
    i8 m_nExited;
    i8 m_nReady;
    i8 m_nSend;
    i8 m_nSleeping;
    i8 m_nWaiting;
    i8 m_nYielded;
  } Task;
  
  struct {
    i8 m_iaSleeps[CN_ENUMS];
    i8 m_iaDuration[CN_ENUMS];
    i8 m_nSleeps;
  } Thread;
  
  private:

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void operator delete(void*);
  
    Statistics(void);
    ~Statistics() noexcept(false);
  
    static void vMemoryPortrait(s48K& sPortrait);

  private:
  
    Footer* aFooter(void);
  };
  
  Statistics* _pStatistics(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
