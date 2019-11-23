/*============================================================================*/
/* PtrVar.hpp - pointer value                                                 */
/*============================================================================*/
#pragma once

namespace SenVM
{
  class PtrVar : public Object
  {
  private:
    Object* m_pObj;
    Footer  m_xFooter;

  public:
    void* operator new(size_t);
    
    PtrVar(void);
   ~PtrVar(void) noexcept(false);

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
