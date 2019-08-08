/*============================================================================*/
/* LimitOp.hpp -- Limit                                                       */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class LimitOp : public Op
  {
  private:
  
    i8 m_nLimit;    // what is the limit?
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    LimitOp(Stz* szOpLabel, Digest* pDigest, OpDefn* aOpDefn, bool bAppR);
   ~LimitOp(void) noexcept(false);
  
    static Op* pNew(Stz* szLabel,
                    Digest* pDigest,  OpDefn* aOpDefn, bool bAppR,
                    TokenVec* pTknVec, i8& i_Tkn_, i8 i_Slot);
    static eOR eCall(void* aThis);
    static eOR eDrain(void* aThis);

    eOR eOpLoop(void);
    eOR eDrainLoop(void);

  private:
    Footer* aFooter(void);
  };

  LimitOp* _pLimitOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
