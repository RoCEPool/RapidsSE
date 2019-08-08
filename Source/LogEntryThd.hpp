/*============================================================================*/
/* LogEntryThd.hpp - Log Entry for Operator                                    */
/*============================================================================*/
#pragma once

namespace Logging
{
  class _LogEntryThd
  {
  public:
    i8   m_iThdId;             // thread id

  public:
   void vSet(LogEntry* pLogEntry, i8 iThdId, char* aText);
  
   void vWrite(File* pFile, LogEntry* pLogEntry,
                i8 iTickDeltaUsec, i8& iTotalDelta);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
