/*============================================================================*/
/* MergeOp.hpp -- Merge                                                       */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  const i8 C_OPERANDS = 32;
  
  class MergeOp : public Op
  {
  private:
  
    Operand* m_pOperand[C_OPERANDS];
    i8       m_iOperands;
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    MergeOp(OpSpec* pOpSpec, Digest* pDigest, Stz* szOpLabel, OpDefn* aOpDefn, bool bAppR);
   ~MergeOp(void) noexcept(false);
  
    static Op* pNew(OpSpec* pOpSpec, Stz* szLabel, Digest* pDigest,
                    OpDefn* aOpDefn, bool bAppR,
                    TokenVec* pTknVec, i8& i_Tkn_, i8 i_Slot);
    
    static eOR eCall(void* aThis);
    
  private:
    void vMergeDangling(void);
    eOR  eMergeLoop(void);
    void vMergeOperands(Operand* pOperand1, Operand* pOperand2);
  
    Footer* aFooter(void);
  };

  MergeOp* _pMergeOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
