/*============================================================================*/
/* StkVal.hpp -- Base Class for a Self-Describing Values on Stack             */
/*============================================================================*/
#pragma once

namespace SenVM
{
  class StkVal : public _Header
  {
  protected:
   
  public:
    void* operator new(size_t);
  
    void vCopy(StkVal* aSrcVal, i8 nBytes=0);
  
    char*   azChar(void);
    Cell*   aFip(void);
    void    vFip(Cell* aFip);
    i8      iInt(void);
    void    vInt(i8 iInt);
    bool    bIsInt(void);
    Object* pObj(void);
    void    vObj(Object* pObj);
    Stz*    szStz(void);
    void    vStz(Stz* szStz);
    i8      nStzSize(void);

    i8   nStackBytes(void);
    void vToString(Stz* szString);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
