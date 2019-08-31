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
    eMA  m_eReqAct; // command action
  
  public:
    void vSet(LogEntry* pLogEntry, Request* pRequest, eMA eReqAct);
  
    void vOutline(s4K& sOutline);

    void vWriteCsv(File* pFile, LogEntry* pEntry);
    void vWriteTxt(s256& sInf, LogEntry* pEntry);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
