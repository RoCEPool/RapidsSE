/*============================================================================*/
/* KeyMap.hpp -- Key Mapping                                                  */
/*============================================================================*/
#pragma once

#include "AbstractDef.hpp"

namespace STORE
{
  class KeyMap : public AbstractDef
  {
    static const i8 C_KEYMAP = 128;

  public:

    s32  m_sRefer;

    i8 m_nCols;

    struct {
      i8   m_i_EmittedCol;  // emitted column $n
      i8   m_i_Cell;        // cell (base 0) of payload/key
    } m_axCols[C_KEYMAP];

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    KeyMap(const char* azFile, i8 iLine);

   ~KeyMap(void) noexcept(false);

    void vAdd(i8 i_EmittedCol, i8 i_Cell);

    void vDump(i8 i_Tab=0);

    void vValidate(void);

  protected:
    Footer* aFooter(void);
  };

  KeyMap* _pKeyMap(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/