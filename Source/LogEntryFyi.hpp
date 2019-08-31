/*============================================================================*/
/* LogEntryFyi.hpp - Log Entry for Info                                       */
/*============================================================================*/
#pragma once

namespace Logging
{
  class _LogEntryFyi
  {
  public:  
  
  public:
    void vSet(LogEntry* pLogEntry);

    void vOutline(s4K& sOutline);

    void vWriteCsv(File* pFile, LogEntry* pEntry);
    void vToTxt(s256& sInf, LogEntry* pEntry);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
