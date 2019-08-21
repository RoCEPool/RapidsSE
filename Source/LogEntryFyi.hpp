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

    void vSprint(s4K& sString);

    void vWriteCsv(File* pFile);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
