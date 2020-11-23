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
    void vSet(LogEntry* pLogEntry, Log* pLog, eGA eLogAct);
  
    void vOutline(s4K& sOutline);

    void vWriteCsv(File* pFile, LogEntry* pEntry);
    void vToTxt(s4K& sInf, LogEntry* pEntry);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
