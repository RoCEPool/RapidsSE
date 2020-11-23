/*============================================================================*/
/* LogEntryLck.hpp - Log Entry for Lock                                       */
/*============================================================================*/
#pragma once

namespace Logging
{
  class _LogEntryLck
  {
  public:
    i8   m_iLckId;  // interface id
    eLA  m_eLckAct; // interface action
  
  public:
    void vSet(LogEntry* pLogEntry, Lock* pLock, eLA eLckAct);
  
    void vOutline(s4K& sOutline);

    void vWriteCsv(File* pFile, LogEntry* pEntry);
    void vToTxt(s4K& sInf, LogEntry* pEntry);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
