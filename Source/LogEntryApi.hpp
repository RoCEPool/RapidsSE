/*============================================================================*/
/* LogEntryItf.hpp - Log Entry for Api                                        */
/*============================================================================*/
#pragma once

namespace Logging
{
  class _LogEntryApi
  {
  public:
    i8   m_iSsnId;  // session id
    eAA  m_eItfAct; // interface action
  
  public:
    void vSet(LogEntry* pLogEntry, i8 iSsnId, eAA eApiAct);
  
    void vWriteCsv(File* pFile);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
