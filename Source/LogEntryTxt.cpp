/*============================================================================*/
/* LogEntryTxt.cpp - Log Entry Text                                           */
/*============================================================================*/

#include "Logging.h"

/*----------------------------------------------------------------------------*/
/* set                                                                        */
/*----------------------------------------------------------------------------*/
void _LogEntryTxt::vSet(LogEntry* pLogEntry, char* aText)
{  
  pLogEntry->m_eKind = EK_TEXT;
  
  m_iEnumGroup = 0;
  m_iEnum      = 0;
  
  StrLib::vCopyLen(m_caText,
                   aText,
                   sizeof(m_caText) - 1);
}

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void _LogEntryTxt::vWrite(File* pFile, LogEntry* pLogEntry,
                           i8 iTickDeltaUsec, i8& iTotalDelta)
{
  pFile->vPrint("%lld,%lld,%lld,%lld,"
                "%s,%s,%s\n"_Stz,
                
                pLogEntry->m_i_Entry,
                pLogEntry->m_iTick,
                iTickDeltaUsec,
                iTotalDelta,
              
                pLogEntry->m_caNode,
                aEK[pLogEntry->m_eKind],
                pLogEntry->m_xText.m_caText);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
