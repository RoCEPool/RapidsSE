/*============================================================================*/
/* LogEntryRes.hpp - Log Entry Resource                                       */
/*============================================================================*/
#pragma once

namespace Logging
{
  class _LogEntryRes
  {
  public:
    eSR  m_eSysRes;            // enum/jobid
    char m_caText[C_LOG_TEXT]; // comment

  public:
   void vSet(LogEntry* pLogEntry, eSR eSysRes, char* aText);
  
   void vWrite(File* pFile, LogEntry* pLogEntry,
                i8 iTickDeltaUsec, i8& iTotalDelta);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
