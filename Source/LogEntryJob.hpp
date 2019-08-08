/*============================================================================*/
/* LogEntryJob.hpp - Log Entry for Job                                        */
/*============================================================================*/
#pragma once

namespace Logging
{
  class _LogEntryJob
  {
  public:  
    i8   m_iJobId;      // jobid
    i8   m_iForeign;    // is foreign job id?
  
    eLN  m_eLogNote;           //
  
  public:
    void vSet(LogEntry* pLogEntry, i8 iJobId, i8 iForeign);
  
  void vWriteCsv(File* pFile, LogEntry* pLogEntry,
                 i8 iTickDeltaUsec, i8& iTotalDelta);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
