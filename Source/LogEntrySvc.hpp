/*============================================================================*/
/* LogEntrySvc.hpp - Log Entry for Service                                    */
/*============================================================================*/
#pragma once

namespace Logging
{
  class _LogEntrySvc
  {
  public:
    i8   m_iSvcId;             // service id
    char m_caText[C_LOG_TEXT]; // comment

  public:
   void vSet(LogEntry* pLogEntry, i8 iSvcId, char* aText);
  
   void vWrite(File* pFile, LogEntry* pLogEntry,
                i8 iTickDeltaUsec, i8& iTotalDelta);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
