/*============================================================================*/
/* LogEntryRes.hpp - Log Entry for Resource                                   */
/*============================================================================*/
#pragma once

namespace Logging
{
  class _LogEntryRes
  {
  public:
  eCN  m_eClass;     // class
  i8   m_iResId;     // resource identifier
  eRA  m_eResAct;    // resource action

  public:
    void vSet(LogEntry* pLogEntry, Resource*  pResource, eRA eResAct);
  
    void vOutline(s4K& sOutline);

    void vWriteCsv(File* pFile, LogEntry* pEntry);
    void vWriteTxt(s256& sInf, LogEntry* pEntry);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
