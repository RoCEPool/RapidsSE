/*============================================================================*/
/* HashCursor.hpp -- HashCursor                                               */
/*============================================================================*/
#pragma once

namespace Structures
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class HashCursor
  {
  public:
    HashMap*    m_pHashMap;

    HashBuffer* m_pHashBuffer;
    i8          m_i_HashEntry;

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
