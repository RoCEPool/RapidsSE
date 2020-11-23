/*============================================================================*/
/* LogEntryDdl.hpp - Log Entry for DDL                                        */
/*============================================================================*/
#pragma once

namespace Logging
{
  class _LogEntryDdl
  {
  public:
    i8   m_iJobId;  // id
    eDA  m_eDdlAct; // action
  
  public:
    void vSet(LogEntry* pLogEntry, Job* pJob, eDA eDdlAct);
  
    void vOutline(s4K& sOutline);

    void vWriteCsv(File* pFile, LogEntry* pEntry);
    void vToTxt(s4K& sInf, LogEntry* pEntry);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
