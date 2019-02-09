/*============================================================================*/
/* ActionJournal.hpp - Action Journal                                         */
/*============================================================================*/
#pragma once

namespace Jobs { class Job; }

namespace Maps
{
  static const i8    C_ACTION_JOURNAL = C_1M * 10;
  static const char* C_ACTION_REGION  = "/SE_ActionJournal";

  class ActionJournal : public Map
  {
  private:
    Lock m_okLock;
    i8   m_i_Action;  // current subscript
    i8   m_nActions;  // number of available actions in array

    Action m_oaAction[C_ACTION_JOURNAL];

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t, ActionJournal* pActionJournal);
    void  operator delete(void*);

    ActionJournal(void);
    ~ActionJournal(void) noexcept(false);

  private:
    Footer* aFooter(void);
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
