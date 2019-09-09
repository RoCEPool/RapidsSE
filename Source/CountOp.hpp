/*============================================================================*/
/* CountOp.hpp -- Output                                                     */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  class CountOp : public Op
  {
  private:
    
    i8 m_nCount;
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    CountOp(OpSpec* pOpSpec, Stz* szOpLabel, Digest* pDigest, OpDefn* aOpDefn, bool bAppR);
   ~CountOp(void) noexcept(false);
  
    static Op* pNew(Stz* szLabel, Digest* pDigest,
                    OpDefn* aOpDefn, bool bAppR, OpSpec* pOpSpec,
                    TokenVec* pTknVec, i8& i_Tkn_, i8 i_Slot);
    static eOR eCall(void* aThis);
    static eOR eDrain(void* aThis);

    eOR eOpLoop(void);
    eOR eDrainLoop(void);

  private:
    Footer* aFooter(void);
  };

  CountOp* _pCountOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
