/*============================================================================*/
/* ToClauseOp.cpp -- To Clause Operator                                       */
/*============================================================================*/

#include "DataOperators.h"

OpDesc __Op_ToClause (
   CN_OUTPUTOP,         // class number
   OK_SIMPLE,           // op kind
   "ToClause"_Stz,      // first word of prompt command
   "Clu"_Stz,           // prefix
   false,               // not partitioned
   ToClauseOp::pNew,    // function which parses the command and new's an op
   ToClauseOp::eCall,   // function which calls the op loop
   ToClauseOp::eDrain,  // function which calls the drain loop
   
   "<help>"_Stz
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
ToClauseOp* DataOperators::_pToClauseOp(void* aAddr)
{
  ToClauseOp* pToClauseOp = reinterpret_cast<ToClauseOp*>(aAddr);
  
  return pToClauseOp;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* ToClauseOp::operator new(size_t)
{
  ToClauseOp* pToClauseOp =
    static_cast<ToClauseOp*>(HeapMem::pAllocate(CN_OUTPUTOP));
  
  return pToClauseOp;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void ToClauseOp::operator delete(void* aAddress)
{
  ToClauseOp* pToClauseOp = static_cast<ToClauseOp*>(aAddress);
  
  HeapMem::vFree(pToClauseOp);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ToClauseOp::ToClauseOp(Stz* szOpLabel, Digest* pDigest, OpDefn* aOpDefn, bool bAppR)
: Op(CN_OUTPUTOP,szOpLabel,pDigest,aOpDefn,bAppR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUTOP,_FOOTER_);
  
  m_fpCall = ToClauseOp::eCall;
  
  _checkInitialised_(CN_OUTPUTOP);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_OUTPUTOP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ToClauseOp::~ToClauseOp(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUTOP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse & new                                                                */
/*----------------------------------------------------------------------------*/
Op* ToClauseOp::pNew(Stz* szLabel,
                   Digest* pDigest, OpDefn* aOpDefn, bool bAppR,
                   TokenVec* pTknVec, i8& i_Tkn_, i8 i_Slot)
{
  i_Tkn_++;
  ASSERT(i_Slot == -1);
  
  ToClauseOp* pToClauseOp = new ToClauseOp(szLabel,pDigest,aOpDefn,bAppR);
  
  return pToClauseOp;
}

/*----------------------------------------------------------------------------*/
/* call loop                                                                  */
/*----------------------------------------------------------------------------*/
eOR ToClauseOp::eCall(void* aThisOp)
{
  eOR eCallAgain = ((ToClauseOp*)aThisOp)->eOpLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/* call drain loop                                                            */
/*----------------------------------------------------------------------------*/
eOR ToClauseOp::eDrain(void* aThisOp)
{
  eOR eCallAgain = ((ToClauseOp*)aThisOp)->eDrainLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* op loop                                                                    */
/*----------------------------------------------------------------------------*/
eOR ToClauseOp::eOpLoop(void)
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
eOR ToClauseOp::eDrainLoop(void)
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

__aFooterFunction(ToClauseOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
