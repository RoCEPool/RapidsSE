/*============================================================================*/
/* ToFileOp.cpp -- write to file operator                                      */
/*============================================================================*/

#include "DataOperators.h"

OpDesc __Op_ToFile (
  CN_TOFILEOP,        // class number
  OK_SINGLE,          // op kind
  "ToFileOp"_Stz,     // first word of prompt command
  "Fil"_Stz,          // prefix
                 
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

  ToFileOp::pNew,      // function which parses the command and new's an op
  ToFileOp::eCall,     // function which calls the op loop
  ToFileOp::eDrain,    // function which calls the drain loop

  "ToCsv"_Stz
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
ToFileOp* DataOperators::_pToFileOp(void* aAddr)
{
  ToFileOp* pToFileOp = reinterpret_cast<ToFileOp*>(aAddr);
  
  return pToFileOp;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* ToFileOp::operator new(size_t)
{
  ToFileOp* pToFileOp =
    static_cast<ToFileOp*>(HeapMem::pAllocate(CN_TOFILEOP));
  
  return pToFileOp;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void ToFileOp::operator delete(void* aAddress)
{
  ToFileOp* pToFileOp = static_cast<ToFileOp*>(aAddress);
  
  HeapMem::vFree(pToFileOp);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ToFileOp::ToFileOp(OpSpec* pOpSpec, Pipeline* pPipeline)
: Op(CN_TOFILEOP,pOpSpec,pPipeline)
, m_oOutFile(this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOFILEOP,_FOOTER_);
  
  m_fpCall = ToFileOp::eCall;
  
  _checkInitialised_(CN_TOFILEOP);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_TOFILEOP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ToFileOp::~ToFileOp(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOFILEOP,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse & new                                                                */
/*----------------------------------------------------------------------------*/
Op* ToFileOp::pNew(OpSpec* pOpSpec, Pipeline* pPipeline)
{
  
  ToFileOp* pToFileOp = new ToFileOp(pOpSpec,pPipeline);
  
  s4K sFilePath;
  sFilePath = pToFileOp->m_pOpSpec->pEnameSet()->szValue((i8)0);
  
  if (! pToFileOp->m_oOutFile.bOpenWrite(sFilePath)) {
    vTrace("Failed to open file: %s\n"_Stz,sFilePath.azChar());
    RASSERT(0);
  }
  
  return pToFileOp;
}

/*----------------------------------------------------------------------------*/
/* call loop                                                                  */
/*----------------------------------------------------------------------------*/
eOR ToFileOp::eCall(void* aThisOp)
{
  eOR eCallAgain = ((ToFileOp*)aThisOp)->eOpLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/* call drain loop                                                            */
/*----------------------------------------------------------------------------*/
eOR ToFileOp::eDrain(void* aThisOp)
{
  eOR eCallAgain = ((ToFileOp*)aThisOp)->eDrainLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* op loop                                                                    */
/*----------------------------------------------------------------------------*/
eOR ToFileOp::eOpLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOFILEOP,_FOOTER_);
  
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
          
          m_oOutFile.vClose();
          
          // pipe out the streamdef
          //vPipeOut(new StreamDef(m_pStreamDef));
          
          // now pipe out the end marker
          vPipeOut(pOperand);
          goto L_EXIT;
        }
        else
          delete (EndMarker*)pOperand;
        
        break;
        
      // -- -- -- -- -- --
        
      case CN_CSVBUFFER: {
        CsvBuffer* pCsvBuffer = _pCsvBuffer(pOperand) ;
        char* aData = pCsvBuffer->aDataArea();
        
        m_oOutFile.vPrint("%s"_Stz,aData);
        m_oOutFile.vFlush();
        break;
      }
      
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
eOR ToFileOp::eDrainLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOFILEOP,_FOOTER_);
  
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

__aFooterFunction(ToFileOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
