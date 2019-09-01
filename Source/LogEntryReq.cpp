/*============================================================================*/
/* LogEntryReq.cpp - Log Entry for Request                                    */
/*============================================================================*/

#include "Logging.h"

/*----------------------------------------------------------------------------*/
/* set entry                                                                  */
/*----------------------------------------------------------------------------*/
void _LogEntryReq::vSet(LogEntry* pLogEntry, Request* pRequest, eMA eReqAct)
{
  pLogEntry->m_eKind   = EK_REQ;
  pLogEntry->m_cPrefix = ' ';
  pLogEntry->m_iAct    = pRequest->iActNo();

  pLogEntry->m_xReqTuple.vSet(pRequest->aReqTuple());

  m_iReqId  = pRequest->m_iUniqueId;
  m_eReqAct = eReqAct;
  
}

/*----------------------------------------------------------------------------*/
/* cat sprint to given string                                                 */
/*----------------------------------------------------------------------------*/
void _LogEntryReq::vOutline(s4K& sOutline)
{
  SP_Id spId;
  SP_String spString;

  sOutline.vCatSprint(".%s %s"_Stz,
                
                      spId.sz(IK_REQUEST,m_iReqId),
                      spString.szRight(aMA[m_eReqAct],24)
                      );
}

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void _LogEntryReq::vWriteCsv(File* pFile, LogEntry* pEntry)
{
  SP_Id spId;
  
  pFile->vPrint("%s,%s"_Stz,
                
                spId.sz(IK_REQUEST,m_iReqId),
                aMA[m_eReqAct]);
}

/*----------------------------------------------------------------------------*/
/* convert entry into text                                                    */
/*----------------------------------------------------------------------------*/
void _LogEntryReq::vToTxt(s256& sInf, LogEntry* pEntry)
{
  sInf.vSprint(C_ENTRY_FMT_TXT,
               aCN[pEntry->m_eClass],
               aMA[m_eReqAct],
               pEntry->szIndicator(),
               pEntry->m_caNote);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
