/*============================================================================*/
/* LogEntrySvc.cpp - Log Entry for Service                                    */
/*============================================================================*/

#include "Logging.h"

/*----------------------------------------------------------------------------*/
/* set resource entry                                                         */
/*----------------------------------------------------------------------------*/
void _LogEntrySvc::vSet(LogEntry* pLogEntry, i8 iSvcId, char* aText)
{
  pLogEntry->m_eKind = EK_SVC;
  
  m_iSvcId = iSvcId;
  
  StrLib::vCopyLen(m_caText,aText,sizeof(m_caText) - 1);
}

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void _LogEntrySvc::vWrite(File* pFile, LogEntry* pLogEntry,
                          i8 iTickDeltaUsec, i8& iTotalDelta)
{
  pFile->vPrint("%lld,%lld,%lld,%lld,"
                "%s,%s,"
                "%lld,%s\n"_Stz,

                pLogEntry->m_i_Entry,
                pLogEntry->m_iTick,
                iTickDeltaUsec,
                iTotalDelta,
                
                pLogEntry->m_caNode,
                aEK[pLogEntry->m_eKind],
                
                m_iSvcId,
                m_caText);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
