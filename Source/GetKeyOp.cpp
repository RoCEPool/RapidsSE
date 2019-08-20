/*============================================================================*/
/* GetKeyOp.cpp -- Output                                                     */
/*============================================================================*/

#include "DataOperators.h"

OpDesc __Op_GetKeyOp (
   CN_GETKEYOP,         // class number
   OK_SIMPLE,           // op kind
   "GetKey"_Stz,        // first word of prompt command
   "Gky"_Stz,           // prefix
   true,                // not partitioned
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
  m_sKey       = ""_Stz;
  m_iKeyCol    = -1;
  
  m_pRightStreamDef = nullptr;
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
  
  if (m_pRightStreamDef)
    delete m_pRightStreamDef;
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
  
  GetKeyOp* pGetKeyOp = new GetKeyOp(szLabel,pDigest,aOpDefn,bAppR);
  
  pGetKeyOp->m_sKey = pTknVec->pGet(i_Tkn_++)->szText();
  pGetKeyOp->m_nSlot = i_Slot;
  
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
     
      case CN_CRWBUFFER: {
        CrwBuffer* pCrwBuffer = _pCrwBuffer(pOperand) ;
        SrtBuffer* pSrtBuffer = pCrwBuffer->pToSrtBuffer(m_iKeyCol);
        vPipeOut(pSrtBuffer);
        break;
      }
        
      case CN_ENTBUFFER: {
        RASSERT(-1 != m_iKeyCol);
        EntBuffer* pEntBuffer = _pEntBuffer(pOperand) ;
        //pEntBuffer->vShow();
        SrtBuffer* pSrtBuffer = pEntBuffer->pToSrtBuffer(m_iKeyCol);
        vPipeOut(pSrtBuffer);
        break;
      }
       
      case CN_REFBUFFER: {
        RASSERT(-1 != m_iKeyCol);
        RefBuffer* pRefBuffer = _pRefBuffer(pOperand) ;
        SrtBuffer* pSrtBuffer = pRefBuffer->pToSrtBuffer(m_iKeyCol, m_bKeyIsLeft,
                                                         m_pStreamDef->m_pColumnVec,
                                                         m_pRightStreamDef->m_pColumnVec);
        //pSrtBuffer->vShow(m_pStreamDef->m_pColumnVec, m_pRightStreamDef->m_pColumnVec);
        SrtBuffer* pBuffer = pSrtBuffer;
        while (pBuffer) {
          SrtBuffer* pPipeOut = pBuffer;
          pBuffer = pBuffer->pNextBuffer();
          pPipeOut->vNextBuffer(nullptr);
          vPipeOut(pPipeOut);
        }
        delete pRefBuffer;
        break;
      }
        
      case CN_STREAMDEF: {
        StreamDef* pStreamDef = _pStreamDef(pOperand) ;
        if ( ! m_pStreamDef )
          m_pStreamDef = new StreamDef(pStreamDef);
        else {
          RASSERT ( ! m_pRightStreamDef );
          m_pRightStreamDef = new StreamDef(pStreamDef);
        }
        
        // figure out the key idx
        i8 nColumns = m_pStreamDef->m_pColumnVec->nItems();
        for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
          Column* pCol = m_pStreamDef->m_pColumnVec->pGet(i_Col);
          if (StrLib::bEqualTo(m_sKey.aChar(),pCol->sColumnName().aChar(),C_NO_CASE)) {
            m_iKeyCol = i_Col;
            break;
          }
        }
        if (m_iKeyCol == -1) {
          // this must be the left streamDef coming from a join
          // do we have a right def ?
          if (m_pRightStreamDef) {
            // figure out the key idx
            i8 nColumns = m_pStreamDef->m_pColumnVec->nItems();
            for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
              Column* pCol = m_pRightStreamDef->m_pColumnVec->pGet(i_Col);
              if (StrLib::bEqualTo(m_sKey.aChar(),pCol->sColumnName().aChar(),C_NO_CASE)) {
                m_iKeyCol = i_Col;
                m_bKeyIsLeft = false;
                break;
              }
            }
          }
        }
        
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
