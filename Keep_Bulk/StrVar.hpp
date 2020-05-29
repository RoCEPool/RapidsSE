/*============================================================================*/
/* StrVar.hpp - String Sen Variable                                           */
/*============================================================================*/
#pragma once

namespace SenVM
{
  class StrVar : public SenVar
  {
  private:
    i8   m_nBytesFollowing;
    char m_caStr[C_48K];

  public:
    void* operator new(size_t);
  
    void  operator =(Stz* szStz);
    char* azChar(void);
    Stz*  szStz(void);
  
    i8    nSize(void);
    i8    nStackBytes(void);
    void  vToString(Stz* szStz);
  };
  
  /*----------------------------------------------------------------------------*/
  /* safe cast                                                                  */
  /*----------------------------------------------------------------------------*/
  inline StrVar* _aStrVar(void* aAddr)
  {
    StrVar* aStrVar = reinterpret_cast<StrVar*>(aAddr);
  
    ASSERT( !aStrVar || CN_STRVAR == aStrVar->eClass() );
  
    return aStrVar;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
