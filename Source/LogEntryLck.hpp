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
    void vSet(LogEntry* pLogEntry, i8 iLckId, eLA eLckAct);
  
    void vWriteCsv(File* pFile);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
