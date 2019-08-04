/*============================================================================*/
/* LimitOp.cpp -- Limit                                                       */
/*============================================================================*/

#include "PushDown.h"

OpDesc __Op_Limit (
  CN_LIMITOP,         // class number
  OK_SIMPLE,          // op kind
  "Limit"_Stz,        // first word of prompt command
  "Lim"_Stz,          // prefix

  LimitOp::pNew,      // function which parses the command and new's an op
  LimitOp::eCall,     // function which does the op
  nullptr,            // function which drains the operand queue

  "<help>"_Stz
  );


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
LimitOp* PushDown::_pLimitOp(void* aAddr)
{
  LimitOp* pLimitOp = reinterpret_cast<LimitOp*>(aAddr);
  
  return pLimitOp;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* LimitOp::operator new(size_t)
{
  LimitOp* pLimitOp = static_cast<LimitOp*>(HeapMem::pAllocate(CN_LIMITOP));
  
  return pLimitOp;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void LimitOp::operator delete(void* aAddress)
{
  LimitOp* pLimitOp = static_cast<LimitOp*>(aAddress);
  
  HeapMem::vFree(pLimitOp);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
LimitOp::LimitOp(Stz* szOpLabel, Digest* pDigest)
: Op(CN_LIMITOP,&__Op_Limit,szOpLabel,pDigest)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LIMITOP,_FOOTER_);
  
  m_fpOpFun = LimitOp::eCall;

  m_nLimit = 0;
  m_nSoFar = 0;
  
  _checkInitialised_(CN_LIMITOP);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_LIMITOP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
LimitOp::~LimitOp(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_LIMITOP);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse & new function                                                       */
/*----------------------------------------------------------------------------*/
Op* LimitOp::pNew(Digest* pDigest, Job_Dynamic* pJob, Stz* szLabel,
                  TokenVec* pTknVec, i8& i_Tkn_)
{
  i_Tkn_++;
  
  LimitOp* pLimitOp = new LimitOp(szLabel,pDigest);
  
  pLimitOp->m_nLimit = pTknVec->pGet(i_Tkn_++)->iToI8();
  
  return pLimitOp;
}

/*----------------------------------------------------------------------------*/
/* call the loop function                                                     */
/*----------------------------------------------------------------------------*/
eOR LimitOp::eCall(void* aThis)
{
  eOR eCallAgain = ((LimitOp*)aThis)->eLimitLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* the fetch operator loop function                                           */
/*----------------------------------------------------------------------------*/
eOR LimitOp::eLimitLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LIMITOP,_FOOTER_);
  
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
        
      case CN_CRWBUFFER:
        m_nSoFar++;
        break;
        
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

__aFooterFunction(LimitOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
