/*============================================================================*/
/* LogEntryRes.cpp - Log Entry for Resource                                   */
/*============================================================================*/

#include "Logging.h"

/*----------------------------------------------------------------------------*/
/* set resource entry                                                         */
/*----------------------------------------------------------------------------*/
void _LogEntryRes::vSet(LogEntry* pLogEntry, eSR eSysRes, char* aText)
{
  pLogEntry->m_eKind = EK_RES;
  
  m_eSysRes = eSysRes;
  
  StrLib::vCopyLen(m_caText,aText,sizeof(m_caText) - 1);
}

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void _LogEntryRes::vWrite(File* pFile, LogEntry* pLogEntry,
                          i8 iTickDeltaUsec, i8& iTotalDelta)
{
  pFile->vPrint("%lld,%lld,%lld,%lld,"
                "%s,%s,%s,"
                "%s\n"_Stz,

                pLogEntry->m_i_Entry,
                pLogEntry->m_iTick,
                iTickDeltaUsec,
                iTotalDelta,
                
                pLogEntry->m_caNode,
                aEK[pLogEntry->m_eKind],
                aSR_Short[m_eSysRes],
                
                m_caText);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
