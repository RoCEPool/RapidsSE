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
   OutputOp::eCall,     // function which does the op
   nullptr,             // function which drains the operand queue
   
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
OutputOp::OutputOp(Stz* szOpLabel, Digest* pDigest, Jobs::Job* pJob)
: Op(CN_OUTPUTOP,&__Op_Output,szOpLabel,pJob)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUTOP,_FOOTER_);
  
  m_fpOpFun = OutputOp::eCall;
  
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
/* function which parses the command and new's an op                          */
/*----------------------------------------------------------------------------*/
Op* OutputOp::pNew(Digest* pDigest, Job_Dynamic* pJob, Stz* szLabel,
                   TokenVec* pTknVec, i8& i_Tkn_)
{
  i_Tkn_++;
  
  OutputOp* pOutputOp = new OutputOp(szLabel,pDigest,(Job*)pJob);
  
  return pOutputOp;
}

/*----------------------------------------------------------------------------*/
/* call the output loop function                                              */
/*----------------------------------------------------------------------------*/
eOR OutputOp::eCall(void* aThis)
{
  eOR eCallAgain = ((OutputOp*)aThis)->eOutputLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* the output operator loop function                                          */
/*----------------------------------------------------------------------------*/
eOR OutputOp::eOutputLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OUTPUTOP,_FOOTER_);
  
  for (;;) {
    
    Operand* pOperand = pPopOperand();
    
    if (! pOperand) {
      ASSERT(bIsLocked());
      return OR_BIDE;
    }
    
    switch (pOperand->eClass()) {
        
      case CN_ENDMARKER:
       vPipeOut(pOperand);
       goto L_EXIT;
        
      case CN_INTBUFFER: {
        IntBuffer* pIntBuffer = _pIntBuffer(pOperand) ;
        pIntBuffer->vShow();
        break;
      }
        
      default:
        break;
    }
    
    vPipeOut(pOperand);
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
