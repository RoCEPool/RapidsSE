/*============================================================================*/
/* ToFileOp.hpp -- write to file operator                                     */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  class ToFileOp : public Op
  {
  private:
    
    File  m_oOutFile;
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    ToFileOp(OpSpec* pOpSpec, Pipeline* pPipeline);
   ~ToFileOp(void) noexcept(false);
  
    static Op* pNew(OpSpec* pOpSpec, Pipeline* pPipeline);
    static eOR eCall(void* aThis);
    static eOR eDrain(void* aThis);

    eOR eOpLoop(void);
    eOR eDrainLoop(void);

  private:
    Footer* aFooter(void);
  };

  ToFileOp* _pToFileOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
