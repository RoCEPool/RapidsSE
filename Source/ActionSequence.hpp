/*============================================================================*/
/* ActionSequence.hpp - Action Sequence                                       */
/*============================================================================*/
#pragma once

namespace Jobs { class Job; }

namespace Maps
{
  static const i8    C_ACTION_SEQUENCE = C_1M * 1;
  static const char* C_ACTION_REGION   = "/SE_ActionSequence";

  class ActionSequence : public Map
  {
  friend class Action;
  
  private:
    Lock m_okLock;
    i8   m_i_Action;  // current subscript
    i8   m_nActions;  // number of available actions in array

    Action m_oaAction[C_ACTION_SEQUENCE];

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t, ActionSequence* pActionSequence);
    void  operator delete(void*);

    ActionSequence(void);
    ~ActionSequence(void) noexcept(false);

    static void vOpenByApi(void);
    static void vOpenBySE(bool bStarting);
  
    static void vAdd(i8 i_RootAct, i8 iJobId, eCN eClassNo,
                     eAN eAction=AN_NONE, i8 iArg0=0, i8 iArg1=0);
  
    static i8 iBegin(i8 iJobId,  eCN eClassNo,
                     eAN eAction=AN_NONE, i8 iArg0=0, i8 iArg1=0);
  
    static void vAdd(Jobs::Job* pJob,
                     eAN eAction=AN_NONE, i8 iArg0=0, i8 iArg1=0);
    static void vAdd(Jobs::Job* pJob,eAN eAction,
                     Jobs::Job* pSystemJob,i8 iArg0=0, i8 iArg1=0);
    static void vAdd(Jobs::Job* pJob,eAN eAction, Stz* szText);
    static void vRoot(Jobs::Job* pJob);
    static void vEnd(Jobs::Job* pJob);

    static void vWriteCsv(void);

  private:
    Action* pAllotAction(void);
    void vDropLock(void);
    void vFindRange_NL(i8& i_Min, i8& i_Max);
    void vGrabLock(void);
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
