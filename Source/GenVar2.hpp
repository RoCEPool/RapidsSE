/*============================================================================*/
/* GenVar2.hpp - Generic Variable                                             */
/*============================================================================*/
#pragma once

namespace Forth
{
  class GenVar2 : public Object
  {
  private:
    union {
    
      Wsp_t<C_WSP_GENVAR> m_xWsp; // workspace
    
      ValSee m_xNestedValue;      // inspector
    } ;

    // must be last
    Footer m_xFooter;
  
    public:
      void* operator new(size_t);
  
      GenVar2(void);
     ~GenVar2(void) noexcept(false);
  
      void operator =(d8 dFlt);
      void operator =(i8 iInt);
      void operator =(Object* pObject);
      void operator =(Stz* szStr);
  
      i8      iNestedAsInt(void);
      IntVal* aNestedIntVal(void);
      PtrVal* aNestedPtrVal(void);
      StrVal* aNestedStrVal(void);
      Value*  aNestedValue(void);
  
      void    vGetString(Stz* aStz);
      char*   aWspArea(void);
  
      Footer* aFooter(void);
    };

    GenVar2* _pGenVar2(void* aAddr);
  }

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
