/*============================================================================*/
/* Op_ToCommand.hpp -- To Command String operator                             */
/*============================================================================*/
#pragma once

namespace CompilerOperators
{
  class Op_ToCommand : public Op
  {
  private:
    s4K m_sCommand;
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Op_ToCommand(Stz* szOpLabel, Digest* pDigest, OpDefn* aOpDefn, bool bAppR);
   ~Op_ToCommand(void) noexcept(false);
  
    static Op* pNew(Stz* szLabel,
                    Digest* pDigest,  OpDefn* aOpDefn, bool bAppR,
                    TokenVec* pTknVec, i8& i_Tkn_, i8 i_Slot);
    static eOR eCall(void* aThis);
    static eOR eDrain(void* aThis);

    eOR eOpLoop(void);
    eOR eDrainLoop(void);

  private:
    void vAppendFilter(Filter* pFilter);
  
    Footer* aFooter(void);
  };

  Op_ToCommand* _pOp_ToCommand(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
