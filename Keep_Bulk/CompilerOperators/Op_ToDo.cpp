/*============================================================================*/
/* Op_ToDo.cpp -- To Do Operator                                              */
/*============================================================================*/

#include "CompilerOperators.h"

OpDesc __Op_ToDo (
  CN_OP_TODO,      // class number
  OK_SINGLE,       // op kind
  "ToDo"_Stz,      // first word of prompt command
  "ToDo"_Stz,      // prefix
                
  false,           // not partitioned
  true,            // compiler op

  false,              // accepts CrwBuffer?
  false,              // accepts EntBuffer?
  false,              // accepts IrwBuffer?
  false,              // accepts SrtBuffer?
                  
  false,              // accepts CrwBuffer?
  false,              // accepts EntBuffer?
  false,              // accepts IrwBuffer?
  false,              // accepts SrtBuffer?
                  
  CN_NONE,         // what kind of output
                
  Op_ToDo::pNew,   // function which parses the command and new's an op
  Op_ToDo::eCall,  // function which calls the op loop
  Op_ToDo::eDrain, // function which calls the drain loop

  "<help>"_Stz
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Op_ToDo* CompilerOperators::_pOp_ToDo(void* aAddr)
{
  Op_ToDo* pToDataOp = reinterpret_cast<Op_ToDo*>(aAddr);
  
  return pToDataOp;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* Op_ToDo::operator new(size_t)
{
  Op_ToDo* pToDataOp =
    static_cast<Op_ToDo*>(HeapMem::pAllocate(CN_OP_TODO));
  
  return pToDataOp;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void Op_ToDo::operator delete(void* aAddress)
{
  Op_ToDo* pToDataOp = static_cast<Op_ToDo*>(aAddress);
  
  HeapMem::vFree(pToDataOp);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Op_ToDo::Op_ToDo(OpSpec* pOpSpec, Pipeline* pPipeline)
: Op(CN_OP_TODO,pOpSpec,pPipeline)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP_TODO,_FOOTER_);
  
  m_fpCall = Op_ToDo::eCall;
  
  _checkInitialised_(CN_OP_TODO);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_OP_TODO,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Op_ToDo::~Op_ToDo(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP_TODO,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse & new                                                                */
/*----------------------------------------------------------------------------*/
Op* Op_ToDo::pNew(OpSpec* pOpSpec, Pipeline* pPipeline)
{
  Op_ToDo* pToDataOp = new Op_ToDo(pOpSpec,pPipeline);
  
  return pToDataOp;
}

/*----------------------------------------------------------------------------*/
/* call loop                                                                  */
/*----------------------------------------------------------------------------*/
eOR Op_ToDo::eCall(void* aThisOp)
{
  eOR eCallAgain = ((Op_ToDo*)aThisOp)->eOpLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/* call drain loop                                                            */
/*----------------------------------------------------------------------------*/
eOR Op_ToDo::eDrain(void* aThisOp)
{
  eOR eCallAgain = ((Op_ToDo*)aThisOp)->eDrainLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* op loop                                                                    */
/*----------------------------------------------------------------------------*/
eOR Op_ToDo::eOpLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP_TODO,_FOOTER_);
  
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
        
      case CN_CMD_SELECT:
        vPromptHandOff((Cmd_Select*)pOperand);
        vPipeOut(pOperand);
        break;
        
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
eOR Op_ToDo::eDrainLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP_TODO,_FOOTER_);
  
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
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* give command to prompt to execute                                          */
/*----------------------------------------------------------------------------*/
void Op_ToDo::vPromptHandOff(Cmd_Select* pCmd_Select)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP_TODO,_FOOTER_);
  
  if (AM::bVerbose())
    vTrace("  vPromptHandOff: %s\n"_Stz,pCmd_Select->szCmd());
  
//  Session* pSession = AM::pThisTLSSession();
//  
//  pSession->m_sToDo = pCmd_Select->szCmd();
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Op_ToDo);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
