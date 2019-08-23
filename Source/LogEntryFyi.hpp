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

    void vWriteCsv(File* pFile);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
