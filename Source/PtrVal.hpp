/*============================================================================*/
/* PtrVal.hpp - pointer value                                                 */
/*============================================================================*/
#pragma once

namespace Values
{
  class PtrVal : public Value
  {
  private:
    Object* m_pObj;

  public:
    void* operator new(size_t);
    
    PtrVal(void);
    
    void operator =(Object* pObject);
 
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
