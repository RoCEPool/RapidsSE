/*============================================================================*/
/* LogEntryQue.hpp - Log Entry for Queues                                     */
/*============================================================================*/
#pragma once

namespace StorageEngine
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
    void vToTxt(s4K& sInf, LogEntry* pEntry);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
