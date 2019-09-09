/*============================================================================*/
/* OpDefn.hpp -- a definition of an op class                                  */
/*============================================================================*/
#pragma once

namespace Operators
{
  struct OpDefn
  {
    public:
      eCN  m_eClass;     // class enum of the op
      eOK  m_eKind;      // op kind
  
      Stz* m_szCommand;  // first word of prompt command
      Stz* m_szPrefix;   // prefix
    
      bool m_bIsPartitioned; // is partitioned
    
      Op*  (*m_fpNewOp)(OpSpec* pOpSpec, Digest* pDigest,
                        OpDefn* aOpDefn, bool bAppR,
                        TokenVec* pTknVec, i8& i_Tkn, i8 i_Slot);
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
