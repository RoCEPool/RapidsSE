/*============================================================================*/
/* Op_ToData.cpp -- To Clause Operator                                       */
/*============================================================================*/

#include "CompilerOperators.h"

OpDesc __Op_ToData (
   CN_OUTPUTOP,         // class number
   OK_SIMPLE,           // op kind
   "ToClause"_Stz,      // first word of prompt command
   "Dta"_Stz,           // prefix
   false,               // not partitioned
   Op_ToData::pNew,      // function which parses the command and new's an op
   Op_ToData::eCall,     // function which calls the op loop
   Op_ToData::eDrain,    // function which calls the drain loop
   
   "<help>"_Stz
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Op_ToData* CompilerOperators::_pOp_ToData(void* aAddr)
{
  Op_ToData* pToDataOp = reinterpret_cast<Op_ToData*>(aAddr);
  
  return pToDataOp;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* Op_ToData::operator new(size_t)
{
  Op_ToData* pToDataOp =
    static_cast<Op_ToData*>(HeapMem::pAllocate(CN_OUTPUTOP));
  
  return pToDataOp;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void Op_ToData::operator delete(void* aAddress)
{
  Op_ToData* pToDataOp = static_cast<Op_ToData*>(aAddress);
  
  HeapMem::vFree(pToDataOp);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Op_ToData::Op_ToData(Stz* szOpLabel, Digest* pDigest, OpDefn* aOpDefn, bool bAppR)
: Op(CN_OUTPUTOP,szOpLabel,pDigest,aOpDefn,bAppR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUTOP,_FOOTER_);
  
  m_fpCall = Op_ToData::eCall;
  
  _checkInitialised_(CN_OUTPUTOP);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_OUTPUTOP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Op_ToData::~Op_ToData(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUTOP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse & new                                                                */
/*----------------------------------------------------------------------------*/
Op* Op_ToData::pNew(Stz* szLabel,
                   Digest* pDigest, OpDefn* aOpDefn, bool bAppR,
                   TokenVec* pTknVec, i8& i_Tkn_, i8 i_Slot)
{
  i_Tkn_++;
  ASSERT(i_Slot == -1);
  
  Op_ToData* pToDataOp = new Op_ToData(szLabel,pDigest,aOpDefn,bAppR);
  
  return pToDataOp;
}

/*----------------------------------------------------------------------------*/
/* call loop                                                                  */
/*----------------------------------------------------------------------------*/
eOR Op_ToData::eCall(void* aThisOp)
{
  eOR eCallAgain = ((Op_ToData*)aThisOp)->eOpLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/* call drain loop                                                            */
/*----------------------------------------------------------------------------*/
eOR Op_ToData::eDrain(void* aThisOp)
{
  eOR eCallAgain = ((Op_ToData*)aThisOp)->eDrainLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* op loop                                                                    */
/*----------------------------------------------------------------------------*/
eOR Op_ToData::eOpLoop(void)
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
eOR Op_ToData::eDrainLoop(void)
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

__aFooterFunction(Op_ToData);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
