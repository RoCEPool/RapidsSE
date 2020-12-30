/*============================================================================*/
/* LogEntry.hpp - Log Entry                                                   */
/*============================================================================*/
#pragma once


namespace STORE
{
  static Stz* C_ENTRY_FMT_TXT = "%-16s %20s %s %s: %s"_Stz;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  class LogEntry : public Object
  {
  friend class Log;

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
  
    eLG  m_eKind;      // log entry kind.  one of the following
  
  
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
