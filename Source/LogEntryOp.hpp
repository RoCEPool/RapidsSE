/*============================================================================*/
/* LogEntryOp.hpp - Log Entry for Operator                                    */
/*============================================================================*/
#pragma once

namespace Logging
{
  class _LogEntryOp
  {
  public:
    i8   m_iOpId;              // opid
    char m_caText[C_LOG_TEXT]; // comment

  public:
   void vSet(LogEntry* pLogEntry, i8 iJobId, char* aText);
  
   void vWrite(File* pFile, LogEntry* pLogEntry,
                i8 iTickDeltaUsec, i8& iTotalDelta);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
