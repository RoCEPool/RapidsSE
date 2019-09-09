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
  
    CountOp(OpSpec* pOpSpec, Digest* pDigest, Stz* szOpLabel, OpDefn* aOpDefn, bool bAppR);
   ~CountOp(void) noexcept(false);
  
    static Op* pNew(OpSpec* pOpSpec, Stz* szLabel, Digest* pDigest,
                    OpDefn* aOpDefn, bool bAppR,
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
