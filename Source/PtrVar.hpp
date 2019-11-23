/*============================================================================*/
/* PtrVar.hpp - pointer value                                                 */
/*============================================================================*/
#pragma once

namespace SenVM
{
  class PtrVar
  {
  private:
    _Header m_xHeader;
    Object* m_pObj;
 
  public:
    void* operator new(size_t);
  
    void operator =(Object* pObject);
 
    i8      iAsInt(void);
    Object* pObj(void);
    i8      nSize(void);
    void    vToString(Stz* szStz);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
