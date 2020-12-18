/*============================================================================*/
/* LogEntrySys.hpp - Log Entry for System                                     */
/*============================================================================*/
#pragma once

namespace StorageEngine
{
  class _LogEntrySys
  {
  public:
    i8   m_iSysId;  // system id
    eYA  m_eSysAct; // system action
  
  public:
    void vSet(LogEntry* pLogEntry, AttachMap* pAttachMap, eYA eSysAct);
  
    void vOutline(s4K& sOutline);

    void vWriteCsv(File* pFile, LogEntry* pEntry);
    void vToTxt(s4K& sInf, LogEntry* pEntry);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
