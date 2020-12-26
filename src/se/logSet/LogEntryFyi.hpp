/*============================================================================*/
/* LogEntryFyi.hpp - Log Entry for Info                                       */
/*============================================================================*/
#pragma once

namespace StorageEngine
{
  class _LogEntryFyi
  {
  public:  
  
  public:
    void vSet(LogEntry* pLogEntry);

    void vOutline(s4K& sOutline);

    void vWriteCsv(File* pFile, LogEntry* pEntry);
    void vToTxt(s4K& sInf, LogEntry* pEntry);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
