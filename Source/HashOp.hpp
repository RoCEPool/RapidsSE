/*============================================================================*/
/* HashOp.hpp -- Hash                                                         */
/*============================================================================*/
#pragma once

namespace DataOperators
{
  class HashOp : public Op
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    HashOp(Stz* szOpLabel, Pipeline* pPipeline);
   ~HashOp(void) noexcept(false);
  
  private:
    Footer* aFooter(void);
  };

  HashOp* _pHashOp(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
