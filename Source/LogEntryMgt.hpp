/*============================================================================*/
/* LogEntryMgt.hpp - Log Entry for Management Actions                         */
/*============================================================================*/
#pragma once

namespace Logging
{
  class _LogEntryMgt
  {
  public:
    i8   m_iJobId;  // id
    eDA  m_eDdlAct; // action
  
  public:
    void vSet(LogEntry* pLogEntry, Job* pJob, eDA eDdlAct);
  
    void vOutline(s4K& sOutline);

    void vWriteCsv(File* pFile, LogEntry* pEntry);
    void vToTxt(s256& sInf, LogEntry* pEntry);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
