/*============================================================================*/
/* Op_ToCommand.cpp -- To Command Operator                                     */
/*============================================================================*/

#include "CompilerOperators.h"

OpDesc __Op_ToCommand (
   CN_OP_TOCOMMAND,      // class number
   OK_SIMPLE,            // op kind
   "ToCommand"_Stz,      // first word of prompt command
   "ToCommand"_Stz,      // prefix
   false,                // not partitioned
   Op_ToCommand::pNew,   // function which parses the command and new's an op
   Op_ToCommand::eCall,  // function which calls the op loop
   Op_ToCommand::eDrain, // function which calls the drain loop
   
   "<help>"_Stz
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Op_ToCommand* CompilerOperators::_pOp_ToCommand(void* aAddr)
{
  Op_ToCommand* pToCommandOp = reinterpret_cast<Op_ToCommand*>(aAddr);
  
  return pToCommandOp;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* Op_ToCommand::operator new(size_t)
{
  Op_ToCommand* pToCommandOp =
    static_cast<Op_ToCommand*>(HeapMem::pAllocate(CN_OP_TOCOMMAND));
  
  return pToCommandOp;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void Op_ToCommand::operator delete(void* aAddress)
{
  Op_ToCommand* pToCommandOp = static_cast<Op_ToCommand*>(aAddress);
  
  HeapMem::vFree(pToCommandOp);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Op_ToCommand::Op_ToCommand(Stz* szOpLabel, Digest* pDigest, OpDefn* aOpDefn, bool bAppR)
: Op(CN_OP_TOCOMMAND,szOpLabel,pDigest,aOpDefn,bAppR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP_TOCOMMAND,_FOOTER_);
  
  m_fpCall = Op_ToCommand::eCall;
  
  _checkInitialised_(CN_OP_TOCOMMAND);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_OP_TOCOMMAND,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Op_ToCommand::~Op_ToCommand(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP_TOCOMMAND,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse & new                                                                */
/*----------------------------------------------------------------------------*/
Op* Op_ToCommand::pNew(Stz* szLabel,
                   Digest* pDigest, OpDefn* aOpDefn, bool bAppR,
                   TokenVec* pTknVec, i8& i_Tkn_, i8 i_Slot)
{
  i_Tkn_++;
  ASSERT(i_Slot == -1);
  
  Op_ToCommand* pToCommandOp = new Op_ToCommand(szLabel,pDigest,aOpDefn,bAppR);
  
  return pToCommandOp;
}

/*----------------------------------------------------------------------------*/
/* call loop                                                                  */
/*----------------------------------------------------------------------------*/
eOR Op_ToCommand::eCall(void* aThisOp)
{
  eOR eCallAgain = ((Op_ToCommand*)aThisOp)->eOpLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/* call drain loop                                                            */
/*----------------------------------------------------------------------------*/
eOR Op_ToCommand::eDrain(void* aThisOp)
{
  eOR eCallAgain = ((Op_ToCommand*)aThisOp)->eDrainLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* op loop                                                                    */
/*----------------------------------------------------------------------------*/
eOR Op_ToCommand::eOpLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP_TOCOMMAND,_FOOTER_);
  
  for (;;) {
    
    Operand* pOperand = pPopQueue();
    
    if (! pOperand) {
      ASSERT(bIsLocked());
      return OR_BIDE;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    switch (pOperand->eClass()) {
        
      case CN_ENDMARKER:
        if (--m_nLeftEndMarkers == 0) {
          vPipeOut(pOperand);
          goto L_EXIT;
        }
        else
          delete (EndMarker*)pOperand;
        break;
        
        // -- -- -- -- -- --
        
      default:
        vPipeOut(pOperand);
        break;
    }

  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_EXIT:
  return OR_EXIT;
}

/*----------------------------------------------------------------------------*/
/* drain loop                                                                 */
/*----------------------------------------------------------------------------*/
eOR Op_ToCommand::eDrainLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP_TOCOMMAND,_FOOTER_);
  
  for (;;) {
    
    Operand* pOperand = pPopQueue();
    
    if (! pOperand) {
      ASSERT(bIsLocked());
      return OR_BIDE;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    switch (pOperand->eClass()) {
        
      case CN_ENDMARKER:
        vPipeOut(pOperand);
        goto L_EXIT;
        break;
        
        // -- -- -- -- -- --
        
      default:
        vPipeOut(pOperand);
        break;
    }

  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
L_EXIT:
  return OR_EXIT;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Op_ToCommand);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
