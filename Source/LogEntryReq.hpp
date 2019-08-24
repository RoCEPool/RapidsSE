/*============================================================================*/
/* LogEntryReq.hpp - Log Entry for Command                                    */
/*============================================================================*/
#pragma once

namespace Logging
{
  class _LogEntryReq
  {
  public:
    i8   m_iReqId;  // command id
    eMA  m_eReqAct; // command action
  
  public:
    void vSet(LogEntry* pLogEntry, Command* pCommand, eMA eReqAct);
  
    void vOutline(s4K& sOutline);

    void vWriteCsv(File* pFile);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
