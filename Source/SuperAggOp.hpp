/*============================================================================*/
/* SuperAggOp.hpp -- Super Count                                            */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  
  class SuperAggOp : public Op
  {
  private:
  
    i8  m_nSum;
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    SuperAggOp(OpSpec* pOpSpec, Stz* szOpLabel, Digest* pDigest, OpDefn* aOpDefn, bool bAppR);
   ~SuperAggOp(void) noexcept(false);
  
    static Op* pNew(Stz* szLabel,
                    Digest* pDigest,
                    OpDefn* aOpDefn, bool bAppR, OpSpec* pOpSpec,
                    TokenVec* pTknVec, i8& i_Tkn_, i8 i_Slot);
    
    static eOR eCall(void* aThis);
    
  private:
    eOR        eSuperAggLoop(void);
  
    Footer* aFooter(void);
  };

  SuperAggOp* _pSuperAggOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
