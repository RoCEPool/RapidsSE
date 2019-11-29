/*============================================================================*/
/* AnyVal.hpp -- Base Class for a Self-Describing Values on Stack             */
/*============================================================================*/
#pragma once

namespace SenVM
{
  class AnyVal : public _Header
  {
  protected:
    union {
      Cell*   m_aFip;
      i8      m_iInt;
      Object* m_pObj;
      
      i8      m_nBytesFollowing;
    };
  
  public:
    void* operator new(size_t);
  
    void vCopy(AnyVal* aSrcVal, i8 nBytes=0);
  
    i8      iAsInt(void);
    char*   azChar(void);
    Cell*   aFip(void);
    void    vFip(Cell* aFip);
    void    vInspect(void);
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
  
  /*----------------------------------------------------------------------------*/
  /* safe cast                                                                  */
  /*----------------------------------------------------------------------------*/
  inline AnyVal* _aAnyVal(void* aAddr)
  {
    AnyVal* aAnyVal = reinterpret_cast<AnyVal*>(aAddr);
  
    return aAnyVal;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
