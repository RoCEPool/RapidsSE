/*============================================================================*/
/* TransformerTool.hpp --                                                       */
/*============================================================================*/
#pragma once

#include "Dissector.hpp"

#include "Bench.hpp"

namespace STORE
{
  class TransformerTool : public Object
  {
  protected:

    Bench*       m_pBench;
    Board*       m_pBoard;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    TransformerTool(eCN eClass, Bench* pBench);
   ~TransformerTool(void) noexcept(false);

  private:

  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
