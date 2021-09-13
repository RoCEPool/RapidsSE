/*============================================================================*/
/* AggTimer.hpp -- AggTimer                                                   */
/*============================================================================*/
#pragma once

namespace BASE
{
  class Stats : public Object
  {
  public:
  
  public:

    // cpu
    i8 m_iStart;      // starting ticks
    i8 m_iUpdate;     // update point

    // work
    i8 m_iTicks;      // total ticks
    i8 m_nTimes;      // ntimes

    i8 m_nOperands;   // total operands processed
    i8 m_nMaxQs;      // max queue

    i8 m_nBuffIn;     // buffers in
    i8 m_nBuffOut;    // buffer out

    // i/o
    i8 m_nSends;      // send() count
    i8 m_nBytesSent;  // total bytes sent
    i8 m_nRecvs;      // recv() count
    i8 m_nBytesRecv;  // total bytes recv

    i8 m_nRead;       // read() count
    i8 m_nBytesRead;  // total bytes read
    i8 m_nWrite;      // write() count
    i8 m_nBytesWrite; // total bytes write

    // memory
    i8 m_nNewRowBuffer;  // new of row buffers
    i8 m_nDelRowBuffer;  // delete of row buffers
    i8 m_nMaxRowBuffer;  // max row buffers
    i8 m_nAvgRowBuffer;  // avg row buffers

    i8 m_nNewCsvBuffer;  // new of csv buffers
    i8 m_nDelCsvBuffer;  // delete of csv buffers
    i8 m_nMaxCsvBuffer;  // max csv buffers
    i8 m_nAvgCsvBuffer;  // avg csv buffers

    i8 m_nAllocs;        // no of allocs
    i8 m_nBytesAlloc;    // bytes allocated
    i8 m_nFrees;         // no of frees
    i8 m_nBytesFree;     // bytes freed
    i8 m_nMaxBytes;      // max bytes allocated
    i8 m_nAvgBytes;      // avg bytes allocated

    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void operator delete(void* pStatsAddress);

    Stats(void);
   ~Stats(void) noexcept(false);

    static void vRepair(Object* pObject);

    void vStartTimer(void);
    void vUpdateTimer(void);

    i8 iNanoseconds(void);
    i8 iMicroseconds(void);
    i8 iMilliseconds(void);
    i8 iSeconds(void);
    i8 iTicks(void);
    i8 nTimes(void);

    d8   dRatePerSec(i8 nItems);

    Footer* aFooter(void);
  };

  Stats* _pStats(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
