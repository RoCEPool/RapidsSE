/*============================================================================*/
/* LogEntryTsk.hpp - Log Entry for Task                                       */
/*============================================================================*/
#pragma once

namespace Logging
{
  class _LogEntryTsk
  {
  public:
    i8   m_iTskId;             // task id

  public:
   void vSet(LogEntry* pLogEntry, i8 iTskId);
  
  void vWriteCsv(File* pFile, LogEntry* pLogEntry,
                 i8 iTickDeltaUsec, i8& iTotalDelta);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
