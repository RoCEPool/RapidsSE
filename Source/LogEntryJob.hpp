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
  
    i8   m_iReqNo;  // request id
  
  public:
    void vSet(LogEntry* pLogEntry, Job* pJob, eJA eJobAct);
  
    void vOutline(s4K& sOutline);

    void vWriteCsv(File* pFile, LogEntry* pEntry);
    void vToTxt(s4K& sInf, LogEntry* pEntry);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
