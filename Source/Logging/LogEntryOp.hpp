/*============================================================================*/
/* LogEntryOp.hpp - Log Entry for Operator                                    */
/*============================================================================*/
#pragma once

namespace Logging
{
  class _LogEntryOp
  {
  public:
    i8  m_iOpId;  // operator id
    eOA m_eOpAct; // operator action
  
  public:
    void vSet(LogEntry* pLogEntry, Op* pOpr, eOA eOpAct);
  
    void vOutline(s4K& sOutline);

    void vWriteCsv(File* pFile, LogEntry* pEntry);
    void vToTxt(s4K& sInf, LogEntry* pEntry);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
