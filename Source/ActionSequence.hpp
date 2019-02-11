/*============================================================================*/
/* ActionSequence.hpp - Action Sequence                                       */
/*============================================================================*/
#pragma once

namespace Maps
{
  static const i8    C_ACTION_SEQUENCE = C_1M * 10;
  static const char* C_ACTION_REGION   = "/SE_ActionSequence";

  class ActionSequence : public Map
  {
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
    static Action* pNewBodyAction(Action *pHeadAction);
    static Action* pNewHeadAction(void);

  private:
    Action* pAllotAction(void);
    void vDropLock(void);
    void vGrabLock(void);
    void vLinkAction(Action* pHead, Action* pBody);

    Footer* aFooter(void);
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
