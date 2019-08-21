/*============================================================================*/
/* LogEntrySsn.hpp - Log Entry for Session                                     */
/*============================================================================*/
#pragma once

namespace Logging
{
  class _LogEntrySsn
  {
  public:
    i8   m_iSsnId;  // session id
    eAA  m_eSsnAct; // session action
  
  public:
    void vSet(LogEntry* pLogEntry, Session* pSession, eAA eSsnAct);
  
    void vSprint(s4K& sString);
  
    void vWriteCsv(File* pFile);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
