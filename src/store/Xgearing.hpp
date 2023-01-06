/*============================================================================*/
/* Xgearing.hpp --                                                            */
/*============================================================================*/
#pragma once

#include "CardIter.hpp"
#include "Bench.hpp"

#include "PermGen.hpp"

#include "Table.hpp"
#include "PtnDef.hpp"

namespace STORE
{
  class Xgearing : public CardIter
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Xgearing(eCN eClass, Bench* pBench);
   ~Xgearing(void) noexcept(false);

    void vTransform(const char* azFunc, Stz* szXdesc);
    i8   iTransformDepth(void);

    void vUsingBoard(eBK eBoard, i8 i_Board);

    Stz* szXdesc(void);
    void vXdesc(Stz* szDescription);
    void vXform(Stz* szTransformation);
    void vXhand(const char* asXhand);

  protected:
    Stz* szT(eCK eMember);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
