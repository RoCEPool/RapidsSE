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
  
  /*----------------------------------------------------------------------------*/
  /* safe cast                                                                  */
  /*----------------------------------------------------------------------------*/
  inline PtrVar* _aPtrVar(void* aAddr)
  {
    PtrVar* aPtrVar = reinterpret_cast<PtrVar*>(aAddr);
  
    ASSERT( !aPtrVar || CN_PTRVAR == aPtrVar->eClass() );
  
    return aPtrVar;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
