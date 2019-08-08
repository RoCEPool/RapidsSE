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

  public:
   void vSet(LogEntry* pLogEntry, i8 iSvcId);
  
  void vWriteCsv(File* pFile, LogEntry* pLogEntry,
                 i8 iTickDeltaUsec, i8& iTotalDelta);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
