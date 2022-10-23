/*============================================================================*/
/* Accessor.hpp --                                                            */
/*============================================================================*/
#pragma once

#include "TransformerTool.hpp"

namespace STORE
{
  class Accessor : public TransformerTool
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    bool A_bTable(Stz* szTableNm);
    bool A_bIndex(Stz* szIndexNm);
    bool A_bIndex(Stz* szTableNm, Stz* szIndexNm);

    bool A_bIndexListOfTable(void);
     void A_vStreamOfTable(void);
    void A_vPartitionOfTable(void);
    void A_vSizeOfTable(void);

    Accessor(eCN eClassNo, Bench* pBench);
   ~Accessor(void) noexcept(false);

  private:

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
