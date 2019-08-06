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
    char m_caText[C_LOG_TEXT]; // comment

  public:
   void vSet(LogEntry* pLogEntry, i8 iTskId, char* aText);
  
   void vWrite(File* pFile, LogEntry* pLogEntry,
                i8 iTickDeltaUsec, i8& iTotalDelta);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
