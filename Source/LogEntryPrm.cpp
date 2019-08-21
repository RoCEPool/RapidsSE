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
  
  if (PA_COMMAND == ePrmAct)
    pLogEntry->m_bLine = true;
}

/*----------------------------------------------------------------------------*/
/* cat sprint to given string                                                 */
/*----------------------------------------------------------------------------*/
void _LogEntryPrm::vSprint(s4K& sString)
{
  SP_Id spId;
  
  sString.vCatSprint(",%s,%s"_Stz,
                
                spId.sz("P"_Stz,m_iPrmId),
                aPA[m_ePrmAct]);
}

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void _LogEntryPrm::vWriteCsv(File* pFile)
{
  SP_Id spId;
  
  pFile->vPrint(",%s,%s"_Stz,
                
                spId.sz("P"_Stz,m_iPrmId),
                aPA[m_ePrmAct]);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
