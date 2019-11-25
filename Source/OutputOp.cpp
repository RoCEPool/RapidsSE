/*============================================================================*/
/* OutputOp.cpp -- Output                                                     */
/*============================================================================*/

#include "DataOperators.h"

OpDesc __Op_Output (
  CN_OUTPUTOP,         // class number
  OK_SINK,             // op kind
  "OutputOp"_Stz,      // first word of prompt command
  "Out"_Stz,           // prefix
                  
  false,               // partitioned
  false,               // compiler op

  true,                // accepts CrwBuffer?
  true,                // accepts EntBuffer?
  true,                // accepts IrwBuffer?
  true,                // accepts SrtBuffer?
                    
  false,            // accepts CrwBuffer?
  false,            // accepts EntBuffer?
  false,            // accepts IrwBuffer?
  false,            // accepts SrtBuffer?

  CN_NONE,             // what kind of output
                  
  OutputOp::pNew,      // function which parses the command and new's an op
  OutputOp::eCall,     // function which calls the op loop
  OutputOp::eDrain,    // function which calls the drain loop

  "outputOp"_Stz
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
OutputOp* DataOperators::_pOutputOp(void* aAddr)
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
OutputOp::OutputOp(OpSpec* pOpSpec, Pipeline* pPipeline)
: Op(CN_OUTPUTOP,pOpSpec,pPipeline)
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
Op* OutputOp::pNew(OpSpec* pOpSpec, Pipeline* pPipeline)
{  
  OutputOp* pOutputOp = new OutputOp(pOpSpec,pPipeline);
  
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
  s4K sMetaData;
  
  RASSERT(0);
  
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
eOR OutputOp::eDrainLoop(void)
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

__aFooterFunction(OutputOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
