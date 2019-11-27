/*============================================================================*/
/* GenVar.hpp - Generic Variable  (which contains a nested value)             */
/*============================================================================*/
#pragma once

namespace SenVM
{
  class GenVar : public Object
  {
  private:

    union {
      
      StkVal  m_xStkVal;

      FipVal  m_xFipVal;
      IntVal  m_xIntVal;
      PtrVal  m_xPtrVal;
      StrVal  m_xStrVal;
      
      byte    m_yaSpace[C_48KB];

    } u;

    // must be last
    Footer m_xFooter;
    
    public:
    void* operator new(size_t);
  
    GenVar(void);
   ~GenVar(void) noexcept(false);
  
    void operator =(StkVal*);
  
    void operator =(Cell* aFip);
    void operator =(i8 iInt);
    void operator =(Object* pObj);
    void operator =(Stz* szStz);

    char*   azChar(void);
    StkVal* aEmbeddedStkVal(void);
    Cell*   aFip(void);
    i8      iInt(void);
    bool    bIsInt(void);
    Object* pObj(void);
    Stz*    szStz(void);
    i8      nStzSize(void);
    void    vToString(Stz* szStz);

  private:
    Footer* aFooter(void);
  };

  GenVar* _pGenVar(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
