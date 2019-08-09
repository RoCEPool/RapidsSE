/*============================================================================*/
/* ToDataOp.hpp -- To Data Operator                                           */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  class ToDataOp : public Op
  {
  private:
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    ToDataOp(Stz* szOpLabel, Digest* pDigest, OpDefn* aOpDefn, bool bAppR);
   ~ToDataOp(void) noexcept(false);
  
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

  ToDataOp* _pToDataOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
