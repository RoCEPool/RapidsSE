/*============================================================================*/
/* ChunkFileOp.cpp -- ChunkFile Op                                            */
/*============================================================================*/

#include "DataOperators.h"

OpDesc __Op_ChunkFile (
  CN_CHUNKFILEOP,     // class number
  OK_SINGLE,          // op kind
  "ChunkFile"_Stz,    // first word of prompt command
  "Chf"_Stz,          // prefix
                 
  true,               // partitioned
  false,              // compiler op
                 
  true,               // accepts CrwBuffer?
  true,               // accepts EntBuffer?
  true,               // accepts IrwBuffer?
  true,               // accepts SrtBuffer?
                   
  false,              // accepts CrwBuffer?
  false,              // accepts EntBuffer?
  true,               // accepts IrwBuffer?
  false,              // accepts SrtBuffer?
                   
  CN_NONE,            // what kind of output

  ChunkFileOp::pNew,      // function which parses the command and new's an op
  ChunkFileOp::eCall,     // function which calls the op loop
  ChunkFileOp::eDrain,    // function which calls the drain loop

  "chunkFile"_Stz
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
ChunkFileOp* DataOperators::_pChunkFileOp(void* aAddr)
{
  ChunkFileOp* pChunkFileOp = reinterpret_cast<ChunkFileOp*>(aAddr);
  
  return pChunkFileOp;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* ChunkFileOp::operator new(size_t)
{
  ChunkFileOp* pChunkFileOp =
    static_cast<ChunkFileOp*>(HeapMem::pAllocate(CN_CHUNKFILEOP));
  
  return pChunkFileOp;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void ChunkFileOp::operator delete(void* aAddress)
{
  ChunkFileOp* pChunkFileOp = static_cast<ChunkFileOp*>(aAddress);
  
  HeapMem::vFree(pChunkFileOp);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ChunkFileOp::ChunkFileOp(OpSpec* pOpSpec, Pipeline* pPipeline)
: Op(CN_CHUNKFILEOP,pOpSpec,pPipeline)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHUNKFILEOP,_FOOTER_);
  
  m_fpCall = ChunkFileOp::eCall;
  
  _checkInitialised_(CN_CHUNKFILEOP);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CHUNKFILEOP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ChunkFileOp::~ChunkFileOp(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHUNKFILEOP,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse & new                                                                */
/*----------------------------------------------------------------------------*/
Op* ChunkFileOp::pNew(OpSpec* pOpSpec, Pipeline* pPipeline)
{
  ChunkFileOp* pCountOp = new ChunkFileOp(pOpSpec,pPipeline);
  
  return pCountOp;
}

/*----------------------------------------------------------------------------*/
/* call loop                                                                  */
/*----------------------------------------------------------------------------*/
eOR ChunkFileOp::eCall(void* aThisOp)
{
  eOR eCallAgain = ((ChunkFileOp*)aThisOp)->eOpLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/* call drain loop                                                            */
/*----------------------------------------------------------------------------*/
eOR ChunkFileOp::eDrain(void* aThisOp)
{
  eOR eCallAgain = ((ChunkFileOp*)aThisOp)->eDrainLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* op loop                                                                    */
/*----------------------------------------------------------------------------*/
eOR ChunkFileOp::eOpLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHUNKFILEOP,_FOOTER_);
  
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
        
      case CN_ENDMARKER:
        if (--m_nLeftEndMarkers == 0) {
          
          // now pipe out the end marker
          vPipeOut(pOperand);
          goto L_EXIT;
        }
        else
          delete (EndMarker*)pOperand;
        
        break;
      
      // -- -- -- -- -- --
        
      case CN_STREAMDEF: {
        delete _pStreamDef(pOperand);
        break;
      }
      
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
/* drain loop                                                                 */
/*----------------------------------------------------------------------------*/
eOR ChunkFileOp::eDrainLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CHUNKFILEOP,_FOOTER_);
  
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

__aFooterFunction(ChunkFileOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
