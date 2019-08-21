/*============================================================================*/
/* LogEntryTxn.hpp - Log Entry for Transaction                                */
/*============================================================================*/
#pragma once

namespace Logging
{
  class _LogEntryTxn
  {
  public:
    i8   m_iTxnId;   // transaction id
    eXA  m_eTxnAct;  // transaction action

  public:
    void vSet(LogEntry* pLogEntry, Tx* pTxn, i8 iTxnId, eXA  eTxnAct);
  
    void vWriteCsv(File* pFile);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
