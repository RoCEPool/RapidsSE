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
  
    InflateOp(Stz* szOpLabel, Digest* pDigest, OpDefn* aOpDefn, bool bAppR, OpSpec* pOpSpec=nullptr);
   ~InflateOp(void) noexcept(false);
  
    static Op* pNew(Stz* szLabel,
                    Digest* pDigest,
                    OpDefn* aOpDefn, bool bAppR, OpSpec* pOpSpec,
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
