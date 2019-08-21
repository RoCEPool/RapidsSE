/*============================================================================*/
/* LogEntryJob.hpp - Log Entry for Job                                        */
/*============================================================================*/
#pragma once

namespace Logging
{
  class _LogEntryJob
  {
  public:  
    i8   m_iJobId;  // job id
    eJA  m_eJobAct; // job action
  
  public:
    void vSet(LogEntry* pLogEntry, Job* pJob, eJA eJobAct);
  
    void vSprint(s4K& sString);
  
    void vWriteCsv(File* pFile);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
