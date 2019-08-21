/*============================================================================*/
/* LogEntryRes.hpp - Log Entry for Resource                                   */
/*============================================================================*/
#pragma once

namespace Logging
{
  class _LogEntryRes
  {
  public:
  i8   m_iResId;     // resource identifier
  eRK  m_eResKnd;    // resource kind
  eRA  m_eResAct;    // resource action

  public:
   void vSet(LogEntry* pLogEntry, Resource*  pResource, i8 iResId, eRK eResKnd, eRA eResAct);
  
  void vWriteCsv(File* pFile);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
