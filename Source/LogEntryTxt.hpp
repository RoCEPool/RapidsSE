/*============================================================================*/
/* LogEntryTxt.hpp - Log Entry Text                                           */
/*============================================================================*/
#pragma once

namespace Logging
{
  class _LogEntryTxt
  {
  public:  
    i8   m_iEnumGroup;  // enum group
    i8   m_iEnum;       // enum/jobid
    i8   m_iForeign;    // foreign job id
  
    eLN  m_eLogNote;           //
    char m_caText[C_LOG_TEXT]; // comment
  
  public:
    void vSet(LogEntry* pLogEntry, char* aText);
  
    void vWrite(File* pFile, LogEntry* pLogEntry, i8 iTickDeltaUsec, i8& iTotalDelta);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
