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
    GenVar(StkVal*);
   ~GenVar(void) noexcept(false);
  
    // auto conversion
    operator AnyVal*(void);
    operator StkVal*(void);

    void operator =(StkVal*);
  
    void operator =(Cell* aFip);
    void operator =(i8 iInt);
    void operator =(Object* pObj);
    void operator =(Stz* szStz);

    char*   azChar(void);
    void    vCopy(StkVal* aStkVal);
    StkVal* aEmbeddedStkVal(void);
    Cell*   aFip(void);
    i8      iInt(void);
    bool    bIsInt(void);
    Object* pObj(void);
    i8      nStackBytes(void);
    Stz*    szStz(void);
    i8      nStzSize(void);
    void    vToString(Stz* szStz);
    eCN     eValClass(void);

  private:
    Footer* aFooter(void);
  };

  GenVar* _pGenVar(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
