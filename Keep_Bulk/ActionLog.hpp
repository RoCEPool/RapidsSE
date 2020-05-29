/*============================================================================*/
/* ActionLog.hpp - Action Log                                                 */
/*============================================================================*/
#pragma once

namespace Logging
{
  static const char* C_ACTION_REGION   = "/SE_ActionLog";

  class ActionLog : public ShmLog
  {
  friend class Action;
  
  private:

    Action m_oaAction[C_LOG_ENTRIES];

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t, ActionLog* pActionSequence);
    void  operator delete(void*);

    ActionLog(void);
    ~ActionLog(void) noexcept(false);

    static void vOpenByApi(void);
    static void vOpenBySE(bool bStarting);
  
    static void vAdd(i8 i_RootAct, i8 iJobId, eCN eClassNo,
                     eAN eAction=AN_NONE, i8 iArg0=0, i8 iArg1=0);
    static void vAdd(Jobs::Job* pJob,
                     eAN eAction=AN_NONE, i8 iArg0=0, i8 iArg1=0);
    static void vAdd(Jobs::Job* pJob,eAN eAction,
                     Jobs::Job* pSystemJob,i8 iArg0=0, i8 iArg1=0);
    static void vAdd(Jobs::Job* pJob,eAN eAction, Stz* szText);
    static i8   iBegin(i8 iJobId,  eCN eClassNo,
                       eAN eAction=AN_NONE, i8 iArg0=0, i8 iArg1=0);
    static void vEnd(Jobs::Job* pJob);

    static void vRoot(Jobs::Job* pJob);

    static void vWriteCsv(void);

  private:
    Action* pAllotAction(void);
    void vFindRange_NL(i8& i_Min, i8& i_Max);
    void vLinkAction(Action* pRoot, Action* pAction);
    Action* pNewLinkedAction(Action *pRootAction);
    Action* pNewRootAction(void);
    Action* pNextAction(Action *pAction);
    i8   iPercentFull_NL(void);
    void vWriteToFile(void);

    Footer* aFooter(void);
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
