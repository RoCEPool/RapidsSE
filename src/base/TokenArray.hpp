/*============================================================================*/
/* PlanNode.hpp - A base class of a generic node in the plan graph            */
/*============================================================================*/
#pragma once

#include "SegmentedArrayNL_t.hpp"
#include "Token.hpp"

namespace BASE {
  typedef SegmentedArrayNL_t<CN_TOKENARRAY, Token, (C_SIZE_TINY_OBJECT - (6 * 8)) / sizeof(Token)> TokenArray;
  typedef SegmentedArrayIter<CN_TOKENARRAY, Token, (C_SIZE_TINY_OBJECT - (6 * 8)) / sizeof(Token)> TokenArrayIter;
}