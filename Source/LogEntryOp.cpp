/*============================================================================*/
/* LogEntryOp.cpp - Log Entry for Operator                                    */
/*============================================================================*/

#include "Logging.h"

/*----------------------------------------------------------------------------*/
/* set resource entry                                                         */
/*----------------------------------------------------------------------------*/
void _LogEntryOp::vSet(LogEntry* pLogEntry, Op* pOp, eOA eOpAct)
{
  pLogEntry->m_eKind = EK_OPR;
  pLogEntry->m_cPrefix = 'O';

  m_iOpId  = pOp->m_iUniqueId;
  m_eOpAct = eOpAct;
}

/*----------------------------------------------------------------------------*/
/* cat sprint to given string                                                 */
/*----------------------------------------------------------------------------*/
void _LogEntryOp::vOutline(s4K& sOutline)
{
  SP_Id spId;
  SP_String spString;

  sOutline.vCatSprint(".%s %24s"_Stz,
                
                      spId.sz(IK_OP,m_iOpId),
                      spString.szRight(aOA[m_eOpAct],24)
                      );
}

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void _LogEntryOp::vWriteCsv(File* pFile, LogEntry* pEntry)
{
  SP_Id spId;
  
  pFile->vPrint("%s,%s"_Stz,
                
                spId.sz(IK_OP,m_iOpId),
                aOA[m_eOpAct]);
}

/*----------------------------------------------------------------------------*/
/* write to text file                                                         */
/*----------------------------------------------------------------------------*/
void _LogEntryOp::vWriteTxt(File* pFile, LogEntry* pEntry)
{
  SP_Id spId;
  
  pFile->vPrint(C_ENTRY_FMT_2s,
                
                aOA[m_eOpAct],
                spId.sz(IK_OP,m_iOpId)
                );
  
  pFile->vPrint("%s"_Stz,pEntry->m_caNote);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
