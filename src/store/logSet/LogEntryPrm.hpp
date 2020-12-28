/*============================================================================*/
/* LogEntryPrm.hpp - Log Entry for PROMPT                                     */
/*============================================================================*/
#pragma once

namespace STORE
{
  class _LogEntryPrm
  {
  public:
    i8  m_iPrmId;  // prompt id
    ePA m_ePrmAct; // prompt action
  
  public:
    void vSet(LogEntry* pLogEntry, SePrompt* pSePrompt, ePA ePrmAct);
  
    void vOutline(s4K& sOutline);

    void vWriteCsv(File* pFile, LogEntry* pEntry);
    void vToTxt(s4K& sInf, LogEntry* pEntry);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
