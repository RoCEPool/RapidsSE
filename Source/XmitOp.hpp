/*============================================================================*/
/* XmitOp.hpp -- Transmit                                                     */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  class XmitOp : public Op
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
      void* operator new(size_t);
      void  operator delete(void*);
      
      XmitOp(OpSpec* pOpSpec, Digest* pDigest, OpDefn* aOpDefn);
      ~XmitOp(void) noexcept(false);
      
      static Op* pNew(OpSpec* pOpSpec, Digest* pDigest, OpDefn* aOpDefn,
                      TokenVec* pTknVec, i8& i_Tkn_);
      
      static eOR eCall(void* aThis);
  
  private:
    eOR eXmitLoop(void);
    Footer* aFooter(void);
  };

  XmitOp* _pXmitOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
