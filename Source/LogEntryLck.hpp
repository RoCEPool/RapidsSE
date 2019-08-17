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
    eIA  m_eLckAct; // interface action
  
  public:
    void vSet(LogEntry* pLogEntry, i8 iLckId, eIA eLckAct);
  
    void vWriteCsv(File* pFile);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
