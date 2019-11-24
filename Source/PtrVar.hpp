/*============================================================================*/
/* PtrVar.hpp - pointer value                                                 */
/*============================================================================*/
#pragma once

namespace SenVM
{
  class PtrVar : public SenVar
  {
  private:
    Object* m_pObj;
 
  public:
    void* operator new(size_t);
  
    void    operator =(Object* pObj);
    Object* pObj(void);

    i8      iAsInt(void);
    i8      nStackBytes(void);
    void    vToString(Stz* szStz);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
