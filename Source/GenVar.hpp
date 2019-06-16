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
      
      Header m_xHeader;  // header
      union {
        i8      m_nSize; // size of str
        i8      m_iInt;  // value contents as iInt
        Object* m_pObj;  // value contents as pPtr
        
        _Dtm    m_xDtm;  // datum
      } ;
      
      char m_caStr[C_MAX_VCH];
      
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
    IntVal* aNestedIntVal(void);
    PtrVal* aNestedPtrVal(void);
  
    Footer* aFooter(void);
  };

  GenVar* _pGenVar(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
