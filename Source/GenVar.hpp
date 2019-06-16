/*============================================================================*/
/* GenVar.hpp - Generic Variable                                              */
/*============================================================================*/
#pragma once

namespace Forth
{
  class GenVar : public Object
  {
  private:
    struct {
      
      Header m_xHeader;
      _Dtm   m_xDtm;
      char   m_caStr[C_MAX_VCH];
      
    } xValue;
  
    // must be last
    Footer m_xFooter;
    
    public:
    void* operator new(size_t);
  
    GenVar(void);
   ~GenVar(void) noexcept(false);
  
    void operator =(d8 dFlt);
    void operator =(i8 iInt);
    void operator =(Object* pObject);
    void operator =(Stz* szStr);

    i8      iInt(void);
    Stz*    szStz(void);

    Value*  aNestedValue(void);
  
  
  private:
    PtrVal* aNestedPtrVal(void);
  
    Footer* aFooter(void);
  };

  GenVar* _pGenVar(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
