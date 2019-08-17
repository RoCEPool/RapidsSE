/*============================================================================*/
/* LogEntryObj.hpp - Log Entry for Object                                     */
/*============================================================================*/
#pragma once

namespace Logging
{
  class _LogEntryObj
  {
  public:
    Object* m_pObject; // object address
    eCA     m_eObjAct; // object action

  public:
   void vSet(LogEntry* pLogEntry, Object* pObject, eCA eObjAct);
  
  void vWriteCsv(File* pFile);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
