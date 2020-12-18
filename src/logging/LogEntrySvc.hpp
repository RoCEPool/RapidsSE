/*============================================================================*/
/* LogEntrySvc.hpp - Log Entry for Service                                    */
/*============================================================================*/
#pragma once

namespace StorageEngine
{
  class _LogEntrySvc
  {
  public:
    i8   m_iSvcId;   // service id
    eVA  m_eSvcAct;  // service action

  public:
    void vSet(LogEntry* pLogEntry, Job* pJob, eVA eSvcAct);
  
    void vOutline(s4K& sOutline);

    void vWriteCsv(File* pFile, LogEntry* pEntry);
    void vToTxt(s4K& sInf, LogEntry* pEntry);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
