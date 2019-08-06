/*============================================================================*/
/* LogEntryJob.hpp - Log Entry for Job                                        */
/*============================================================================*/
#pragma once

namespace Logging
{
  class _LogEntryJob
  {
  public:  
    i8   m_iJobId;       // enum/jobid
    i8   m_iForeign;    // is foreign job id?
  
    eLN  m_eLogNote;           //
    char m_caText[C_LOG_TEXT]; // comment
  
  public:
    void vSet(LogEntry* pLogEntry, i8 iJobId, i8 iForeign, char* aText);
  
    void vWrite(File* pFile, LogEntry* pLogEntry, i8 iTickDeltaUsec, i8& iTotalDelta);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
