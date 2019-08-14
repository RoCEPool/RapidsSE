/*============================================================================*/
/* GetKeyOp.cpp -- Output                                                     */
/*============================================================================*/

#include "DataOperators.h"

OpDesc __Op_GetKeyOp (
   CN_GETKEYOP,         // class number
   OK_SIMPLE,           // op kind
   "GetKey"_Stz,        // first word of prompt command
   "gky"_Stz,           // prefix
   false,               // not partitioned
   GetKeyOp::pNew,      // function which parses the command and new's an op
   GetKeyOp::eCall,     // function which calls the op loop
   GetKeyOp::eDrain,    // function which calls the drain loop
   
   "<help>"_Stz
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
GetKeyOp::GetKeyOp(Stz* szOpLabel, Digest* pDigest, OpDefn* aOpDefn, bool bAppR)
: Op(CN_GETKEYOP,szOpLabel,pDigest,aOpDefn,bAppR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_GETKEYOP,_FOOTER_);
  
  m_fpCall = GetKeyOp::eCall;
  m_pStreamDef = nullptr;
  
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
Op* GetKeyOp::pNew(Stz* szLabel,
                   Digest* pDigest, OpDefn* aOpDefn, bool bAppR,
                   TokenVec* pTknVec, i8& i_Tkn_, i8 i_Slot)
{
  i_Tkn_++;
  ASSERT(i_Slot == -1);
  
  GetKeyOp* pGetKeyOp = new GetKeyOp(szLabel,pDigest,aOpDefn,bAppR);
  
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
        
      case CN_ENDMARKER:
        if (--m_nLeftEndMarkers == 0) {
          vPipeOut(pOperand);
          goto L_EXIT;
        }
        else
          delete (EndMarker*)pOperand;
        break;
        
      // -- -- -- -- -- --
        
      case CN_ENTBUFFER: {
        EntBuffer* pEntBuffer = _pEntBuffer(pOperand) ;
        pEntBuffer->vShow();
        SrtBuffer* pSrtBuffer = pEntBuffer->pToSrtBuffer();
        vPipeOut(pSrtBuffer);
        break;
      }
        
      case CN_STREAMDEF: {
        StreamDef* pStreamDef = _pStreamDef(pOperand) ;
        m_pStreamDef = new StreamDef(pStreamDef);
        vPipeOut(pOperand);
        break;
      }
        
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
