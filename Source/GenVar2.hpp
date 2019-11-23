/*============================================================================*/
/* GenVar2.hpp - Generic Variable  (which contains a nested value)            */
/*============================================================================*/
#pragma once

namespace SenVM
{
  class GenVar2 : public Object
  {
  private:
    struct {
      Header       m_xHeader;
      SingleWord   m_wWord;
      char         m_caStr[C_MAX_VCH2];
    } xNestedValue;
  
//    union {
//      FipVal  m_xFipVal;
//      IntVal  m_xIntVal;
//      PtrVal  m_xPtrVal;
//      s48K    m_sStrVal;
//    } uAllVal;
  
    // must be last
    Footer m_xFooter;
    
    public:
    void* operator new(size_t);
  
    GenVar2(void);
   ~GenVar2(void) noexcept(false);
  
    void operator =(Cell* aFipPtr);
    void operator =(i8 iInt);
    void operator =(Object* pObject);
    void operator =(Stz* szStz);

    Cell*   aFipPtr(void);
    i8      iInt(void);
    bool    bIsInt(void);
    StkVal* aNestedValue(void);
    Object* pObject(void);
    i8      nSize(void);
    Stz*    szStz(void);
  
  private:
    Footer* aFooter(void);
  };

  GenVar2* _pGenVar2(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
