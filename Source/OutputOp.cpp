/*============================================================================*/
/* OutputOp.cpp -- Output                                                     */
/*============================================================================*/

#include "PushDown.h"

OpDesc __Op_Output (
   CN_OUTPUTOP,         // class number
   OK_SIMPLE,           // op kind
   "Output"_Stz,        // first word of prompt command
   "Out"_Stz,           // prefix

   OutputOp::pNew,      // function which parses the command and new's an op
   OutputOp::eCall,     // function which calls the op loop
   OutputOp::eDrain,    // function which calls the drain loop
   
   "<help>"_Stz
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
OutputOp* PushDown::_pOutputOp(void* aAddr)
{
  OutputOp* pOutputOp = reinterpret_cast<OutputOp*>(aAddr);
  
  return pOutputOp;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* OutputOp::operator new(size_t)
{
  OutputOp* pOutputOp =
    static_cast<OutputOp*>(HeapMem::pAllocate(CN_OUTPUTOP));
  
  return pOutputOp;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void OutputOp::operator delete(void* aAddress)
{
  OutputOp* pOutputOp = static_cast<OutputOp*>(aAddress);
  
  HeapMem::vFree(pOutputOp);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
OutputOp::OutputOp(Stz* szOpLabel, Digest* pDigest, OpDefn* aOpDefn)
: Op(CN_OUTPUTOP,szOpLabel,pDigest,aOpDefn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUTOP,_FOOTER_);
  
  m_fpCall = OutputOp::eCall;
  
  _checkInitialised_(CN_OUTPUTOP);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_OUTPUTOP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
OutputOp::~OutputOp(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUTOP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse & new                                                                */
/*----------------------------------------------------------------------------*/
Op* OutputOp::pNew(Stz* szLabel, Digest* pDigest, OpDefn* aOpDefn,
                   TokenVec* pTknVec, i8& i_Tkn_)
{
  i_Tkn_++;
  
  OutputOp* pOutputOp = new OutputOp(szLabel,pDigest,aOpDefn);
  
  return pOutputOp;
}

/*----------------------------------------------------------------------------*/
/* call loop                                                                  */
/*----------------------------------------------------------------------------*/
eOR OutputOp::eCall(void* aThisOp)
{
  eOR eCallAgain = ((OutputOp*)aThisOp)->eOpLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/* call drain loop                                                            */
/*----------------------------------------------------------------------------*/
eOR OutputOp::eDrain(void* aThisOp)
{
  eOR eCallAgain = ((OutputOp*)aThisOp)->eDrainLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* op loop                                                                    */
/*----------------------------------------------------------------------------*/
eOR OutputOp::eOpLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUTOP,_FOOTER_);
  
  for (;;) {
    
    Operand* pOperand = pPopOperand();
    
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
        
      case CN_INTBUFFER: {
        IntBuffer* pIntBuffer = _pIntBuffer(pOperand) ;
        pIntBuffer->vShow();
        vPipeOut(pOperand);
        break;
      }
       
      case CN_ENTBUFFER: {
        EntBuffer* pEntBuffer = _pEntBuffer(pOperand) ;
        pEntBuffer->vShow();
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
eOR OutputOp::eDrainLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUTOP,_FOOTER_);
  
  for (;;) {
    
    Operand* pOperand = pPopOperand();
    
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

__aFooterFunction(OutputOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
