/*============================================================================*/
/* LogEntryPrm.hpp - Log Entry for Prompt                                     */
/*============================================================================*/
#pragma once

namespace Logging
{
  class _LogEntryPrm
  {
  public:
    i8  m_iPrmId;  // prompt id
    ePA m_ePrmAct; // prompt action
  
  public:
   void vSet(LogEntry* pLogEntry, SePrompt* pSePrompt, ePA ePrmAct);
  
    void vSprint(s4K& sString);
  
   void vWriteCsv(File* pFile);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
