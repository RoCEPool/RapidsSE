/*============================================================================*/
/* LogEntry.cpp - Shared Log Entry                                            */
/*============================================================================*/

#include "Measurement.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
LogEntry* Logging::_pLogEntry(void* aAddr)
{
  LogEntry* pLogEntry = reinterpret_cast<LogEntry*>(aAddr);
  
  RASSERT_TYPE(pLogEntry,CN_LOGENTRY);
 
  return pLogEntry;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* LogEntry::operator new(size_t)
{
  void* pObject = nullptr;
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void LogEntry::operator delete(void* pDataAddress)
{
  LogEntry* pLogEntry = reinterpret_cast<LogEntry*>(pDataAddress);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
LogEntry::LogEntry(void)
: Object(CN_LOGENTRY)
{
  _precondition_(_MAGIC_,CN_LOGENTRY,_FOOTER_);
  
  _checkInitialised_(CN_LOGENTRY);
  
  _postcondition_(_MAGIC_,CN_LOGENTRY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
LogEntry::~LogEntry(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_LOGENTRY,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* init                                                                       */
/*----------------------------------------------------------------------------*/
void LogEntry::vInit(void)
{
  _precondition_(_MAGIC_,CN_LOGENTRY,_FOOTER_);
  
  m_i_Entry    = -1;
  m_iTick      = -1;
  m_caNode[0]  = '\0';
  m_iEnumGroup = -1;
  m_iEnum      = -1;
  m_caText[0]  = '\0';
}

/*----------------------------------------------------------------------------*/
/* set                                                                        */
/*----------------------------------------------------------------------------*/
void LogEntry::vSet(i8 iEnumGroup, i8 iEnum, i8 iForeign, char* aText)
{
  _precondition_(_MAGIC_,CN_LOGENTRY,_FOOTER_);
  
  m_iTick = Tick::iRdClock();
  
  StrLib::vCopyLen(m_caNode,
                   gpAttachMap->m_pGrid->m_pThisNode->aNodeName()->aChar(),
                   sizeof(m_caNode) - 1);
  
  m_iEnumGroup = iEnumGroup;
  m_iEnum      = iEnum;
  m_iForeign   = iForeign;

  StrLib::vCopyLen(m_caText,
                   aText,
                   sizeof(m_caText) - 1);
  
  _postcondition_(_MAGIC_,CN_LOGENTRY,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* dump to csv file                                                           */
/*----------------------------------------------------------------------------*/
void LogEntry::vDump(File* pFile, bool bShortForm,
                     i8 iTickDeltaUsec, i8& iTotalDelta)
{
  _precondition_(_MAGIC_,CN_LOGENTRY,_FOOTER_);
  
  if (bShortForm) {
    vDumpShort(pFile);
    return;
  }
  
  iTotalDelta += iTickDeltaUsec;
  
  if (StrLib::bSame(m_caText,"delta reset"))
    iTotalDelta = 0;
  
  if (m_iEnum < 0)
    pFile->vPrint("%lld,%lld,%lld,%lld,%s,%s,%s\n"_Stz,
                  m_i_Entry,
                  m_iTick,
                  iTickDeltaUsec,
                  iTotalDelta,
                  
                  m_caNode,
                  aSR_Short[-m_iEnum],
                  m_caText);
  else
  if (m_iEnum == 0)
    pFile->vPrint("%lld,%lld,%lld,%lld,%s,inf,%s\n"_Stz,
                  m_i_Entry,
                  m_iTick,
                  iTickDeltaUsec,
                  iTotalDelta,
                  
                  m_caNode,
                  m_caText);
  else
    pFile->vPrint("%lld,%lld,%lld,%lld,%s,%lld,%s\n"_Stz,
                  m_i_Entry,
                  m_iTick,
                  iTickDeltaUsec,
                  iTotalDelta,
                  
                  m_caNode,
                  m_iEnum,
                  m_caText);
}

/*----------------------------------------------------------------------------*/
/* dump to csv file                                                           */
/*----------------------------------------------------------------------------*/
void LogEntry::vDumpShort(File* pFile)
{
  _precondition_(_MAGIC_,CN_LOGENTRY,_FOOTER_);
  
  if (m_iForeign) {
    vDumpShortForeign(pFile);
    return;
  }
  
  // inf
  if (C_INF_GROUP == m_iEnumGroup) {
    pFile->vPrint("[%3lld %s inf] %s\n"_Stz,
                  m_i_Entry,
                  m_caNode,
                  m_caText);
    return;
  }
  
  // job
  if (C_JOB_GROUP == m_iEnumGroup) {
    pFile->vPrint("[%3lld %s %03lld] %s\n"_Stz,
                  m_i_Entry,
                  m_caNode,
                  m_iEnum,
                  m_caText);
    return;
  }
  
  // enum
  pFile->vPrint("[%3lld %s %s] %s\n"_Stz,
                m_i_Entry,
                m_caNode,
                EnumGroups::szEnumText(m_iEnumGroup,m_iEnum),
                m_caText);
}

/*----------------------------------------------------------------------------*/
/* dump to csv file                                                           */
/*----------------------------------------------------------------------------*/
void LogEntry::vDumpShortForeign(File* pFile)
{
  _precondition_(_MAGIC_,CN_LOGENTRY,_FOOTER_);
  
  char caText[C_MARGIN+1] = { '\0' };
  char caRightAdj[C_MARGIN+1]  = { '\0' };
  i8 iLen;
  
  // inf
  if (C_INF_GROUP == m_iEnumGroup) {
  
    StrLib::vSprint(caText,"%s [inf %s %3lld]"_Stz,
                    m_caText,
                    m_caNode,
                    m_i_Entry);
    iLen = StrLib::iLength(caText);
    StrLib::vPad(caRightAdj,C_MARGIN-iLen,' ');
    StrLib::vConcat(caRightAdj,caText);
    
    pFile->vPrint("%s\n"_Stz,caRightAdj);
    return;
  }
  
  // job
  if (C_JOB_GROUP == m_iEnumGroup) {
    StrLib::vSprint(caText,"%s [%3lld %s %03lld]"_Stz,
                    m_caText,
                    m_iEnum,
                    m_caNode,
                    m_i_Entry);
    iLen = StrLib::iLength(caText);
    StrLib::vPad(caRightAdj,C_MARGIN-iLen,' ');
    StrLib::vConcat(caRightAdj,caText);

    pFile->vPrint("%s\n"_Stz,caRightAdj);
    return;
  }
  
  // enum
  StrLib::vSprint(caText,"%s [%s %s %3lld]"_Stz,
                  m_caText,
                  EnumGroups::szEnumText(m_iEnumGroup,m_iEnum),
                  m_caNode,
                  m_i_Entry);
  iLen = StrLib::iLength(caText);
  StrLib::vPad(caRightAdj,C_MARGIN-iLen,' ');
  StrLib::vConcat(caRightAdj,caText);

  pFile->vPrint("%s\n"_Stz,caRightAdj);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(LogEntry)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
