/*============================================================================*/
/* ZedOp.cpp -- Zed Sink                                                      */
/*============================================================================*/

#include "DataOperators.h"

OpDesc __Op_Zed (
  CN_ZEDOP,         // class number
  OK_SIMPLE,        // op kind
  "Zed"_Stz,        // first word of prompt command
  "Zed"_Stz,        // prefix
  false,            // not partitioned
  ZedOp::pNew,      // function which parses the command and new's an op
  ZedOp::eCall,     // function which calls the op loop
  nullptr,          // function which calls the drain loop
  
  "<help>"_Stz
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
ZedOp* DataOperators::_pZedOp(void* aAddr)
{
  ZedOp* pZedOp = reinterpret_cast<ZedOp*>(aAddr);
  
  return pZedOp;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* ZedOp::operator new(size_t)
{
  ZedOp* pZedOp =
    static_cast<ZedOp*>(HeapMem::pAllocate(CN_ZEDOP));
  
  return pZedOp;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void ZedOp::operator delete(void* aAddress)
{
  ZedOp* pZedOp = static_cast<ZedOp*>(aAddress);
  
  HeapMem::vFree(pZedOp);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ZedOp::ZedOp(Stz* szOpLabel, Digest* pDigest, OpDefn* aOpDefn, bool bAppR)
: Op(CN_ZEDOP,szOpLabel,pDigest,aOpDefn,bAppR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ZEDOP,_FOOTER_);
  
  _checkInitialised_(CN_ZEDOP);
  
  m_fpCall = ZedOp::eCall;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ZEDOP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ZedOp::~ZedOp(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ZEDOP,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse & new                                                                */
/*----------------------------------------------------------------------------*/
Op* ZedOp::pNew(Stz* szLabel,
                Digest* pDigest, OpDefn* aOpDefn, bool bAppR,
                TokenVec* pTknVec, i8& i_Tkn_, i8 i_Slot)
{
  i_Tkn_++;
  ASSERT(i_Slot == -1);
  
  ZedOp* pZedOp = new ZedOp(szLabel,pDigest,aOpDefn,bAppR);
  
  return pZedOp;
}

/*----------------------------------------------------------------------------*/
/* call loop                                                                  */
/*----------------------------------------------------------------------------*/
eOR ZedOp::eCall(void* aThisOp)
{
  eOR eCallAgain = ((ZedOp*)aThisOp)->eOpLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* op loop                                                                    */
/*----------------------------------------------------------------------------*/
eOR ZedOp::eOpLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ZEDOP,_FOOTER_);
  
  for (;;) {
    
    Operand* pOperand = pPopQueue();
    
    if (! pOperand) {
      ASSERT(bIsLocked());
      return OR_BIDE;
    }
        
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    
    switch (pOperand->eClass()) {
        
      case CN_ENDMARKER:
        delete (EndMarker*)pOperand;
        if (--m_nLeftEndMarkers == 0) {
          goto L_EXIT;
        }
        break;
        
      // -- -- -- -- -- --
  
      case CN_PAUSEMARKER:
        delete (PauseMarker*)pOperand;
        break;

      // -- -- -- -- -- --
        
      case CN_SRTBUFFER: {
        SrtBuffer* pSrtBuffer = _pSrtBuffer(pOperand);
        delete pSrtBuffer;
        break;
      }
       
      case CN_STREAMDEF: {
        StreamDef* pStreamDef = _pStreamDef(pOperand);
        ASSERT(pStreamDef->m_bTemp);
        delete pStreamDef;
        break;
      }
        
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

__aFooterFunction(ZedOp);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
