/*============================================================================*/
/* LogEntryCmd.hpp - Log Entry for Command                                    */
/*============================================================================*/
#pragma once

namespace Logging
{
  class _LogEntryCmd
  {
  public:
    i8   m_iCmdId;  // command id
    eMA  m_eCmdAct; // command action
  
  public:
    void vSet(LogEntry* pLogEntry, void* aCommand, eMA eCmdAct);
  
    void vOutline(s4K& sOutline);

    void vWriteCsv(File* pFile);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
