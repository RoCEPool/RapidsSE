/*============================================================================*/
/* Op_ToFilter.cpp -- To Filter Op's operator                                 */
/*============================================================================*/

#include "CompilerOperators.h"

OpDesc __Op_ToFilter (
   CN_OP_TOFILTER,      // class number
   OK_SIMPLE,           // op kind
   "ToFilter"_Stz,      // first word of prompt command
   "ToFilter"_Stz,      // prefix
   false,               // not partitioned
   Op_ToFilter::pNew,   // function which parses the command and new's an op
   Op_ToFilter::eCall,  // function which calls the op loop
   Op_ToFilter::eDrain, // function which calls the drain loop
   
   "<help>"_Stz
);

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Op_ToFilter* CompilerOperators::_pOp_ToFilter(void* aAddr)
{
  Op_ToFilter* pToDataOp = reinterpret_cast<Op_ToFilter*>(aAddr);
  
  return pToDataOp;
}

/*----------------------------------------------------------------------------*/
/* new                                                                        */
/*----------------------------------------------------------------------------*/
void* Op_ToFilter::operator new(size_t)
{
  Op_ToFilter* pToDataOp =
    static_cast<Op_ToFilter*>(HeapMem::pAllocate(CN_OP_TOFILTER));
  
  return pToDataOp;
}

/*----------------------------------------------------------------------------*/
/* delete                                                                     */
/*----------------------------------------------------------------------------*/
void Op_ToFilter::operator delete(void* aAddress)
{
  Op_ToFilter* pToDataOp = static_cast<Op_ToFilter*>(aAddress);
  
  HeapMem::vFree(pToDataOp);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Op_ToFilter::Op_ToFilter(Stz* szOpLabel, Digest* pDigest, OpDefn* aOpDefn, bool bAppR)
: Op(CN_OP_TOFILTER,szOpLabel,pDigest,aOpDefn,bAppR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP_TOFILTER,_FOOTER_);
  
  m_fpCall = Op_ToFilter::eCall;
  
  m_pFilter_From    = nullptr;
  m_pFilter_GroupBy = nullptr;
  m_pFilter_Join    = nullptr;
  m_pFilter_Limit   = nullptr;
  m_pFilter_OrderBy = nullptr;
  m_pFilter_Select  = nullptr;
  m_pFilter_Where   = nullptr;
  
  _checkInitialised_(CN_OP_TOFILTER);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_OP_TOFILTER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Op_ToFilter::~Op_ToFilter(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP_TOFILTER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse & new                                                                */
/*----------------------------------------------------------------------------*/
Op* Op_ToFilter::pNew(Stz* szLabel,
                   Digest* pDigest, OpDefn* aOpDefn, bool bAppR,
                   TokenVec* pTknVec, i8& i_Tkn_, i8 i_Slot)
{
  i_Tkn_++;
  ASSERT(i_Slot == -1);
  
  Op_ToFilter* pToDataOp = new Op_ToFilter(szLabel,pDigest,aOpDefn,bAppR);
  
  return pToDataOp;
}

/*----------------------------------------------------------------------------*/
/* call loop                                                                  */
/*----------------------------------------------------------------------------*/
eOR Op_ToFilter::eCall(void* aThisOp)
{
  eOR eCallAgain = ((Op_ToFilter*)aThisOp)->eOpLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/* call drain loop                                                            */
/*----------------------------------------------------------------------------*/
eOR Op_ToFilter::eDrain(void* aThisOp)
{
  eOR eCallAgain = ((Op_ToFilter*)aThisOp)->eDrainLoop();
  
  return eCallAgain;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* op loop                                                                    */
/*----------------------------------------------------------------------------*/
eOR Op_ToFilter::eOpLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP_TOFILTER,_FOOTER_);
  
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
        
      case CN_CLAUSE_FROM:
        m_pFilter_From = new Filter_From(((Clause*)pOperand)->pTokenVec());
        vPipeOut(m_pFilter_From);
        break;

      case CN_CLAUSE_GROUPBY:
        m_pFilter_GroupBy = new Filter_GroupBy(((Clause*)pOperand)->pTokenVec());
        vPipeOut(m_pFilter_From);
        break;
        
      case CN_CLAUSE_JOIN:
        m_pFilter_Join = new Filter_Join(((Clause*)pOperand)->pTokenVec());
        vPipeOut(m_pFilter_Join);
        break;
        
      case CN_CLAUSE_LIMIT:
        m_pFilter_Limit = new Filter_Limit(((Clause*)pOperand)->pTokenVec());
        vPipeOut(m_pFilter_Limit);
        break;
        
      case CN_CLAUSE_ORDERBY:
        m_pFilter_OrderBy = new Filter_OrderBy(((Clause*)pOperand)->pTokenVec());
        vPipeOut(m_pFilter_OrderBy);
        break;
        
      case CN_CLAUSE_SELECT:
        m_pFilter_Select = new Filter_Select(((Clause*)pOperand)->pTokenVec());
        vPipeOut(m_pFilter_Select);
        break;
        
     case CN_CLAUSE_WHERE:
        m_pFilter_Where = new Filter_Where(((Clause*)pOperand)->pTokenVec());
        vPipeOut(m_pFilter_From);
        break;
        
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
eOR Op_ToFilter::eDrainLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_OP_TOFILTER,_FOOTER_);
  
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
        break;
        
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
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Op_ToFilter);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
