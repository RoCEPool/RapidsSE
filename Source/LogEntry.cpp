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
/* init                                                                        */
/*----------------------------------------------------------------------------*/
void LogEntry::vInit(void)
{
  _precondition_(_MAGIC_,CN_LOGENTRY,_FOOTER_);
  
  m_iTick = Tick::iRdClock();
  m_iNodeNo = AM::iThisNodeNo();
  StrLib::vCopyLen(m_caNode,AM::szNodeName()->aChar(),sizeof(m_caNode) - 1);
  
  m_caText[0] = '\0';
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
  
  //if (StrLib::bSame(m_xInf.m_caText,"delta reset"))
  //  iTotalDeltaUsec = 0;
  
  switch (m_eKind) {
      
    case EK_INF:
      m_xInf.vWriteCsv(pFile,this,iEntryDeltaUsec,iTotalDeltaUsec);
      break;
      
    case EK_ITF:
      m_xItf.vWriteCsv(pFile,this,iEntryDeltaUsec,iTotalDeltaUsec);
      break;
      
    case EK_JOB:
      m_xJob.vWriteCsv(pFile,this,iEntryDeltaUsec,iTotalDeltaUsec);
      break;
      
    case EK_OP:
      m_xOp.vWriteCsv(pFile,this,iEntryDeltaUsec,iTotalDeltaUsec);
      break;
      
    case EK_PRM:
      m_xPrm.vWriteCsv(pFile,this,iEntryDeltaUsec,iTotalDeltaUsec);
      break;
      
    case EK_RES:
      vWriteCsv(pFile,iEntryDeltaUsec,iTotalDeltaUsec);
      m_xRes.vWriteCsv(pFile);
      pFile->vPrint(",%s\n"_Stz,m_caText);
      break;

    case EK_SVC:
      vWriteCsv(pFile,iEntryDeltaUsec,iTotalDeltaUsec);
      m_xSvc.vWriteCsv(pFile);
      pFile->vPrint(",%s\n"_Stz,m_caText);
      break;
      
    case EK_TSK:
      vWriteCsv(pFile,iEntryDeltaUsec,iTotalDeltaUsec);
      m_xTsk.vWriteCsv(pFile);
      pFile->vPrint(",%s\n"_Stz,m_caText);
      break;
      
    case EK_THD:
      vWriteCsv(pFile,iEntryDeltaUsec,iTotalDeltaUsec);
      m_xThd.vWriteCsv(pFile);
      pFile->vPrint(",%s\n"_Stz,m_caText);
      break;
      
    case EK_TXN:
      vWriteCsv(pFile,iEntryDeltaUsec,iTotalDeltaUsec);
      m_xTxn.vWriteCsv(pFile);
      pFile->vPrint(",%s\n"_Stz,m_caText);
      break;
      
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* write to csv file                                                          */
/*----------------------------------------------------------------------------*/
void LogEntry::vWriteCsv(File* pFile, i8 iTickDeltaUsec, i8& iTotalDelta)
{
  pFile->vPrint("%lld,%lld,%lld,%lld,"
                "%lld,%s,%s"_Stz,
                
                m_i_Entry,
                m_iTick,
                iTickDeltaUsec,
                iTotalDelta,
                
                m_iNodeNo,
                m_caNode,
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
  
  m_eKind      = EK_INF;
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(LogEntry)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
