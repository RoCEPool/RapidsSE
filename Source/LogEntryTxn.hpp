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
    void vSet(LogEntry* pLogEntry, Tx* pTxn, eXA eTxnAct);
  
    void vOutline(s4K& sOutline);
  
    void vWriteCsv(File* pFile, LogEntry* pEntry);
    void vToTxt(s4K& sInf, LogEntry* pEntry);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
