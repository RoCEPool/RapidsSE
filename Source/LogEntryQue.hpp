/*============================================================================*/
/* LogEntryQue.hpp - Log Entry for Queues                                     */
/*============================================================================*/
#pragma once

namespace Logging
{
  class _LogEntryQue
  {
  public:
    i8   m_iQueId;  // queues id
    eQA  m_eQueAct; // queues action
  
  public:
    void vSet(LogEntry* pLogEntry, Queues* pQueues, eQA eQueAct);
  
    void vOutline(s4K& sOutline);

    void vWriteCsv(File* pFile, LogEntry* pEntry);
    void vWriteTxt(s256& sInf, File* pFile, LogEntry* pEntry);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
