/*============================================================================*/
/* LogEntryRes.hpp - Log Entry for Resource                                   */
/*============================================================================*/
#pragma once

namespace Logging
{
  class _LogEntryRes
  {
  public:
    eRK  m_eSysRes;  // resource kind
    i8   m_iResId;     // resource identifier

  public:
   void vSet(LogEntry* pLogEntry, eRK eSysRes, i8 iResId);
  
   void vWrite(File* pFile, LogEntry* pLogEntry,
                i8 iTickDeltaUsec, i8& iTotalDelta);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
