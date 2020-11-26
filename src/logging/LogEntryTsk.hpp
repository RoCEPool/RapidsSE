/*============================================================================*/
/* LogEntryTsk.hpp - Log Entry for Task                                       */
/*============================================================================*/
#pragma once

namespace Logging
{
  class _LogEntryTsk
  {
  public:
    i8   m_iTskId;  // task id
    eTA  m_eTskAct; // task action

  public:
    void vSet(LogEntry* pLogEntry, Task* pTask, eTA eTskAct);
 
    void vOutline(s4K& sOutline);

    void vWriteCsv(File* pFile, LogEntry* pEntry);
    void vToTxt(s4K& sInf, LogEntry* pEntry);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
