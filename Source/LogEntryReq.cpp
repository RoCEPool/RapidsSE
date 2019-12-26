/*============================================================================*/
/* LogEntryReq.cpp - Log Entry for Request                                    */
/*============================================================================*/

#include "Logging.h"

/*----------------------------------------------------------------------------*/
/* set entry                                                                  */
/*----------------------------------------------------------------------------*/
void _LogEntryReq::vSet(LogEntry* pLogEntry, Request* pRequest, eCA eReqAct)
{
  pLogEntry->m_eKind   = EK_REQ;
  pLogEntry->m_cPrefix = ' ';
  pLogEntry->m_iAct    = pRequest->iStepNo();

  //pLogEntry->m_xReqTuple.vSet(pRequest->aReqTuple());

  m_iReqId  = pRequest->m_iFocalId;
  m_eReqAct = eReqAct;
  
  if (CA_REQ_CONSTRUCT == eReqAct)
    pLogEntry->m_iIndicator = OC_CONSTRUCT;
  else
    if (CA_REQ_DESTRUCT == eReqAct)
    pLogEntry->m_iIndicator = OC_DESTRUCT;
}

/*----------------------------------------------------------------------------*/
/* cat sprint to given string                                                 */
/*----------------------------------------------------------------------------*/
void _LogEntryReq::vOutline(s4K& sOutline)
{
  SP_Gid spId;
  SP_String spString;

  sOutline.vCatSprint(".%s %s"_Stz,
                
                      spId.sz(IK_REQUEST,m_iReqId),
                      spString.szRight(aCA[m_eReqAct],24)
                      );
}

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void _LogEntryReq::vWriteCsv(File* pFile, LogEntry* pEntry)
{
  SP_Gid spId;
  
  pFile->vPrint("%s,%s"_Stz,
                
                spId.sz(IK_REQUEST,m_iReqId),
                aCA[m_eReqAct]);
}

/*----------------------------------------------------------------------------*/
/* convert entry into text                                                    */
/*----------------------------------------------------------------------------*/
void _LogEntryReq::vToTxt(s4K& sInf, LogEntry* pEntry)
{
  sInf.vSprint(C_ENTRY_FMT_TXT,
               aCN[pEntry->m_eClass],
               aCA[m_eReqAct],
               pEntry->szIndicator(),
               aCA_short[m_eReqAct],
               pEntry->m_caNote);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
