/*============================================================================*/
/* LogEntry.hpp - Log Entry                                                   */
/*============================================================================*/
#pragma once

namespace Logging
{
  class LogEntry : public Object
  {
  friend class Log;
  friend class _LogEntryInf;
  friend class _LogEntryItf;
  friend class _LogEntryJob;
  friend class _LogEntryOp;
  friend class _LogEntryPrm;
  friend class _LogEntryRes;
  friend class _LogEntrySvc;
  friend class _LogEntryThd;
  friend class _LogEntryTsk;
  friend class _LogEntryTxn;

  private:
    i8   m_i_Entry;     // entry no
  
    i8   m_iTick;       // tick when entry added
    char m_caNode[32];  // which node
  
    eEK  m_eKind;       // log entry kind.  one of the following
    union {
      _LogEntryInf m_xInf;  // when m_eKind == EK_INF
      _LogEntryItf m_xItf;  // when m_eKind == EK_ITF
      _LogEntryJob m_xJob;  // when m_eKind == EK_JOB
      _LogEntryOp  m_xOp;   // when m_eKind == EK_OP
      _LogEntryPrm m_xPrm;  // when m_eKind == EK_PRM
      _LogEntryRes m_xRes;  // when m_eKind == EK_RES
      _LogEntrySvc m_xSvc;  // when m_eKind == EK_SVC
      _LogEntryTsk m_xTsk;  // when m_eKind == EK_TSK
      _LogEntryThd m_xThd;  // when m_eKind == EK_THD
      _LogEntryTxn m_xTxn;  // when m_eKind == EK_TXN
    };
  
    // must be last
    Footer m_xFooter;
    
  public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    LogEntry(void);
   ~LogEntry(void) noexcept(false);

    void vInit(void);
    void vWrite(File* pFile, i8 iEntryDeltaUsec, i8& iTotalDeltaUsec);
    void vZero(void);

    Footer* aFooter(void);
  };
  
  LogEntry* _pLogEntry(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
