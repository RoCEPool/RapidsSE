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
  
  StrLib::vCopyLen(m_caNode,AM::szNodeName()->aChar(),sizeof(m_caNode) - 1);
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
      m_xInf.vWrite(pFile,this,iEntryDeltaUsec,iTotalDeltaUsec);
      break;
      
    case EK_ITF:
      m_xItf.vWrite(pFile,this,iEntryDeltaUsec,iTotalDeltaUsec);
      break;
      
    case EK_JOB:
      m_xJob.vWrite(pFile,this,iEntryDeltaUsec,iTotalDeltaUsec);
      break;
      
    case EK_OP:
      m_xOp.vWrite(pFile,this,iEntryDeltaUsec,iTotalDeltaUsec);
      break;
      
    case EK_PRM:
      m_xPrm.vWrite(pFile,this,iEntryDeltaUsec,iTotalDeltaUsec);
      break;
      
    case EK_RES:
      m_xRes.vWrite(pFile,this,iEntryDeltaUsec,iTotalDeltaUsec);
      break;

    case EK_SVC:
      m_xSvc.vWrite(pFile,this,iEntryDeltaUsec,iTotalDeltaUsec);
      break;
      
    case EK_TSK:
      m_xTsk.vWrite(pFile,this,iEntryDeltaUsec,iTotalDeltaUsec);
      break;
      
    case EK_THD:
      m_xThd.vWrite(pFile,this,iEntryDeltaUsec,iTotalDeltaUsec);
      break;
      
    case EK_TXN:
      m_xTxn.vWrite(pFile,this,iEntryDeltaUsec,iTotalDeltaUsec);
      break;
      
    default:
      NEVER_REACH;
  }
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
