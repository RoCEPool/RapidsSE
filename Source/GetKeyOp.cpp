/*============================================================================*/
/* GetKeyOp.cpp -- Output                                                     */
/*============================================================================*/

#include "DataOperators.h"

OpDesc __Op_GetKeyOp (
  CN_GETKEYOP,         // class number
  OK_SINGLE,           // op kind
  "GetKey"_Stz,        // first word of prompt command
  "Gky"_Stz,           // prefix
                    
  true,                // partitioned
  false,               // compiler op
                    
  true,                // accepts CrwBuffer?
  true,                // accepts EntBuffer?
  false,               // accepts IrwBuffer?
  true,                // accepts SrtBuffer?
                      
  false,               // accepts CrwBuffer?
  false,               // accepts EntBuffer?
  false,               // accepts IrwBuffer?
  true,                // accepts SrtBuffer?
                      
  CN_NONE,             // what kind of output
                    
  GetKeyOp::pNew,      // function which parses the command and new's an op
  GetKeyOp::eCall,     // function which calls the op loop
  GetKeyOp::eDrain,    // function which calls the drain loop

  "getKey <colNm>"_Stz
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
GetKeyOp* DataOperators::_pGetKeyOp(void* aAddr)
{
  GetKeyOp* pGetKeyOp = reinterpret_cast<GetKeyOp*>(aAddr);
  
  return pGetKeyOp;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* GetKeyOp::operator new(size_t)
{
  GetKeyOp* pGetKeyOp =
    static_cast<GetKeyOp*>(HeapMem::pAllocate(CN_GETKEYOP));
  
  return pGetKeyOp;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void GetKeyOp::operator delete(void* aAddress)
{
  GetKeyOp* pGetKeyOp = static_cast<GetKeyOp*>(aAddress);
  
  HeapMem::vFree(pGetKeyOp);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
GetKeyOp::GetKeyOp(OpSpec* pOpSpec, Pipeline* pPipeline)
: Op(CN_GETKEYOP,pOpSpec,pPipeline)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GETKEYOP,_FOOTER_);
  
  m_fpCall = GetKeyOp::eCall;
  m_sKey       = ""_Stz;
  m_iKeyCol    = C_UNKNOWN;
  m_iColVec    = C_UNKNOWN;
  
  m_bKeyIsLeft = true;
  
  _checkInitialised_(CN_GETKEYOP);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_GETKEYOP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
GetKeyOp::~GetKeyOp(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GETKEYOP,_FOOTER_);
  
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse & new                                                                */
/*----------------------------------------------------------------------------*/
Op* GetKeyOp::pNew(OpSpec* pOpSpec, Pipeline* pPipeline)
{
  
  GetKeyOp* pGetKeyOp = new GetKeyOp(pOpSpec,pPipeline);
  
  pGetKeyOp->m_sKey = pGetKeyOp->m_pOpSpec->pEnameSet()->szValue((i8)0);
  
  return pGetKeyOp;
}

/*----------------------------------------------------------------------------*/
/* call loop                                                                  */
/*----------------------------------------------------------------------------*/
eOR GetKeyOp::eCall(void* aThisOp)
{
  eOR eCallAgain = ((GetKeyOp*)aThisOp)->eOpLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/* call drain loop                                                            */
/*----------------------------------------------------------------------------*/
eOR GetKeyOp::eDrain(void* aThisOp)
{
  eOR eCallAgain = ((GetKeyOp*)aThisOp)->eDrainLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* op loop                                                                    */
/*----------------------------------------------------------------------------*/
eOR GetKeyOp::eOpLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GETKEYOP,_FOOTER_);
  
  for (;;) {
    
    Operand* pOperand = pPopQueue();
    
    if (! pOperand) {
      ASSERT(bIsLocked());
      return OR_BIDE;
    }
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    switch (pOperand->eClass()) {
        
      case CN_ENDMARKER: {
        if (--m_nLeftEndMarkers == 0) {
          vPipeOut(pOperand);
          goto L_EXIT;
        }
        else
          delete (EndMarker*)pOperand;
        break;
      }
        
      // -- -- -- -- -- --
     
      case CN_CRWBUFFER: {
        CrwBuffer* pCrwBuffer = _pCrwBuffer(pOperand) ;
        SrtBuffer* pSrtBuffer = pCrwBuffer->pToSrtBuffer(m_iKeyCol);
        
        while (pSrtBuffer) {
          SrtBuffer* pPipeOut = pSrtBuffer;
          pSrtBuffer = pSrtBuffer->pNextBuffer();
          vPipeOut(pPipeOut);
        }
        break;
      }
        
      // -- -- -- -- -- --
        
      case CN_ENTBUFFER: {
        RASSERT(-1 != m_iKeyCol);
        EntBuffer* pEntBuffer = _pEntBuffer(pOperand) ;
        SrtBuffer* pSrtBuffer = pEntBuffer->pToSrtBuffer(m_iKeyCol);
        
        while (pSrtBuffer) {
          SrtBuffer* pPipeOut = pSrtBuffer;
          pSrtBuffer = pSrtBuffer->pNextBuffer();
          vPipeOut(pPipeOut);
        }
        
        break;
      }
      
      // -- -- -- -- -- --
        
      case CN_IRWBUFFER: {
        RASSERT(0);
        break;
      }
        
      // -- -- -- -- -- --
        
      case CN_SRTBUFFER: {
        RASSERT(-1 != m_iKeyCol);
        SrtBuffer* pSrtBuffer = _pSrtBuffer(pOperand) ;
        pSrtBuffer->vGetKeys(m_pStreamDef, m_iKeyCol, m_iColVec);

        while (pSrtBuffer) {
          SrtBuffer* pPipeOut = pSrtBuffer;
          pSrtBuffer = pSrtBuffer->pNextBuffer();
          vPipeOut(pPipeOut);
        }
        break;
      }
        
      // -- -- -- -- -- --
        
      case CN_STREAMDEF: {
        StreamDef* pStreamDef = _pStreamDef(pOperand) ;
        m_pStreamDef = new StreamDef(pStreamDef);
     
        // figure out the key column
        m_iKeyCol = m_pStreamDef->iFindColumn(m_sKey.azChar(), m_iColVec);

        // we should have the key column now
        RASSERT(m_iKeyCol != -1);
        vPipeOut(pOperand);
        
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
eOR GetKeyOp::eDrainLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GETKEYOP,_FOOTER_);
  
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

__aFooterFunction(GetKeyOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
