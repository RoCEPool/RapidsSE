/*============================================================================*/
/* HashCursor.hpp -- HashCursor                                               */
/*============================================================================*/
#pragma once

namespace Structures
{
  static const i8 C_HASH_DEPTH = 8;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class HashCursor
  {
  public:
    HashMap*    m_pHashMap;

    HashBuffer* m_pHashBuffer;
    i8          m_i_HashEntry;
  
    i8          m_ia_HashEntry[C_HASH_DEPTH];
    HashBuffer* m_paHashBuffer[C_HASH_DEPTH];
    i8          m_i_Stack;

    HashBuffer* m_pDupeBuffer;
    HashEntry*  m_pDupeEntry;

  public:
    HashCursor(HashMap* pHashMap);
    ~HashCursor(void) noexcept(false);
  };
  
  HashCursor* _pHashCursor(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
