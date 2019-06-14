/*============================================================================*/
/* Action.cpp - when did an action occur and what happened                    */
/*============================================================================*/

#include "Maps.h"
#include "StorageEngine.h"

extern ActionSequence* gpActionSequence;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Action* Maps::_pAction(void* aAddr)
{
  Action* pAction = reinterpret_cast<Action*>(aAddr);
  
  RASSERT_TYPE(pAction,CN_ACTION);
  
  return pAction;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Action::Action(void)
: Object(CN_ACTION)
{
  _precondition_(_MAGIC_,CN_ACTION,_FOOTER_);
  
  m_i_Action = 0;
  m_i_Root   = 0;
  m_iTick    = 0;
  
  m_aJob       = nullptr;
  m_iJobId     = 0;
  m_eClassNo   = CN_NONE;
  m_iParentId  = 0;
  m_bWasRecv   = false;
  m_i_Activity = C_UNKNOWN;

  m_aJob_Arg       = nullptr;
  m_iJobId_Arg     = 0;
  m_eClassNo_Arg   = CN_NONE;
  m_iParentId_Arg  = 0;
  m_bWasRecv_Arg   = false;
  m_i_Activity_Arg = C_UNKNOWN;
  
  m_eAction  = AN_NONE;
  m_iArg0    = 0;
  m_iArg1    = 0;
  
  m_caName[0] = '\0';
  m_caText[0] = '\0';
  
  ASSERT(0 == *m_wLink);

 _checkInitialised_(CN_ACTION);
  
  _postcondition_(_MAGIC_,CN_ACTION,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Action::~Action(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_ACTION,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* init                                                                       */
/*----------------------------------------------------------------------------*/
i8 Action::iDeltaNextUsec(void)
{
  _precondition_(_MAGIC_,CN_ACTION);
  
  Action* pNext = this->pNext();
  
  if (! pNext)
    return 0;
  
  i8 iTickDelta = m_iTick - pNext->m_iTick;
  
  i8 iDeltaUsec = Tsc::iToUsec(iTickDelta);
  
  return iDeltaUsec;
}

/*----------------------------------------------------------------------------*/
/* init                                                                       */
/*----------------------------------------------------------------------------*/
void Action::vInit(i8 i_Action)
{
  _precondition_(_MAGIC_,CN_ACTION);
  
  m_i_Action = 0;
  m_i_Root   = 0;
  m_iTick    = 0;
  
  m_aJob       = nullptr;
  m_iJobId     = 0;
  m_eClassNo   = CN_NONE;
  m_iParentId  = 0;
  m_bWasRecv   = false;
  m_i_Activity = C_UNKNOWN;
  
  m_aJob_Arg       = nullptr;
  m_iJobId_Arg     = 0;
  m_eClassNo_Arg   = CN_NONE;
  m_iParentId_Arg  = 0;
  m_bWasRecv_Arg   = false;
  m_i_Activity_Arg = C_UNKNOWN;
  
  m_eAction  = AN_NONE;
  m_iArg0    = 0;
  m_iArg1    = 0;
  
  m_caName[0] = '\0';
  m_caText[0] = '\0';
  
  m_wLink.vReset();
}

/*----------------------------------------------------------------------------*/
/* is this action a root action?                                              */
/*----------------------------------------------------------------------------*/
bool Action::bIsRoot(void)
{
  _precondition_(_MAGIC_,CN_ACTION);
  
  if (m_i_Action == m_i_Root && m_iTick)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* reset                                                                      */
/*----------------------------------------------------------------------------*/
Action* Action::pNext(void)
{
  _precondition_(_MAGIC_,CN_ACTION);
  
  i8 i_NextAction = * m_wLink;
  
  if (C_UNKNOWN == i_NextAction)
    return nullptr;
  
  Action* pNext = &gpActionSequence->m_oaAction[i_NextAction];
  
  return pNext;
}

/*----------------------------------------------------------------------------*/
/* reset                                                                      */
/*----------------------------------------------------------------------------*/
void Action::vReset(void)
{
  _precondition_(_MAGIC_,CN_ACTION);
  
  m_i_Action = 0;
  m_i_Root   = 0;
  m_iTick    = 0;
  
  m_aJob       = nullptr;
  m_iJobId     = 0;
  m_eClassNo   = CN_NONE;
  m_iParentId  = 0;
  m_bWasRecv   = false;
  m_i_Activity = C_UNKNOWN;
  
  m_aJob_Arg       = nullptr;
  m_iJobId_Arg     = 0;
  m_eClassNo_Arg   = CN_NONE;
  m_iParentId_Arg  = 0;
  m_bWasRecv_Arg   = false;
  m_i_Activity_Arg = C_UNKNOWN;
  
  m_eAction  = AN_NONE;
  m_iArg0    = 0;
  m_iArg1    = 0;
  
  m_caName[0] = '\0';
  m_caText[0] = '\0';
  
  m_wLink.vReset();
}

/*----------------------------------------------------------------------------*/
/* write action to file                                                       */
/*----------------------------------------------------------------------------*/
void Action::vWrite(File* pFile)
{
  _precondition_(_MAGIC_,CN_ACTION);
  
  switch (m_eAction) {
  
  case AN_ADD_EXECUTING_SET:
  case AN_ADD_READY_QUEUE:
  case AN_ADD_READY_QUEUE_LP:
  case AN_ADD_SEND_QUEUE:
  case AN_ADD_SLEEPING_QUEUE:
  case AN_ADD_SLOT_QUEUE:
  case AN_ADD_WAITING_SET:
  case AN_ADDED_ORIGINATOR:
  case AN_ADDED_PARTICIPANT:
  case AN_DDL_DROPPED_READ:
  case AN_DDL_DROPPED_WRITE:
  case AN_DDL_GRAB_READ:
  case AN_DDL_GRAB_WRITE:
  case AN_DDL_GRABBED_READ:
  case AN_DDL_GRABBED_WRITE:
  case AN_DESTRUCTOR:
  case AN_REMOVED_ORIGINATOR:
  case AN_REMOVED_PARTICIPANT:
  case AN_TASK_ACKED:
  case AN_TASK_LOCKED:
  case AN_TASK_WAIT:
    vWrite_P(pFile);
    break;
      
  case AN_CONSTRUCTOR:
    vWrite_Penum(pFile);
    break;
      
  case AN_JOB_PHASE:
    case AN_PRE_PROMPT:
    vWrite_Penum(pFile);
    break;
    
  case AN_ACKS_REQUIRED:
  case AN_JOB_BEGIN:
  case AN_JOB_CHUNK:
  case AN_JOB_END:
  case AN_POST_PROMPT:
  case AN_THREAD_EXECUTE:
    vWrite_Pi8(pFile);
    break;
      
  case AN_ACK_TASK:
  case AN_AM_DECIDER:
  case AN_NEW_JOB:
    vWrite_Pjob(pFile);
    break;
      
  case AN_END:
  case AN_ROOT:
    vWrite_Proot(pFile);
    break;
      
  case AN_FIND_ORIGINATOR:
  case AN_FIND_PARTICIPANT:
  case AN_PRE_SEND:
  case AN_POST_RECV:
    vWrite_Psysjob(pFile);
    break;
    
  case AN_SPAWN_JOB:
    vWrite_Psysjob_enum(pFile);
    break;
    
  case AN_RECV_JOB:
  case AN_SEND_JOB:
    vWrite_Psysjob_i8(pFile);
    break;
      
  case AN_CLOSE_FILE:
  case AN_OPEN_FILE:
  case AN_TABLE_USE:
    vWrite_Ptxt(pFile);
    break;

  case AN_RESOURCES_CHECKED:
  case AN_RESOURCES_RELEASED:
  case AN_SEED_CAN:
  case AN_SEED_EXE:
  case AN_SEED_END:
  case AN_SEED_RCV:
  case AN_SEED_REQ:
  case AN_SEED_RUN:
    vWrite_Pwrd(pFile);
    break;

  default:
    vWrite_Default(pFile);
  }
}

////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* write action to file                                                       */
/*----------------------------------------------------------------------------*/
void Action::vWriteJob(File* pFile)
{
  _precondition_(_MAGIC_,CN_ACTION);
  
  pFile->vPrint("%16s[%1s.%d.%03lld,%03lld]"_Stz,
                CN_NONE == m_eClassNo ? "PROMPT"_Stz : aCN[m_eClassNo],
                m_bWasRecv ? "r" : "n",
                Job::iNodeNo(m_iJobId),
                Job::iJobNo(m_iJobId),
                m_iParentId
                );
}

/*----------------------------------------------------------------------------*/
/* write action to file                                                       */
/*----------------------------------------------------------------------------*/
void Action::vWriteLead(File* pFile)
{
  _precondition_(_MAGIC_,CN_ACTION);
  
  i8 iDeltaUsec = iDeltaNextUsec();
  
  pFile->vPrint("[%03lld, %03lld, %lld (+%06lld)] "_Stz,
                
                m_i_Action,
                m_i_Root,
                m_iTick,
                iDeltaUsec
                );
}

/*----------------------------------------------------------------------------*/
/* write action to file                                                       */
/*----------------------------------------------------------------------------*/
void Action::vWriteRefer(File* pFile)
{
  _precondition_(_MAGIC_,CN_ACTION);
  
  //pFile->vPrint("<<%lld, %03lld>>"_Stz,m_i_Activity,* m_wLink);
  pFile->vPrint("<<%03lld>>"_Stz,m_i_Activity);
}

/*----------------------------------------------------------------------------*/
/* write action to file                                                       */
/*----------------------------------------------------------------------------*/
void Action::vWriteSysJob(File* pFile)
{
  _precondition_(_MAGIC_,CN_ACTION);
  
  pFile->vPrint("%16s[%1s.%d.%03lld,%03lld]"_Stz,
                CN_NONE == m_eClassNo_Arg ? "PROMPT"_Stz : aCN[m_eClassNo_Arg],
                m_bWasRecv_Arg ? "r" : "n",
                Job::iNodeNo(m_iJobId_Arg),
                Job::iJobNo(m_iJobId_Arg),
                m_iParentId_Arg
                );
}

////////////////////////////////////////////////////////////////////////////////

/*----------------------------------------------------------------------------*/
/* write action to file                                                       */
/*----------------------------------------------------------------------------*/
void Action::vWrite_Default(File* pFile)
{
  _precondition_(_MAGIC_,CN_ACTION);
  
  pFile->vPrint(/* 0 */ "[%03lld, %03lld, %lld] "
                /* 1 */ "%16s"
                /* 2 */ "[%14p, %03lld, %03lld, %1s] "
                /* 3 */ "%20s"
                /* 4 */ "(%2lld, %14p) "
                /* 5 */ "<<%lld, %03lld>>\n"_Stz,
                
                /* 0 */ m_i_Action,
                m_i_Root,
                m_iTick,
                
                /* 1 */ aCN[m_eClassNo],
                
                /* 2 */ m_aJob,
                        m_iJobId,
                        m_iParentId,
                        m_bWasRecv ? "R" : "n",
                
                /* 3 */ aAN[m_eAction],
                
                /* 4 */ m_iArg0,
                m_iArg1,
                
                /* 5 */ m_i_Activity,
                * m_wLink
                );
}

/*----------------------------------------------------------------------------*/
/* write action to file                                                       */
/*----------------------------------------------------------------------------*/
void Action::vWrite_P(File* pFile)
{
  _precondition_(_MAGIC_,CN_ACTION);

  vWriteLead(pFile);
  vWriteRefer(pFile);
  vWriteJob(pFile);

  pFile->vPrint(".%s() "_Stz,
                aAN[m_eAction]);
  
  pFile->vPrint("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* write action to file                                                       */
/*----------------------------------------------------------------------------*/
void Action::vWrite_Penum(File* pFile)
{
  _precondition_(_MAGIC_,CN_ACTION);
  
  vWriteLead(pFile);
  vWriteRefer(pFile);
  vWriteJob(pFile);
  
  pFile->vPrint(".%s(%s)"_Stz,
                aAN[m_eAction],
                EnumGroups::szEnumText(m_iArg1,m_iArg0));
  
  pFile->vPrint("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* write action to file                                                       */
/*----------------------------------------------------------------------------*/
void Action::vWrite_Pi8(File* pFile)
{
  _precondition_(_MAGIC_,CN_ACTION);
  
  vWriteLead(pFile);
  vWriteRefer(pFile);
  vWriteJob(pFile);

  pFile->vPrint(".%s(%lld) "_Stz,
                aAN[m_eAction],
                m_iArg0);
  
  pFile->vPrint("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* write action to file                                                       */
/*----------------------------------------------------------------------------*/
void Action::vWrite_Pjob(File* pFile)
{
  _precondition_(_MAGIC_,CN_ACTION);
  
  vWriteLead(pFile);
  vWriteRefer(pFile);
  vWriteJob(pFile);

  pFile->vPrint(".%s(%s[%03lld]) "_Stz,
                aAN[m_eAction],
                aCN[m_eClassNo_Arg],m_iJobId_Arg);
  
  pFile->vPrint("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* write action to file                                                       */
/*----------------------------------------------------------------------------*/
void Action::vWrite_Proot(File* pFile)
{
  _precondition_(_MAGIC_,CN_ACTION);
  
  vWriteLead(pFile);
  vWriteRefer(pFile);
  vWriteJob(pFile);
  
  
  Tsp* aTsp = (Tsp*)&m_iArg0;
  
  spTSP spTsp;
  pFile->vPrint(".%s(%s) "_Stz,
                aAN[m_eAction],
                spTsp.sz(aTsp->m_iValue));
  
//  Tsp wTsp;
//  wTsp.vSet((Tsp*)&m_iArg0);
//  
//  s32 sTsp;
//  wTsp.vSprint(sTsp);
//  pFile->vPrint(".%s(%s) "_Stz,
//                aAN[m_eAction],
//                sTsp.aStz());
  
  pFile->vPrint("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* write action to file                                                       */
/*----------------------------------------------------------------------------*/
void Action::vWrite_Psysjob(File* pFile)
{
  _precondition_(_MAGIC_,CN_ACTION);
  
  vWriteLead(pFile);
  vWriteRefer(pFile);
  vWriteSysJob(pFile);
  
  pFile->vPrint(".%s(%s[%03lld]) "_Stz,
                aAN[m_eAction],
                aCN[m_eClassNo],m_iJobId);
  
  pFile->vPrint("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* write action to file                                                       */
/*----------------------------------------------------------------------------*/
void Action::vWrite_Psysjob_enum(File* pFile)
{
  _precondition_(_MAGIC_,CN_ACTION);
  
  vWriteLead(pFile);
  vWriteRefer(pFile);
  vWriteSysJob(pFile);
  
  pFile->vPrint(".%s(%s[%03lld],%s) "_Stz,
                aAN[m_eAction],
                aCN[m_eClassNo],m_iJobId,
                EnumGroups::szEnumText(m_iArg1,m_iArg0));
  
  pFile->vPrint("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* write action to file                                                       */
/*----------------------------------------------------------------------------*/
void Action::vWrite_Psysjob_i8(File* pFile)
{
  _precondition_(_MAGIC_,CN_ACTION);
  
  vWriteLead(pFile);
  vWriteRefer(pFile);
  vWriteSysJob(pFile);
  
  pFile->vPrint(".%s(%s[%03lld],%lld) "_Stz,
                aAN[m_eAction],
                aCN[m_eClassNo],m_iJobId,
                m_iArg0);
  
  pFile->vPrint("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* write action to file                                                       */
/*----------------------------------------------------------------------------*/
void Action::vWrite_Ptxt(File* pFile)
{
  _precondition_(_MAGIC_,CN_ACTION);
  
  vWriteLead(pFile);
  vWriteRefer(pFile);
  vWriteJob(pFile);
  
  pFile->vPrint(".%s(\"%s\") "_Stz,
                aAN[m_eAction],
                m_caText);
  
  pFile->vPrint("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* write action to file                                                       */
/*----------------------------------------------------------------------------*/
void Action::vWrite_Pwrd(File* pFile)
{
  _precondition_(_MAGIC_,CN_ACTION);
  
  vWriteLead(pFile);
  vWriteRefer(pFile);
  vWriteJob(pFile);
  
  pFile->vPrint(".%s(: %s) "_Stz,
                aAN[m_eAction],
                m_caText);
  
  pFile->vPrint("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Action::aFooter(void)
{
  _precondition_(_MAGIC_,CN_ACTION);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
