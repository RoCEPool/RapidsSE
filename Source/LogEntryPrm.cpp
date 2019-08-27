/*============================================================================*/
/* LogEntryPrm.cpp - Log Entry for Prompt                                     */
/*============================================================================*/

#include "Logging.h"

/*----------------------------------------------------------------------------*/
/* set entry                                                                  */
/*----------------------------------------------------------------------------*/
void _LogEntryPrm::vSet(LogEntry* pLogEntry, SePrompt*  pPrompt, ePA ePrmAct)
{
  pLogEntry->m_eKind = EK_PRM;
  
  m_iPrmId  = pPrompt->m_iUniqueId;
  m_ePrmAct = ePrmAct;
  
  if (PA_PRM_COMMAND == ePrmAct)
    pLogEntry->m_bLine = true;
}

/*----------------------------------------------------------------------------*/
/* cat sprint to given string                                                 */
/*----------------------------------------------------------------------------*/
void _LogEntryPrm::vOutline(s4K& sOutline)
{
  SP_Id spId;
  SP_String spString;

  sOutline.vCatSprint(".%s %24s"_Stz,
                
                      spId.sz(IK_PROMPT,m_iPrmId),
                      spString.szRight(aPA[m_ePrmAct],24)
                      );
}

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void _LogEntryPrm::vWriteCsv(File* pFile)
{
  SP_Id spId;
  
  pFile->vPrint(",%s,%s"_Stz,
                
                spId.sz(IK_PROMPT,m_iPrmId),
                aPA[m_ePrmAct]);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
