/*============================================================================*/
/* LogEntryThd.hpp - Log Entry for Thread                                     */
/*============================================================================*/
#pragma once

namespace Logging
{
  class _LogEntryThd
  {
  public:
    i8  m_iThdId;  // thread id
    eHA m_eThdAct; // thread action

  public:
    void vSet(LogEntry* pLogEntry, Thread* pThread, eHA eThdAct);
  
    void vOutline(s4K& sOutline);

    void vWriteCsv(File* pFile, LogEntry* pEntry);
    void vWriteTxt(s256& sInf, LogEntry* pEntry);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
