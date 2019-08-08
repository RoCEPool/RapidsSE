/*============================================================================*/
/* LogEntryItf.hpp - Log Entry for Interface                                  */
/*============================================================================*/
#pragma once

namespace Logging
{
  class _LogEntryItf
  {
  public:
    i8   m_iItfId;             // interface id

  public:
   void vSet(LogEntry* pLogEntry, i8 iItfId, char* aText);
  
   void vWrite(File* pFile, LogEntry* pLogEntry,
                i8 iTickDeltaUsec, i8& iTotalDelta);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
