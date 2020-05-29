/*============================================================================*/
/* Op_ToCommand.cpp -- To Request Operator                                     */
/*============================================================================*/

#include "CompilerOperators.h"

OpDesc __Op_ToCommand (
  CN_OP_TOCOMMAND,      // class number
  OK_SINGLE,            // op kind
  "ToCommand"_Stz,      // first word of prompt command
  "ToCommand"_Stz,      // prefix
                     
  false,                // not partitioned
  true,                 // compiler op

  false,              // accepts CrwBuffer?
  false,              // accepts EntBuffer?
  false,              // accepts IrwBuffer?
  false,              // accepts SrtBuffer?
                       
  false,              // accepts CrwBuffer?
  false,              // accepts EntBuffer?
  false,              // accepts IrwBuffer?
  false,              // accepts SrtBuffer?
                       
  CN_NONE,              // what kind of output
                     
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
Op_ToCommand::Op_ToCommand(OpSpec* pOpSpec, Pipeline* pPipeline)
: Op(CN_OP_TOCOMMAND,pOpSpec,pPipeline)
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
Op* Op_ToCommand::pNew(OpSpec* pOpSpec, Pipeline* pPipeline)
{
  Op_ToCommand* pToCommandOp = new Op_ToCommand(pOpSpec,pPipeline);
  
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
          vConcludeCmd();
          vPipeCmd();
          vPipeOut(pOperand);
          goto L_EXIT;
        }
        else
          delete (EndMarker*)pOperand;
        break;
        
      // -- -- -- -- -- --
        
      case CN_FILTER_FROM:
      case CN_FILTER_GROUPBY:
      case CN_FILTER_INFLATE:
      case CN_FILTER_JOIN:
      case CN_FILTER_LIMIT:
      case CN_FILTER_ORDERBY:
      case CN_FILTER_OUTPUT:
      case CN_FILTER_PROJECT:
      case CN_FILTER_SCAN:
      case CN_FILTER_SELECT:
      case CN_FILTER_WHERE:
      case CN_FILTER_ZED:
        vCollectFilter((Filter*)pOperand);
        //vPipeOut(pOperand);
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
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* collect filters                                                            */
/*----------------------------------------------------------------------------*/
void Op_ToCommand::vCollectFilter(Filter* pFilter)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP_TOCOMMAND,_FOOTER_);
  
  s4K sFilter;
  pFilter->vAsString(sFilter);
  
  if (! *m_sCmd.azChar()) {
    m_sCmd = "pipeline "_Stz;
    m_sCmd.vConcat(sFilter);
    return;
  }
  
  m_sCmd.vConcat(" | "_Stz);
  m_sCmd.vConcat(sFilter);
}

/*----------------------------------------------------------------------------*/
/* conclude command                                                           */
/*----------------------------------------------------------------------------*/
void Op_ToCommand::vConcludeCmd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP_TOCOMMAND,_FOOTER_);
  
  //m_sCmd.vConcat(" ;"_Stz);
}

/*----------------------------------------------------------------------------*/
/* pipe command (collected from filters)                                      */
/*----------------------------------------------------------------------------*/
void Op_ToCommand::vPipeCmd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP_TOCOMMAND,_FOOTER_);
  
  Cmd_Select* pCmd_Select = new Cmd_Select(m_sCmd.szStz());
  
  vPipeOut(pCmd_Select);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Op_ToCommand);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
