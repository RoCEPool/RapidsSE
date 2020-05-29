/*============================================================================*/
/* LogEntryReq.hpp - Log Entry for Request                                    */
/*============================================================================*/
#pragma once

namespace Logging
{
  class _LogEntryReq
  {
  public:
    i8   m_iReqId;  // command id
    eCA  m_eReqAct; // command action
  
  public:
    void vSet(LogEntry* pLogEntry, Request* pRequest, eCA eReqAct);
  
    void vOutline(s4K& sOutline);

    void vWriteCsv(File* pFile, LogEntry* pEntry);
    void vToTxt(s4K& sInf, LogEntry* pEntry);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
