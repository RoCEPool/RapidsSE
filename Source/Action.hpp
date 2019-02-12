/*============================================================================*/
/* Action.hpp - when did an action occur and what happened                    */
/*============================================================================*/
#pragma once

namespace Maps
{
  enum eAN // action numbers
  {
  /*  0 */ AN_NONE=0,
  
  /*    */ AN_BEGIN,
  /*    */ AN_CONSTRUCTOR,
  /*    */ AN_DESTRUCTOR,

  /*    */ AN_ADDED_ORIGINATOR,
  /*    */ AN_ADDED_PARTICIPANT,
  /*    */ AN_REMOVED_ORIGINATOR,
  /*    */ AN_REMOVED_PARTICIPANT,

  /*    */ AN_ENUMS,
  };
  
  static Stz* aAN[]
  {
  /*  0 */ "NONE"_Stz,

  /*    */ "BEGIN"_Stz,
  /*    */ "CONSTRUCTOR"_Stz,
  /*    */ "DESTRUCTOR"_Stz,

  /*    */ "ADDED_ORIGINATOR"_Stz,
  /*    */ "ADDED_PARTICIPANT"_Stz,
  
  /*    */ "REMOVED_ORIGINATOR"_Stz,
  /*    */ "REMOVED_PARTICIPANT"_Stz,
  
  /*    */ "ENUMS"_Stz,
  };

  class Action : public Object
  {
  friend class ActionSequence;
  
  private:
    i8  m_i_Action;
    i8  m_i_Root;
    i8  m_iTick;
  
    i8  m_iJobId;
    eCN m_eClassNo;
    eAN m_eAction;
    i8  m_iArg0;
    i8  m_iArg1;
  
    Link_t<i8> m_wLink;
  
    // must be last
    Footer m_xFooter;
    
  public:
    Action();
    ~Action() noexcept(false);
  
    void vInit(i8 i_Action);
    bool bIsRoot(void);
    void vReset(void);
    void vWrite(File* pFile);

    Footer* aFooter(void);
  };
  
  Action* _pAction(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
