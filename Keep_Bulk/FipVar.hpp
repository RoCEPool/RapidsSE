/*============================================================================*/
/* FipVar.hpp - Forth Instruction Pointer Sen Variable                        */
/*============================================================================*/
#pragma once

namespace SenVM
{
  class FipVar : public SenVar
  {
  private:
    Cell* m_aFip;

  public:
    void* operator new(size_t);
    
    void  operator =(Cell* aFip);
    Cell* aFip(void);
  
    i8    nStackBytes(void);
    void  vToString(Stz* szStz);
  };
  
  /*----------------------------------------------------------------------------*/
  /* safe cast                                                                  */
  /*----------------------------------------------------------------------------*/
  inline FipVar* _aFipVar(void* aAddr)
  {
    FipVar* aFipVar = reinterpret_cast<FipVar*>(aAddr);
  
    ASSERT( !aFipVar || CN_FIPVAR == aFipVar->eClass() );
  
    return aFipVar;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
