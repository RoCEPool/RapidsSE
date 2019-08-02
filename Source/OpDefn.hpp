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
  
      Stz* m_szCommand;  // first word of prompt command
  
      Op   (*m_fpNew)(TokenVec* pTknVec, i8& i_Tkn);
  
      eOR  (*m_fpCall)(void* aThis);   // function which calls the op
      void (*m_fpDrain)(void* aThis);  // function which drains the operand queue

      Stz* m_szHelp;     // help text
    public:
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  OpDefn* _pOpDefn(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
