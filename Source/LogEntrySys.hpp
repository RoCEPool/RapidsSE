/*============================================================================*/
/* LogEntrySys.hpp - Log Entry for System                                     */
/*============================================================================*/
#pragma once

namespace Logging
{
  class _LogEntrySys
  {
  public:
    i8   m_iSysId;  // system id
    eYA  m_eSysAct; // system action
  
  public:
    void vSet(LogEntry* pLogEntry, i8 iSysId, eYA eSysAct);
  
    void vWriteCsv(File* pFile);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
