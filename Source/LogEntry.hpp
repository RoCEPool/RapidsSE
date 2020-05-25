/*============================================================================*/
/* LogEntry.hpp - Log Entry                                                   */
/*============================================================================*/
#pragma once

namespace Logging
{
  static Stz* C_ENTRY_FMT_TXT = "%-16s %20s %s %s: %s"_Stz;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  class LogEntry : public Object
  {
  friend class Log;
  friend class _LogEntryDdl;
  friend class _LogEntryFyi;
  friend class _LogEntryJob;
  friend class _LogEntryLck;
  friend class _LogEntryLog;
  friend class _LogEntryMgt;
  friend class _LogEntryObj;
  friend class _LogEntryOp;
  friend class _LogEntryPrm;
  friend class _LogEntryReq;
  friend class _LogEntryQue;
  friend class _LogEntryRes;
  friend class _LogEntrySsn;
  friend class _LogEntrySvc;
  friend class _LogEntrySys;
  friend class _LogEntryThd;
  friend class _LogEntryTsk;
  friend class _LogEntryTxn;

  private:
    i8   m_i_Entry;    // this entry no
    i8   m_i_Prev;     // previous entry
  
    i8   m_iTick;      // tick when entry added
    i8   m_iNodeNo;    // which node
    char m_caNode[32]; // which node
  
    eCN  m_eClass;     // class of caller
  
    Focal* m_pFocal;   // object
    i8   m_i_Focal;    // focal lattice subscript
  
    ReqTuple m_xReqTuple;
  
    i8   m_iAct;
    eSL  m_eSeverity;  // severity level
  
    bool m_bLine;      // draw line before?
  
    char m_cPrefix;
    i8   m_iFocalId;
    i8   m_iForId;
  
    i8   m_iIndicator;
  
    char m_caNote[C_MAX_LOG_NOTE]; // comment
  
    eEK  m_eKind;      // log entry kind.  one of the following
    union {
      _LogEntryDdl m_xDdl;  // when m_eKind == EK_DDL
      _LogEntryFyi m_xFyi;  // when m_eKind == EK_FYI
      _LogEntryJob m_xJob;  // when m_eKind == EK_JOB
      _LogEntryLck m_xLck;  // when m_eKind == EK_LCK
      _LogEntryLog m_xLog;  // when m_eKind == EK_LOG
      _LogEntryMgt m_xMgt;  // when m_eKind == EK_MGT
      _LogEntryOp  m_xOpr;  // when m_eKind == EK_OPR
      _LogEntryPrm m_xPrm;  // when m_eKind == EK_PRM
      _LogEntryQue m_xQue;  // when m_eKind == EK_QUE
      _LogEntryRes m_xRes;  // when m_eKind == EK_RES
      _LogEntrySsn m_xSsn;  // when m_eKind == EK_SSN
      _LogEntrySvc m_xSvc;  // when m_eKind == EK_SVC
      _LogEntrySys m_xSys;  // when m_eKind == EK_SYS
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

    void vAsTxt(s4K& sString);
    Stz* szIndicator(void);
    void vInit(eSL eSeverity, eCN eClass, Stz* szText);
    void vInit(eSL eSeverity, Focal* pFocal, Stz* szText);
    void vSprint(s4K& sString,i8 iEntryDeltaUsec, i8& iTotalDeltaUsec);
    void vSprintCsv(s4K& sString, i8 iEntryDeltaUsec, i8& iTotalDeltaUsec);
    void vWriteCsv(File* pFile, i8 iEntryDeltaUsec, i8& iTotalDeltaUsec);
    void vWriteCsvEntry(File* pFile, i8 iEntryDeltaUsec, i8& iTotalDeltaUsec);
    void vWriteTxt(File* pFile);
    void vWriteTxtEntry(File* pFile);
    void vZero(void);

    Footer* aFooter(void);
  };
  
  LogEntry* _pLogEntry(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
