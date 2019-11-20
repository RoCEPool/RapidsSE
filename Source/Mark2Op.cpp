/*============================================================================*/
/* Mark2Op.cpp -- Output                                                     */
/*============================================================================*/

#include "DataOperators.h"

OpDesc __Op_Mark2Op (
  CN_MARK2OP,          // class number
  OK_SINGLE,           // op kind
  "mark2op"_Stz,       // first word of prompt command
  "Op2"_Stz,           // prefix
                  
  false,               // partitioned
  false,               // compiler op

  true,                // accepts CrwBuffer?
  true,                // accepts EntBuffer?
  true,                // accepts IrwBuffer?
  true,                // accepts SrtBuffer?
                    
  false,               // outputs CrwBuffer?
  false,               // outputs EntBuffer?
  false,               // outputs IrwBuffer?
  false,               // outputs SrtBuffer?

  CN_NONE,             // what kind of output
                  
  Mark2Op::pNew,       // function which parses the command and new's an op
  Mark2Op::eCall,      // function which calls the op loop
  Mark2Op::eDrain,     // function which calls the drain loop

  "mark2op"_Stz
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Mark2Op* DataOperators::_pMark2Op(void* aAddr)
{
  Mark2Op* pMark2Op = reinterpret_cast<Mark2Op*>(aAddr);
  
  return pMark2Op;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* Mark2Op::operator new(size_t)
{
  Mark2Op* pMark2Op =
    static_cast<Mark2Op*>(HeapMem::pAllocate(CN_MARK2OP));
  
  return pMark2Op;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void Mark2Op::operator delete(void* aAddress)
{
  Mark2Op* pMark2Op = static_cast<Mark2Op*>(aAddress);
  
  HeapMem::vFree(pMark2Op);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Mark2Op::Mark2Op(OpSpec* pOpSpec, Pipeline* pPipeline)
: Op(CN_MARK2OP,pOpSpec,pPipeline)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MARK2OP,_FOOTER_);
  
  m_fpCall = Mark2Op::eCall;
  
  _checkInitialised_(CN_MARK2OP);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_MARK2OP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Mark2Op::~Mark2Op(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MARK2OP,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse & new                                                                */
/*----------------------------------------------------------------------------*/
Op* Mark2Op::pNew(OpSpec* pOpSpec, Pipeline* pPipeline)
{  
  Mark2Op* pMark2Op = new Mark2Op(pOpSpec,pPipeline);
  
  return pMark2Op;
}

/*----------------------------------------------------------------------------*/
/* call loop                                                                  */
/*----------------------------------------------------------------------------*/
eOR Mark2Op::eCall(void* aThisOp)
{
  eOR eCallAgain = ((Mark2Op*)aThisOp)->eOpLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/* call drain loop                                                            */
/*----------------------------------------------------------------------------*/
eOR Mark2Op::eDrain(void* aThisOp)
{
  eOR eCallAgain = ((Mark2Op*)aThisOp)->eDrainLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* op loop                                                                    */
/*----------------------------------------------------------------------------*/
eOR Mark2Op::eOpLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MARK2OP,_FOOTER_);
  s4K sMetaData;
  
  for (;;) {
    
    Operand* pOperand = pPopQueue();
    
    //! HACK: TaskLoop_Op
    if (! pOperand) {
      //ASSERT(bIsLocked());
      //return OR_BIDE;
      continue;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    switch (pOperand->eClass()) {
      
      // -- -- -- -- -- --
        
      case CN_CRWBUFFER: {
        CrwBuffer* pCrwBuffer = _pCrwBuffer(pOperand);
        m_iWorkDone += pCrwBuffer->nRowCount();
        vPipeToJob(pOperand);
        break;
      }
  
      // -- -- -- -- -- --
        
      case CN_CSVBUFFER: {
        CsvBuffer* pCsvBuffer = _pCsvBuffer(pOperand);
        m_iWorkDone += pCsvBuffer->nRecords();
        vPipeToJob(pOperand);
        break;
      }
        
      // -- -- -- -- -- --
        
      case CN_ENDMARKER: {
        
        if (--m_nLeftEndMarkers == 0) {
          if (bIsCancelling()) {
            delete _pEndMarker(pOperand);
            vPipeToJob(new CancelMarker);
          }
          else
            vPipeToJob(pOperand);
          goto L_EXIT;
        }
        else
          delete (EndMarker*)pOperand;
        
        break;
      }
        
      // -- -- -- -- -- --
        
      case CN_ENTBUFFER: {
        EntBuffer* pEntBuffer = _pEntBuffer(pOperand);
        m_iWorkDone += pEntBuffer->nItems();
        vPipeToJob(pOperand);
        break;
      }
        
      // -- -- -- -- -- --
        
      case CN_IRWBUFFER: {
        IrwBuffer* pIrwBuffer = _pIrwBuffer(pOperand);
        m_iWorkDone += pIrwBuffer->nItems();
        vPipeToJob(pOperand);
        break;
      }
      
      // -- -- -- -- -- --
        
      case CN_SRTBUFFER: {
        SrtBuffer* pSrtBuffer = _pSrtBuffer(pOperand);
        m_iWorkDone += pSrtBuffer->nItems();
        vPipeToJob(pOperand);
        break;
      }
       
      // -- -- -- -- -- --
        
      case CN_STREAMDEF: {
        vPipeToJob(pOperand);
        break;
      }
        
      // -- -- -- -- -- --
        
      default:
        RASSERT(0);
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
eOR Mark2Op::eDrainLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MARK2OP,_FOOTER_);
  
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

__aFooterFunction(Mark2Op);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
