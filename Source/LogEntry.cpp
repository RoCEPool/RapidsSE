/*============================================================================*/
/* LogEntry.cpp - Shared Log Entry                                            */
/*============================================================================*/

#include "Logging.h"
#include "AM.hpp"


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
/* init                                                                        */
/*----------------------------------------------------------------------------*/
void LogEntry::vInit(void)
{
  _precondition_(_MAGIC_,CN_LOGENTRY,_FOOTER_);
  
  m_iTick = Tick::iRdClock();
  
  StrLib::vCopyLen(m_caNode,AM::szNodeName()->aChar(),sizeof(m_caNode) - 1);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void LogEntry::vWrite(File* pFile, bool bShortForm,
                      i8 iTickDeltaUsec, i8& iTotalDelta)
{
  _precondition_(_MAGIC_,CN_LOGENTRY,_FOOTER_);
  
  if (bShortForm) {
    vWriteShort(pFile);
    return;
  }
  
  iTotalDelta += iTickDeltaUsec;
  
  if (StrLib::bSame(m_xText.m_caText,"delta reset"))
    iTotalDelta = 0;
  
  switch (m_eKind) {
      
    case EK_JOB:
      m_xJob.vWrite(pFile,this,iTickDeltaUsec,iTotalDelta);
      break;
      
    case EK_RES:
      m_xResource.vWrite(pFile,this,iTickDeltaUsec,iTotalDelta);
      break;

    case EK_TXT:
      m_xText.vWrite(pFile,this,iTickDeltaUsec,iTotalDelta);
      break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void LogEntry::vWriteShort(File* pFile)
{
  _precondition_(_MAGIC_,CN_LOGENTRY,_FOOTER_);
  
  if (m_xText.m_iForeign) {
    vWriteShortForeign(pFile);
    return;
  }
  
  // inf
  if (C_INF_GROUP == m_xText.m_iEnumGroup) {
    pFile->vPrint("[%3lld %s inf] %s\n"_Stz,
                  m_i_Entry,
                  m_caNode,
                  m_xText.m_caText);
    return;
  }
  
  // job
  if (C_JOB_GROUP == m_xText.m_iEnumGroup) {
    pFile->vPrint("[%3lld %s %03lld] %s\n"_Stz,
                  m_i_Entry,
                  m_caNode,
                  m_xText.m_iEnum,
                  m_xText.m_caText);
    return;
  }
  
  // enum
  pFile->vPrint("[%3lld %s %s] %s\n"_Stz,
                m_i_Entry,
                m_caNode,
                EnumGroups::szEnumText(m_xText.m_iEnumGroup,m_xText.m_iEnum),
                m_xText.m_caText);
}

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void LogEntry::vWriteShortForeign(File* pFile)
{
  _precondition_(_MAGIC_,CN_LOGENTRY,_FOOTER_);
  
  char caText[C_MARGIN+1] = { '\0' };
  char caRightAdj[C_MARGIN+1]  = { '\0' };
  i8 iLen;
  
  // inf
  if (C_INF_GROUP == m_xText.m_iEnumGroup) {
  
    StrLib::vSprint(caText,"%s [inf %s %3lld]"_Stz,
                    m_xText.m_caText,
                    m_caNode,
                    m_i_Entry);
    iLen = StrLib::iLength(caText);
    StrLib::vPad(caRightAdj,C_MARGIN-iLen,' ');
    StrLib::vConcat(caRightAdj,caText);
    
    pFile->vPrint("%s\n"_Stz,caRightAdj);
    return;
  }
  
  // job
  if (C_JOB_GROUP == m_xText.m_iEnumGroup) {
    StrLib::vSprint(caText,"%s [%3lld %s %03lld]"_Stz,
                    m_xText.m_caText,
                    m_xText.m_iEnum,
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
                  m_xText.m_caText,
                  EnumGroups::szEnumText(m_xText.m_iEnumGroup,m_xText.m_iEnum),
                  m_caNode,
                  m_i_Entry);
  iLen = StrLib::iLength(caText);
  StrLib::vPad(caRightAdj,C_MARGIN-iLen,' ');
  StrLib::vConcat(caRightAdj,caText);

  pFile->vPrint("%s\n"_Stz,caRightAdj);
}

/*----------------------------------------------------------------------------*/
/* init                                                                       */
/*----------------------------------------------------------------------------*/
void LogEntry::vZero(void)
{
  _precondition_(_MAGIC_,CN_LOGENTRY,_FOOTER_);
  
  m_i_Entry    = -1;
  m_iTick      = -1;
  m_caNode[0]  = '\0';
  
  m_eKind      = EK_TXT;
  
  //  m_xText.m_iEnumGroup = -1;
  //  m_xText.m_iEnum      = -1;
  //  m_xText.m_caText[0]  = '\0';
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(LogEntry)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
