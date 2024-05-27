/*============================================================================*/
/* PlanNode.hpp - A base class of a generic node in the plan graph            */
/*============================================================================*/
#pragma once

#include "SegmentedArrayNL_t.hpp"
#include "Token.hpp"

namespace BASE {
  typedef SegmentedArrayNL_t<CN_TOKENARRAY, Token, (C_SIZE_TINY_OBJECT - (6 * C_BYTES_PER_WORD)) / sizeof(Token)> TokenArray;
  typedef SegmentedArrayIter<CN_TOKENARRAY, Token, (C_SIZE_TINY_OBJECT - (6 * C_BYTES_PER_WORD)) / sizeof(Token)> TokenArrayIter;

  // Note: The '6 * C_BYTES_PER_WORD' in the above template parameter is calculated
  // from the size of the SegmentedArrayNL_t class without the array of objects:
  //  Object header (derived)       1 word
  //  i8 m_nItems                   1 word
  //  SegmentedArrayNL_t* m_pNext   1 word
  //  SegmentedArrayNL_t* m_pHead   1 word
  //  Footer                        2 words
  //  ------                        -------
  //  Total                         6 words
}