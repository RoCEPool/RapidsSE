/*============================================================================*/
/* LogEntryItf.hpp - Log Entry for Interface                                  */
/*============================================================================*/
#pragma once

namespace Logging
{
  class _LogEntryItf
  {
  public:
    i8   m_iItfId;  // interface id
    eIA  m_eItfAct; // interface action
  
  public:
    void vSet(LogEntry* pLogEntry, i8 iItfId, eIA eItfAct);
  
    void vWriteCsv(File* pFile);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
