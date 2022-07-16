/*============================================================================*/
/* TypeValueDscr.hpp -- Type + Value + Descriptor                             */
/*============================================================================*/
#pragma once

#include "TypeValue.hpp"

namespace BASE
{
  class LocalStr;
  class LocalVars;

//class Local         : public TypeValue
  class TypeValueDscr : public TypeValue
  {
  public:
    
    char       m_caArea_tbr[C_16KB];
  
    LocalVars* m_pLocalVars;
    i8         m_i_Local;

    b8         m_bLocalStr;
    union {
      LocalStr*        m_pLocalStr;
    };
  
  public:
    TypeValueDscr(void);
   ~TypeValueDscr(void) noexcept(false);
  
    char* azArea(void);

    i8    iAsInt(void);

    i8    iBcd(void);
    void  vBcd(i8 iBcd);
  
    i8    iBol(void);
    void  vBol(i8 iBol);

    char* azChar(void);
  
    void  vCopy(TypeValueDscr* aTypedCell, i8 nBytes=0);

    CodeCell* aFip(void);
    void  vFip(CodeCell* aFip);

    d8    dFlt(void);
    void  vFlt(d8 dVal);
  
    void  vInit(void);

    i8    iInt(void);
    void  vInt(i8 iVal);

    i8    iIntBol(void);
  
    void  vMagic(void);
  
    Object* pObj(void);
    void  vObj(Object* pObject);

    i8    nStackUsage(void);
  
    Stz*  szStz(void);
    void  vStz(Stz* szStz);
    i8    nStzSize(void);
    void  vStzSize(i8 nSize);
  
    void  vToStz(Stz* szStz);
  
    eCN   eType(void);
    void  vType(eCN eType);
  
    TypeValue* aTypeValue(void);
  };

  /*--------------------------------------------------------------------------*/
  /* safe cast                                                                */
  /*--------------------------------------------------------------------------*/
  inline TypeValueDscr* _aTypeValueDscr(void* aAddr)
  {
  TypeValueDscr* aTypeValueDscr = reinterpret_cast<TypeValueDscr*>(aAddr);

    return aTypeValueDscr;
  }

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
