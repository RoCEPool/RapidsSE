/*============================================================================*/
/* Action.hpp - when did an action occur                                      */
/*============================================================================*/
#pragma once

namespace Measurement
{
  enum eAN // action numbers
  {
  /*  0 */ AN_NONE=0,
  
  /*    */ AN_ENUMS,
  };
  
  static Stz* aAN[]
  {
  /*  0 */ "NONE"_Stz,

  /*    */ "ENUMS"_Stz,
  };
  

  class Action : public Object
  {
  private:
    i8 m_iTick;
    i8 m_iRootId;
    i8 m_iThisId;
    i8 m_iAction;
    i8 m_iArg0;
    i8 m_iArg1;
    i8 m_iLink;
  
    // must be last
    Footer m_xFooter;
    
  public:
    Action();
    ~Action() noexcept(false);
  
    Footer* aFooter(void);
  };
  
  Action* _pAction(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
