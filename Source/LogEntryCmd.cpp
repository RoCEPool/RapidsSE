/*============================================================================*/
/* LogEntryCmd.cpp - Log Entry for Command                                    */
/*============================================================================*/

#include "Logging.h"

/*----------------------------------------------------------------------------*/
/* set entry                                                                  */
/*----------------------------------------------------------------------------*/
void _LogEntryCmd::vSet(LogEntry* pLogEntry, void* aCommand, eMA eCmdAct)
{
  pLogEntry->m_eKind = EK_LCK;
  
  m_iCmdId  = C_UNKNOWN; //pCommand->m_iUniqueId;
  m_eCmdAct = eCmdAct;
}

/*----------------------------------------------------------------------------*/
/* cat sprint to given string                                                 */
/*----------------------------------------------------------------------------*/
void _LogEntryCmd::vOutline(s4K& sOutline)
{
  SP_Id spId;
  SP_String spString;

  sOutline.vCatSprint(".%s %s"_Stz,
                
                      spId.sz("M"_Stz,m_iCmdId),
                      spString.szRight(aMA[m_eCmdAct],24)
                      );
}

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void _LogEntryCmd::vWriteCsv(File* pFile)
{
  SP_Id spId;
  
  pFile->vPrint(",%s,%s"_Stz,
                
                spId.sz("L"_Stz,m_iCmdId),
                aMA[m_eCmdAct]);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
