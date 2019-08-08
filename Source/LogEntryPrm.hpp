/*============================================================================*/
/* LogEntryPrm.hpp - Log Entry for Interface                                  */
/*============================================================================*/
#pragma once

namespace Logging
{
  class _LogEntryPrm
  {
  public:
    i8   m_iPrmId;             // prompt id

  public:
   void vSet(LogEntry* pLogEntry, i8 iPrmId);
  
  void vWriteCsv(File* pFile, LogEntry* pLogEntry,
                 i8 iTickDeltaUsec, i8& iTotalDelta);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
