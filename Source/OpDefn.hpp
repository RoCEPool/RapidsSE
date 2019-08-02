/*============================================================================*/
/* OpDefn.hpp -- a definition of an op class                                  */
/*============================================================================*/
#pragma once

namespace PushDown
{
  struct OpDefn
  {
    public:
      eCN  m_eClass;     // enum of the class
      eOK  m_eKind;      // op kind
  
      Stz* m_aCommand;   // first word of prompt command
  
      eOR  (*m_fpOpFun)(void* aThis);  // function which does the op
      void (*m_fpDrain)(void* aThis);  // function which drains the operand queue

    public:
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  OpDefn* _pOpDefn(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
