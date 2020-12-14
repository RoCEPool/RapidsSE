/*============================================================================*/
/* LogEntryPrm.cpp - Log Entry for Prompt                                     */
/*============================================================================*/

#include "Logging.h"

#include "File.hpp" //?

/*----------------------------------------------------------------------------*/
/* set entry                                                                  */
/*----------------------------------------------------------------------------*/
void _LogEntryPrm::vSet(LogEntry* pLogEntry, SePrompt*  pPrompt, ePA ePrmAct)
{
  pLogEntry->m_eKind   = LG_PRM;
  pLogEntry->m_cPrefix = 'P';
  pLogEntry->m_iAct    = pPrompt->iStepNo();

  m_iPrmId  = pPrompt->m_iFocalId;
  m_ePrmAct = ePrmAct;
  
  if (PA_PRM_COMMAND == ePrmAct)
    pLogEntry->m_bLine = true;
  
       if (PA_PRM_WAITS      == ePrmAct)  pLogEntry->m_iIndicator = OC_WAIT;
  else if (PA_PRM_RESUMES    == ePrmAct)  pLogEntry->m_iIndicator = OC_RESUME;
  else if (PA_PRM_COMMAND    == ePrmAct)  pLogEntry->m_iIndicator = OC_COMMAND;
  else if (PA_PRM_FREE_SPACE == ePrmAct)  pLogEntry->m_iIndicator = OC_SPACE;
}

/*----------------------------------------------------------------------------*/
/* cat sprint to given string                                                 */
/*----------------------------------------------------------------------------*/
void _LogEntryPrm::vOutline(s4K& sOutline)
{
  SP_Gid spId;
  SP_String spString;

  sOutline.vCatSprint(".%s %24s"_Stz,
                
                      spId.sz(IK_PROMPT,m_iPrmId),
                      spString.szRight(aPA[m_ePrmAct],24)
                      );
}

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void _LogEntryPrm::vWriteCsv(File* pFile, LogEntry* pEntry)
{
  SP_Gid spId;
  
  pFile->vPrint("%s,%s"_Stz,
                
                spId.sz(IK_PROMPT,m_iPrmId),
                aPA[m_ePrmAct]);
}

/*----------------------------------------------------------------------------*/
/* convert entry into text                                                    */
/*----------------------------------------------------------------------------*/
void _LogEntryPrm::vToTxt(s4K& sInf, LogEntry* pEntry)
{
  sInf.vSprint(C_ENTRY_FMT_TXT,
               aCN[pEntry->m_eClass],
               aPA[m_ePrmAct],
               pEntry->szIndicator(),
               aPA_short[m_ePrmAct],
               pEntry->m_caNote);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
