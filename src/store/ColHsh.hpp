/*============================================================================*/
/* ColHsh.hpp -- Column Mapping                                               */
/*============================================================================*/
#pragma once

#include "AbstractDef.hpp"
#include "Card_Hashing.hpp"

namespace STORE
{
  class ColHsh : public AbstractDef
  {
    static const i8 C_COLHSH = 128;

  public:

    i8 m_nCols;

    struct {
      i8   m_i_Col;
      eCT  m_eColumnType;

			s256  m_sHashOf;

    } m_axCols[C_COLHSH];

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    ColHsh(const char* azFile, i8 iLine);

   ~ColHsh(void) noexcept(false);

    void vAdd(i8 i_Col, eCT eColumnType, Stz* szHashOf);

    void vDump(i8 i_Tab=0);

    void vUpdate(Card_Hashing* p_hashing);

    void vValidate(void);

  protected:
    Footer* aFooter(void);
  };

  ColHsh* _pColHsh(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/