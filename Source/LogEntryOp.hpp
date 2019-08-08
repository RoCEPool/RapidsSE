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

  public:
   void vSet(LogEntry* pLogEntry, i8 iJobId);
  
  void vWriteCsv(File* pFile, LogEntry* pLogEntry,
                 i8 iTickDeltaUsec, i8& iTotalDelta);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
