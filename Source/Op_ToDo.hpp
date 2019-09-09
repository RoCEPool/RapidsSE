/*============================================================================*/
/* Op_ToDo.hpp -- To Do Operator                                              */
/*============================================================================*/
#pragma once

namespace CompilerOperators
{
  class Op_ToDo : public Op
  {
  private:
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Op_ToDo(OpSpec* pOpSpec, Digest* pDigest, Stz* szOpLabel, OpDefn* aOpDefn, bool bAppR);
   ~Op_ToDo(void) noexcept(false);
  
    static Op* pNew(OpSpec* pOpSpec, Stz* szLabel, Digest* pDigest,
                    OpDefn* aOpDefn, bool bAppR,
                    TokenVec* pTknVec, i8& i_Tkn_, i8 i_Slot);
    static eOR eCall(void* aThis);
    static eOR eDrain(void* aThis);

    eOR eOpLoop(void);
    eOR eDrainLoop(void);

  private:
    void vPromptHandOff(Cmd_Select* pCmd_Select);
    Footer* aFooter(void);
  };

  Op_ToDo* _pOp_ToDo(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
