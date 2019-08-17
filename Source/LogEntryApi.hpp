/*============================================================================*/
/* LogEntryItf.hpp - Log Entry for Interface                                  */
/*============================================================================*/
#pragma once

namespace Logging
{
  class _LogEntryApi
  {
  public:
    i8   m_iItfId;  // interface id
    eAA  m_eItfAct; // interface action
  
  public:
    void vSet(LogEntry* pLogEntry, i8 iItfId, eAA eApiAct);
  
    void vWriteCsv(File* pFile);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
