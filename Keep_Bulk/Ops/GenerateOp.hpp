/*============================================================================*/
/* GenerateOp.hpp -- Generate                                                 */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  class GenerateOp : public Op
  {
  private:
    i8         m_iBuffers;
    IntBuffer* m_pRootBuffer;
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    GenerateOp(Stz* szOpLabel, Pipeline* pPipeline, i8 iBuffers);
   ~GenerateOp(void) noexcept(false);

    static eOR eCallLoop(void* aThis);
    eOR eGenerateLoop(void);
    
  private:
    Footer* aFooter(void);
  };

  GenerateOp* _pGenerateOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
