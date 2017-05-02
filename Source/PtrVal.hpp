/*============================================================================*/
/* PtrVal.hpp - pointer value                                                 */
/*============================================================================*/
#pragma once

namespace Values
{
  class PtrVal : public Value
  {
  private:
    Object* m_pPtr;
        
  public:
    void* operator new(size_t);
    
    PtrVal(void);
    
    void operator =(Object* pObject);
    
    i8      nSize(void) const;
    Object* pPtr(void) const;
    i8      iAsInt(void);
  
    void vToString(Str* aWorkspace);
  };
  
  PtrVal* _pPtrVal(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
