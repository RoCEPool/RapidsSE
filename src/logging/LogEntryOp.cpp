/*============================================================================*/
/* LogEntryOp.cpp - Log Entry for Operator                                    */
/*============================================================================*/

#include "StorageEngine.h"

#include "eOC.h"

#include "File.hpp" //?

/*----------------------------------------------------------------------------*/
/* set resource entry                                                         */
/*----------------------------------------------------------------------------*/
void _LogEntryOp::vSet(LogEntry* pLogEntry, Op* pOp, eOA eOpAct)
{
  pLogEntry->m_eKind = LG_OPR;
  pLogEntry->m_cPrefix = 'O';

  m_iOpId  = pOp->m_iFocalId;
  m_eOpAct = eOpAct;
  
  if (OA_OP_CONSTRUCT == eOpAct)
    pLogEntry->m_iIndicator = OC_CONSTRUCT;
  else
    if (OA_OP_DESTRUCT == eOpAct)
      pLogEntry->m_iIndicator = OC_DESTRUCT;
}

/*----------------------------------------------------------------------------*/
/* cat sprint to given string                                                 */
/*----------------------------------------------------------------------------*/
void _LogEntryOp::vOutline(s4K& sOutline)
{
  SP_Gid spId;
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
  SP_Gid spId;
  
  pFile->vPrint("%s,%s"_Stz,
                
                spId.sz(IK_OP,m_iOpId),
                aOA[m_eOpAct]);
}

/*----------------------------------------------------------------------------*/
/* convert entry into text                                                    */
/*----------------------------------------------------------------------------*/
void _LogEntryOp::vToTxt(s4K& sInf, LogEntry* pEntry)
{
  sInf.vSprint(C_ENTRY_FMT_TXT,
               aCN[pEntry->m_eClass],
               aOA[m_eOpAct],
               pEntry->szIndicator(),
               aOA_short[m_eOpAct],
               pEntry->m_caNote);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
