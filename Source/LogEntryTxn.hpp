/*============================================================================*/
/* LogEntryTxn.hpp - Log Entry for Operator                                    */
/*============================================================================*/
#pragma once

namespace Logging
{
  class _LogEntryTxn
  {
  public:
    i8   m_iTxnId;             // transaction id

  public:
   void vSet(LogEntry* pLogEntry, i8 iTxnId, char* aText);
  
   void vWrite(File* pFile, LogEntry* pLogEntry,
                i8 iTickDeltaUsec, i8& iTotalDelta);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
