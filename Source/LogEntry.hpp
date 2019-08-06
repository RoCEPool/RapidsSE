/*============================================================================*/
/* LogEntry.hpp - Log Entry                                                   */
/*============================================================================*/
#pragma once

namespace Logging
{
  class LogEntry : public Object
  {
  friend class Log;
  friend class _LogEntryJob;
  friend class _LogEntryOp;
  friend class _LogEntryRes;
  friend class _LogEntryThd;
  friend class _LogEntryTxt;

  private:
    i8   m_i_Entry;     // entry no
  
    i8   m_iTick;       // tick when entry added
    char m_caNode[32];  // which node
  
    eEK  m_eKind;       // log entry kind.  one of the following
    union {
      _LogEntryJob m_xJob;      // when m_eKind == EK_JOB
      _LogEntryOp  m_xOp;       // when m_eKind == EK_OP
      _LogEntryRes m_xResource; // when m_eKind == EK_RESOURCE
      _LogEntryThd m_xThread;   // when m_eKind == EK_THREAD
      _LogEntryTxt m_xText;     // when m_eKind == EK_TEXT
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
