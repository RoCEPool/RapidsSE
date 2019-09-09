/*============================================================================*/
/* InflateOp.hpp -- Inflate                                                   */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  class InflateOp : public Op
  {
  private:
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    InflateOp(OpSpec* pOpSpec, Digest* pDigest, Stz* szOpLabel, OpDefn* aOpDefn, bool bAppR);
   ~InflateOp(void) noexcept(false);
  
    static Op* pNew(OpSpec* pOpSpec, Digest* pDigest, Stz* szLabel,
                    OpDefn* aOpDefn, bool bAppR,
                    TokenVec* pTknVec, i8& i_Tkn_, i8 i_Slot);
    static eOR eCall(void* aThis);
    static eOR eDrain(void* aThis);

    eOR eOpLoop(void);
    eOR eDrainLoop(void);

  private:
    Footer* aFooter(void);
  };

  InflateOp* _pInflateOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
