/*============================================================================*/
/* LogEntryTxt.hpp - Log Entry Text                                           */
/*============================================================================*/
#pragma once

namespace Logging
{
  static const i8 C_LOG_TEXT = 128;
  
  class _LogEntryTxt
  {
  public:
    i8   m_iEnumGroup;  // enum group
    i8   m_iEnum;       // enum/jobid
    i8   m_iForeign;    // foreign job id
  
    char m_caText[C_LOG_TEXT]; // comment
  
  public:
    void vWrite(File* pFile, LogEntry* pLogEntry, i8 iTickDeltaUsec, i8& iTotalDelta);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
