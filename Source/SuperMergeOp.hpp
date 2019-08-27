/*============================================================================*/
/* SuperMergeOp.hpp -- Super Merge                                            */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  
  class SuperMergeOp : public Op
  {
  private:
  
    Operand* m_pOperand[C_OPERANDS];
    i8       m_iOperands;
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    SuperMergeOp(Stz* szOpLabel, Digest* pDigest, OpDefn* aOpDefn, bool bAppR);
   ~SuperMergeOp(void) noexcept(false);
  
    static Op* pNew(Stz* szLabel,
                    Digest* pDigest, OpDefn* aOpDefn, bool bAppR,
                    TokenVec* pTknVec, i8& i_Tkn_, i8 i_Slot);
    
    static eOR eCall(void* aThis);
    
  private:
    void vMergeDangling(void);
    eOR  eMergeLoop(void);
    void vSuperMergeOperands(Operand* pOperand1, Operand* pOperand2);
  
    Footer* aFooter(void);
  };

  SuperMergeOp* _pSuperMergeOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
