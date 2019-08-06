/*============================================================================*/
/* LogEntryText.cpp - Log Entry Text                                          */
/*============================================================================*/

#include "Logging.h"

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void _LogEntryText::vWrite(File* pFile, LogEntry* pLogEntry,
                           i8 iTickDeltaUsec, i8& iTotalDelta)
{
  pFile->vPrint("%lld,%lld,%lld,%lld,%s,inf,%s\n"_Stz,
                pLogEntry->m_i_Entry,
                pLogEntry->m_iTick,
                iTickDeltaUsec,
                iTotalDelta,
              
                pLogEntry->m_caNode,
                pLogEntry->m_xText.m_caText);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
