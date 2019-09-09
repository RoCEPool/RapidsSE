/*============================================================================*/
/* OpDesc.hpp -- the class which populates the op catalog                     */
/*============================================================================*/
#pragma once

namespace Operators
{
  class OpDesc
  {
  public:

  public:
    OpDesc(eCN  eClass,     // class number
           eOK  eKind,      // op kind
           Stz* szCommand,  // first word of prompt command
           Stz* szPrefix,   // prefix
           bool bIsPartitioned, // is operator partitioned
           Op*  (*fpNewOp)(OpSpec* pOpSpec, Digest* pDigest,
                           OpDefn* aOpDefn, bool bAppR,
                           TokenVec* pTknVec, i8& i_Tkn_, i8 i_Slot),
           
           eOR  (*fpCall)(void* aThisOp),  // function which calls the op loop
           eOR  (*fpDrain)(void* aThisOp), // function which drains the drain loop
           Stz* szHelp      // help text
          );
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
