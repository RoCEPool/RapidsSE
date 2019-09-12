/*============================================================================*/
/* LogEntryTxn.cpp - Log Entry for Transaction                                */
/*============================================================================*/

#include "Logging.h"

/*----------------------------------------------------------------------------*/
/* set transaction entry                                                      */
/*----------------------------------------------------------------------------*/
void _LogEntryTxn::vSet(LogEntry* pLogEntry, Tx* pTxn, eXA eTxnAct)
{
  pLogEntry->m_eKind = EK_TXN;
  pLogEntry->m_cPrefix = 'X';

  m_iTxnId  = pTxn->m_iUniqueId;
  m_eTxnAct = eTxnAct;
  
}

/*----------------------------------------------------------------------------*/
/* cat sprint to given string                                                 */
/*----------------------------------------------------------------------------*/
void _LogEntryTxn::vOutline(s4K& sOutline)
{
  SP_Id spId;
  SP_String spString;

  sOutline.vCatSprint(".%s %24s"_Stz,
                
                      spId.sz(IK_TX,m_iTxnId),
                      spString.szRight(aXA[m_eTxnAct],24)
                      );
}

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void _LogEntryTxn::vWriteCsv(File* pFile, LogEntry* pEntry)
{
  SP_Id spId;
  
  pFile->vPrint("%s,%s"_Stz,
                
                spId.sz(IK_TX,m_iTxnId),
                aXA[m_eTxnAct]);
}

/*----------------------------------------------------------------------------*/
/* convert entry into text                                                    */
/*----------------------------------------------------------------------------*/
void _LogEntryTxn::vToTxt(s4K& sInf, LogEntry* pEntry)
{
  sInf.vSprint(C_ENTRY_FMT_TXT,
               aCN[pEntry->m_eClass],
               aXA[m_eTxnAct],
               pEntry->szIndicator(),
               aXA_short[m_eTxnAct],
               pEntry->m_caNote);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
