/*============================================================================*/
/* XmitOp.hpp -- Transmit                                                     */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  class XmitOp : public Op
  {
  private:
  
    bool m_bCancelMarker;  // has the cancel marker been already sent ?
    
    // must be last
    Footer m_xFooter;

  public:
      void* operator new(size_t);
      void  operator delete(void*);
      
      XmitOp(OpSpec* pOpSpec, Pipeline* pPipeline);
      ~XmitOp(void) noexcept(false);
      
      static Op* pNew(OpSpec* pOpSpec, Pipeline* pPipeline);
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
