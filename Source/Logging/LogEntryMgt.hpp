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
    eMA  m_eMgtAct; // action
  
  public:
    void vSet(LogEntry* pLogEntry, Job* pJob, eMA eMgtAct);
  
    void vOutline(s4K& sOutline);

    void vWriteCsv(File* pFile, LogEntry* pEntry);
    void vToTxt(s4K& sInf, LogEntry* pEntry);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
