/*============================================================================*/
/* PtrVal.hpp - pointer value                                                 */
/*============================================================================*/
#pragma once

namespace Values
{
  class PtrVal : public Value
  {
  private:
    union {
      Object* m_pObj;
      Value*  m_aValue;
    };

  public:
    void* operator new(size_t);
    
    PtrVal(void);
    
    void operator =(Object* pObject);
    void operator =(Value* aValue);

    i8      nSize(void) const;
    Object* pObj(void) const;
    i8      iAsInt(void);
  
    void vToString(Stz* aStz);
  };
  
  PtrVal* _aPtrVal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
