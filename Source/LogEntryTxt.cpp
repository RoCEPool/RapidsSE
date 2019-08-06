/*============================================================================*/
/* LogEntryTxt.cpp - Log Entry Text                                           */
/*============================================================================*/

#include "Logging.h"

/*----------------------------------------------------------------------------*/
/* set text entry                                                             */
/*----------------------------------------------------------------------------*/
void _LogEntryTxt::vSet(LogEntry* pLogEntry, char* aText)
{  
  pLogEntry->m_eKind = EK_TXT;

  m_eLogNote = LN_TEXT;
  
  StrLib::vCopyLen(m_caText,aText,sizeof(m_caText) - 1);
}

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void _LogEntryTxt::vWrite(File* pFile, LogEntry* pLogEntry,
                           i8 iTickDeltaUsec, i8& iTotalDelta)
{
  pFile->vPrint("%lld,%lld,%lld,%lld,"
                "%s,%s,%s,%s\n"_Stz,
                
                pLogEntry->m_i_Entry,
                pLogEntry->m_iTick,
                iTickDeltaUsec,
                iTotalDelta,
              
                pLogEntry->m_caNode,
                aEK[pLogEntry->m_eKind],
                aLN[m_eLogNote],
                m_caText);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
