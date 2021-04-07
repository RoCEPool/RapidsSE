/*============================================================================*/
/* Loggable.cpp - Loggable Object                                             */
/*============================================================================*/

#include "BASE.h"

#include "CmdLineArgs.hpp"
#include "Row.hpp"
#include "WriteBehind.hpp"
#include "InternalTables.hpp"

extern CmdLineArgs gxCmdLineArgs;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Loggable::Loggable(eCN eClassNo, i8 iLogCtl)
: Object(eClassNo)
{
   m_iObjectId = AM::iNextObjectNo(eClassNo);
  
//  if (CN_JOB == eClassNo)
//    printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ m_iObjectId %lld\n",m_iObjectId);

  m_iEventNo   = C_UNKNOWN;
  m_iEventTick = AM::iRdTickingDevice();
  
  m_eEvent     = EV_NONE;
  m_iDuration  = C_UNKNOWN;
  m_iLogCtl    = iLogCtl;

  m__spare0    = C_UNKNOWN;
  m__spare1    = C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Loggable::~Loggable(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* log next event                                                             */
/*----------------------------------------------------------------------------*/
void Loggable::vLog_TBR(eEV eEvent, Stz* szFormat, ...)
{
  _precondition_(_MAGIC_);
  UNUSED(eEvent);
    
  if (! gxCmdLineArgs.m_xSysArgs.m_bEvents)
    return;
  
  if (C_DONT_LOG == m_iLogCtl)
    return;
 
  if (! AM::bHasStarted())
    return;
  
//  if (   CN_DIR == eGetClassNo()
//      || CN_FIFO == eGetClassNo()
//      || CN_FILE == eGetClassNo()
//      || CN_JOB == eGetClassNo()
//      || CN_JOB__FIND == eGetClassNo()
//      || CN_LOCK == eGetClassNo()
//      || CN_MUTEX == eGetClassNo()
//      || CN_OP == eGetClassNo()
//      || CN_OPERANDQUEUE == eGetClassNo()
//      || CN_PSXTHD == eGetClassNo()
//      || CN_SEPROMPT == eGetClassNo()
//      || CN_SESSION == eGetClassNo()
//      || CN_SOCKET == eGetClassNo()
//      || CN_TASK == eGetClassNo()
//      )
//    return;


  s4K sNote;
  
  va_list xVarArgs;
  
  va_start(xVarArgs,szFormat);
  
  sNote.vSprintVA(szFormat,xVarArgs);
  
  va_end(xVarArgs);
  
  vAddRow(eEvent,sNote);
}


/*----------------------------------------------------------------------------*/
/* log entry into internal table                                              */
/*----------------------------------------------------------------------------*/
void Loggable::vLogTI(i8 iTable, ...)
{
  _precondition_(_MAGIC_);
  
  WriteBehind* pWriteBehind = AM::THREAD::pWriteBehind();
  if (! pWriteBehind)
    return;

  StreamDef* pStreamDef = AM::pInternalTables()->pStreamDef((eTI)iTable);
  if (! pStreamDef)
    return;
  
//  pWriteBehind->vGrabLock();
//
//  va_list xVarArgs;
//  va_start(xVarArgs,iTable);
//
//  Row* pRow = pWriteBehind->pAllotRow((eTI)iTable);
//  pRow->vDeposit(xVarArgs);
//
//  va_end(xVarArgs);
//
//  pWriteBehind->vDropLock();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get last event                                                             */
/*----------------------------------------------------------------------------*/
eEV Loggable::eEvent(void)
{
  _precondition_(_MAGIC_);
  
  return m_eEvent;
}

/*----------------------------------------------------------------------------*/
/* set last event                                                             */
/*----------------------------------------------------------------------------*/
void Loggable::vEvent(eEV eEvent)
{
  _precondition_(_MAGIC_);
  
  m_eEvent = eEvent;
}

/*----------------------------------------------------------------------------*/
/* get event no                                                               */
/*----------------------------------------------------------------------------*/
i8 Loggable::iEventNo(void)
{
  _precondition_(_MAGIC_);
  
  return m_iEventNo;
}

/*----------------------------------------------------------------------------*/
/* inc event no                                                               */
/*----------------------------------------------------------------------------*/
void Loggable::vEventNoInc(void)
{
  _precondition_(_MAGIC_);
  
  m_iEventNo++;
}

/*----------------------------------------------------------------------------*/
/* get event no                                                               */
/*----------------------------------------------------------------------------*/
i8 Loggable::iEventTick(void)
{
  _precondition_(_MAGIC_);
  
  return m_iEventTick;
}

/*----------------------------------------------------------------------------*/
/* set event tick                                                             */
/*----------------------------------------------------------------------------*/
void Loggable::vEventTickSet(void)
{
  _precondition_(_MAGIC_);
  
  m_iEventTick = AM::iRdTickingDevice();
}

/*----------------------------------------------------------------------------*/
/* get object id                                                              */
/*----------------------------------------------------------------------------*/
i8 Loggable::iObjectId(void)
{
  _precondition_(_MAGIC_);
  
  return m_iObjectId;
}

/*----------------------------------------------------------------------------*/
/* set object id                                                              */
/*----------------------------------------------------------------------------*/
void Loggable::vObjectId(i8 iObjectId)
{
  _precondition_(_MAGIC_);
  
  m_iObjectId = iObjectId;
}

/*----------------------------------------------------------------------------*/
/* get object name                                                            */
/*----------------------------------------------------------------------------*/
Stz* Loggable::sObjectNm(void)
{
  _precondition_(_MAGIC_);
  
  return m_sObjectNm;
}

/*----------------------------------------------------------------------------*/
/* set object name                                                            */
/*----------------------------------------------------------------------------*/
void Loggable::vObjectNm(Stz* szObjectNm)
{
  _precondition_(_MAGIC_);
  
  m_sObjectNm = szObjectNm;
}

/*----------------------------------------------------------------------------*/
/* set next event                                                             */
/*----------------------------------------------------------------------------*/
void Loggable::vNextEvent(eEV eEvent)
{
  _precondition_(_MAGIC_);
  
  vEventTickSet();
  vEvent(eEvent);
  vEventNoInc();
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* log next event                                                             */
/*----------------------------------------------------------------------------*/
void Loggable::vAddRow(eEV eEvent, Stz* szNote)
{
  _precondition_(_MAGIC_);
  UNUSED(eEvent);
  UNUSED(szNote);
  
//  printf("Loggable::vAddRow : %s %s\n",
//         aEV[eEvent]->azChar(),
//         szNote->azChar());

  WriteBehind* pWriteBehind = AM::THREAD::pWriteBehind();
  
  if (! pWriteBehind)
    return;
  
  pWriteBehind->vDepositEvent(C_VARGS_BEGIN,
                              eEvent,
                              szNote,
                              C_VARGS_END);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
