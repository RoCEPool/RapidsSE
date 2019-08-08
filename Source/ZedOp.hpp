/*============================================================================*/
/* ZedOp.hpp -- Zed                                                           */
/*============================================================================*/
#pragma once

namespace PushDown
{
  class ZedOp : public Op
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    ZedOp(Stz* szOpLabel, Digest* pDigest, OpDefn* aOpDefn, bool bAppR);
   ~ZedOp(void) noexcept(false);
  
    static Op* pNew(Stz* szLabel,
                    Digest* pDigest, OpDefn* aOpDefn, bool bAppR,
                    TokenVec* pTknVec, i8& i_Tkn_, i8 i_Slot);
    static eOR eCall(void* aThis);
  
    eOR eOpLoop(void);
  
  private:
    Footer* aFooter(void);
  };

  ZedOp* _pZedOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
