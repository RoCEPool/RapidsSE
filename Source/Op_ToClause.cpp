/*============================================================================*/
/* Op_ToClause.cpp -- To Clause Operator                                      */
/*============================================================================*/

#include "CompilerOperators.h"

OpDesc __Op_ToClause (
   CN_OUTPUTOP,         // class number
   OK_SIMPLE,           // op kind
   "ToClause"_Stz,      // first word of prompt command
   "Clu"_Stz,           // prefix
   false,               // not partitioned
   Op_ToClause::pNew,    // function which parses the command and new's an op
   Op_ToClause::eCall,   // function which calls the op loop
   Op_ToClause::eDrain,  // function which calls the drain loop
   
   "<help>"_Stz
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Op_ToClause* CompilerOperators::_pOp_ToClause(void* aAddr)
{
  Op_ToClause* pToClauseOp = reinterpret_cast<Op_ToClause*>(aAddr);
  
  return pToClauseOp;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* Op_ToClause::operator new(size_t)
{
  Op_ToClause* pToClauseOp =
    static_cast<Op_ToClause*>(HeapMem::pAllocate(CN_OUTPUTOP));
  
  return pToClauseOp;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void Op_ToClause::operator delete(void* aAddress)
{
  Op_ToClause* pToClauseOp = static_cast<Op_ToClause*>(aAddress);
  
  HeapMem::vFree(pToClauseOp);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Op_ToClause::Op_ToClause(Stz* szOpLabel, Digest* pDigest, OpDefn* aOpDefn, bool bAppR)
: Op(CN_OUTPUTOP,szOpLabel,pDigest,aOpDefn,bAppR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUTOP,_FOOTER_);
  
  m_fpCall = Op_ToClause::eCall;
  
  _checkInitialised_(CN_OUTPUTOP);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_OUTPUTOP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Op_ToClause::~Op_ToClause(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUTOP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse & new                                                                */
/*----------------------------------------------------------------------------*/
Op* Op_ToClause::pNew(Stz* szLabel,
                   Digest* pDigest, OpDefn* aOpDefn, bool bAppR,
                   TokenVec* pTknVec, i8& i_Tkn_, i8 i_Slot)
{
  i_Tkn_++;
  ASSERT(i_Slot == -1);
  
  Op_ToClause* pToClauseOp = new Op_ToClause(szLabel,pDigest,aOpDefn,bAppR);
  
  return pToClauseOp;
}

/*----------------------------------------------------------------------------*/
/* call loop                                                                  */
/*----------------------------------------------------------------------------*/
eOR Op_ToClause::eCall(void* aThisOp)
{
  eOR eCallAgain = ((Op_ToClause*)aThisOp)->eOpLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/* call drain loop                                                            */
/*----------------------------------------------------------------------------*/
eOR Op_ToClause::eDrain(void* aThisOp)
{
  eOR eCallAgain = ((Op_ToClause*)aThisOp)->eDrainLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* op loop                                                                    */
/*----------------------------------------------------------------------------*/
eOR Op_ToClause::eOpLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUTOP,_FOOTER_);
  
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
       
      case CN_ENTBUFFER: {
        EntBuffer* pEntBuffer = _pEntBuffer(pOperand) ;
        pEntBuffer->vShow();
        vPipeOut(pOperand);
        break;
      }
        
      case CN_INTBUFFER: {
        IntBuffer* pIntBuffer = _pIntBuffer(pOperand) ;
        pIntBuffer->vShow();
        vPipeOut(pOperand);
        break;
      }
      
      case CN_REFBUFFER: {
        RefBuffer* pRefBuffer = _pRefBuffer(pOperand) ;
        pRefBuffer->vShow();
        vPipeOut(pOperand);
        break;
      }
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
eOR Op_ToClause::eDrainLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUTOP,_FOOTER_);
  
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
        
      // -- -- -- -- -- --
        
      case CN_PAUSEMARKER:
        vPipeOut(pOperand);
        break;
        
      // -- -- -- -- -- --
        
      case CN_INTBUFFER:
        //vPipeOut(pOperand);
        break;
        
      default:
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

__aFooterFunction(Op_ToClause);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
