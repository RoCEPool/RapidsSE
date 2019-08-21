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
void LogEntry::vInit(eCN eClassNo, Stz* szText)
{
  _precondition_(_MAGIC_,CN_LOGENTRY,_FOOTER_);
  
  m_i_Prev = C_UNKNOWN;
  
  m_iTick = Tick::iRdClock();
  m_iNodeNo = AM::iThisNodeNo();
  StrLib::vCopyLen(m_caNode,AM::szNodeName()->aChar(),sizeof(m_caNode) - 1);
  
  m_caText[0] = '\0';
  StrLib::vCopyLen(m_caText,szText->aChar(),C_LOG_TEXT - 1);

  m_eClass = eClassNo;
  m_pObject = nullptr;
  
  m_bLine = false;
}

/*----------------------------------------------------------------------------*/
/* init                                                                       */
/*----------------------------------------------------------------------------*/
void LogEntry::vInit(Focal *pFocal, Stz* szText)
{
  _precondition_(_MAGIC_,CN_LOGENTRY,_FOOTER_);
  
  m_i_Prev = pFocal->m_i_LogPrev;
  pFocal->m_i_LogPrev = m_i_Entry;
  
  m_iTick = Tick::iRdClock();
  m_iNodeNo = AM::iThisNodeNo();
  StrLib::vCopyLen(m_caNode,AM::szNodeName()->aChar(),sizeof(m_caNode) - 1);
  
  m_caText[0] = '\0';
  StrLib::vCopyLen(m_caText,szText->aChar(),C_LOG_TEXT - 1);
  
  m_eClass  = pFocal->eClass();
  m_pObject = pFocal;
  
  m_bLine = false;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void LogEntry::vWrite(File* pFile, i8 iEntryDeltaUsec, i8& iTotalDeltaUsec)
{
  _precondition_(_MAGIC_,CN_LOGENTRY,_FOOTER_);
  
  iTotalDeltaUsec += iEntryDeltaUsec;
  
  //if (StrLib::bSame(m_xFyi.m_caText,"delta reset"))
  //  iTotalDeltaUsec = 0;
  
  vWriteCsv(pFile,iEntryDeltaUsec,iTotalDeltaUsec);
  
  switch (m_eKind) {
    case EK_FYI: m_xFyi.vWriteCsv(pFile); break;
    case EK_JOB: m_xJob.vWriteCsv(pFile); break;
    case EK_LCK: m_xLck.vWriteCsv(pFile); break;
    case EK_LOG: m_xLog.vWriteCsv(pFile); break;
    case EK_OBJ: m_xObj.vWriteCsv(pFile); break;
    case EK_OPR: m_xOpr.vWriteCsv(pFile); break;
    case EK_PRM: m_xPrm.vWriteCsv(pFile); break;
    case EK_QUE: m_xQue.vWriteCsv(pFile); break;
    case EK_RES: m_xRes.vWriteCsv(pFile); break;
    case EK_SSN: m_xSsn.vWriteCsv(pFile); break;
    case EK_SVC: m_xSvc.vWriteCsv(pFile); break;
    case EK_SYS: m_xSys.vWriteCsv(pFile); break;
    case EK_TSK: m_xTsk.vWriteCsv(pFile); break;
    case EK_THD: m_xThd.vWriteCsv(pFile); break;
    case EK_TXN: m_xTxn.vWriteCsv(pFile); break;
    default:
      NEVER_REACH;
  }
  
  pFile->vPrint("}, %s\n"_Stz,m_caText);
}

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void LogEntry::vWriteCsv(File* pFile, i8 iTickDeltaUsec, i8& iTotalDelta)
{
  if (m_bLine)
    pFile->vPrint("%s\n"_Stz,C_LOG_LINE);

  pFile->vPrint("[%lld,%lld], "
                "(%lld,%lld,%lld), "
                "<%s,%lld,%s,%p>, "_Stz
                "{%s"_Stz,
                
                m_i_Entry,
                m_i_Prev,

                m_iTick,
                iTotalDelta,
                iTickDeltaUsec,
                
                m_caNode,
                m_iNodeNo,
                aCN[m_eClass],
                m_pObject,
                
                aEK[m_eKind]
                );
}

/*----------------------------------------------------------------------------*/
/* zero                                                                       */
/*----------------------------------------------------------------------------*/
void LogEntry::vZero(void)
{
  _precondition_(_MAGIC_,CN_LOGENTRY,_FOOTER_);
  
  m_i_Entry    = -1;
  m_iTick      = -1;
  m_caNode[0]  = '\0';
  
  m_eKind      = EK_FYI;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(LogEntry)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
