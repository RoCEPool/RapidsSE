/*============================================================================*/
/* OpDefn.hpp -- a definition of an op class                                  */
/*============================================================================*/
#pragma once

namespace PushDown
{
  struct OpDefn
  {
    public:
      eCN  m_eClass;     // class enum of the op
      eOK  m_eKind;      // op kind
  
      Stz* m_szCommand;  // first word of prompt command
      Stz* m_szPrefix;   // prefix

      Op*  (*m_fpNew)(Stz* szLabel,      // new op fun
                      Digest* pDigest,  OpDefn* aOpDefn, bool bOutB,
                      TokenVec* pTknVec, i8& i_Tkn);
      eOR  (*m_fpCall) (void* aThisOp);  // function which calls the op loop
      eOR  (*m_fpDrain)(void* aThisOp);  // function which calls the drain loop

      Stz* m_szHelp;     // help text
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  OpDefn* _pOpDefn(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
