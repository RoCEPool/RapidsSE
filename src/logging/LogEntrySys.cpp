/*============================================================================*/
/* LogEntrySys.cpp - Log Entry for System                                     */
/*============================================================================*/

#include "Logging.h"

#include "eOC.h"

#include "File.hpp" //?

/*----------------------------------------------------------------------------*/
/* set entry                                                                  */
/*----------------------------------------------------------------------------*/
void _LogEntrySys::vSet(LogEntry* pLogEntry, AttachMap* pAttachMap, eYA eSysAct)
{
  pLogEntry->m_eKind = LG_SYS;
  pLogEntry->m_cPrefix = 'Y';

  m_iSysId  = pAttachMap->m_iFocalId;
  m_eSysAct = eSysAct;
  
  if (YA_SYS_BOOT == eSysAct)
    pLogEntry->m_bLine = true;

       if (YA_SYS_BOOT  == eSysAct)  pLogEntry->m_iIndicator = OC_TRACE;
  else if (YA_SYS_TRACE  == eSysAct) pLogEntry->m_iIndicator = OC_TRACE;
}

/*----------------------------------------------------------------------------*/
/* cat sprint to given string                                                 */
/*----------------------------------------------------------------------------*/
void _LogEntrySys::vOutline(s4K& sOutline)
{
  SP_Gid spId;
  SP_String spString;

  sOutline.vCatSprint(".%s %24s"_Stz,
                
                      spId.sz(IK_SYSTEM,m_iSysId),
                      spString.szRight(aYA[m_eSysAct],24)
                      );
}

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void _LogEntrySys::vWriteCsv(File* pFile, LogEntry* pEntry)
{
  SP_Gid spId;
  
  pFile->vPrint("%s,%s"_Stz,
                
                spId.sz(IK_SYSTEM,m_iSysId),
                aYA[m_eSysAct]);
}

/*----------------------------------------------------------------------------*/
/* convert entry into text                                                    */
/*----------------------------------------------------------------------------*/
void _LogEntrySys::vToTxt(s4K& sInf, LogEntry* pEntry)
{
  sInf.vSprint(C_ENTRY_FMT_TXT,
               aCN[pEntry->m_eClass],
               aYA[m_eSysAct],
               pEntry->szIndicator(),
               aYA_short[m_eSysAct],
               pEntry->m_caNote);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
