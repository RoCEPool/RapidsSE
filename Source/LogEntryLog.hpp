/*============================================================================*/
/* LogEntryLog.hpp - Log Entry for Log                                        */
/*============================================================================*/
#pragma once

namespace Logging
{
  class _LogEntryLog
  {
  public:
    i8   m_iLogId;  // log id
    eGA  m_eLogAct; // log action
  
  public:
    void vSet(LogEntry* pLogEntry, Log* pLog, i8 iLogId, eGA eLogAct);
  
    void vWriteCsv(File* pFile);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
