/*============================================================================*/
/* LogEntryOp.cpp - Log Entry for Operator                                    */
/*============================================================================*/

#include "Logging.h"

/*----------------------------------------------------------------------------*/
/* set resource entry                                                         */
/*----------------------------------------------------------------------------*/
void _LogEntryOp::vSet(LogEntry* pLogEntry, i8 iOpId, char* aText)
{
  pLogEntry->m_eKind = EK_OP;
  
  m_iOpId = iOpId;
  
  StrLib::vCopyLen(m_caText,aText,sizeof(m_caText) - 1);
}

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void _LogEntryOp::vWrite(File* pFile, LogEntry* pLogEntry,
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
                
                m_iOpId,
                m_caText);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
