/*============================================================================*/
/* Structures.h -- Structures namespace                                       */
/*============================================================================*/
#pragma once

#include "Persistence.h"

//------------------------------------------------------------------------------

namespace Structures
{
  void vCheckAssumptions(void);
}

//------------------------------------------------------------------------------

#include "ArrayNL_t.hpp"

#include "FreeItem.hpp"

#include "HashInsert.hpp"
#include "HashEntry.hpp"
#include "HashBuffer.hpp"
#include "HashMap.hpp"
#include "HashCursor.hpp"

#include "LatticeNL_t.hpp"
#include "Lexicon.hpp"
#include "Link_t.hpp"
#include "ListItem_t.hpp"
#include "List_t.hpp"
#include "ListNL_t__.hpp"
#include "ListNL_t.hpp"

#include "QueueBuffer.hpp"
#include "Queue.hpp"

#include "SkipNode.hpp"
#include "SkipListNL.hpp"

#include "Vector_t.hpp"
#include "VectorNL_t.hpp"

#include "Wilderness.hpp"

// -- -- -- -- -- -- -- --

typedef void (pARTFUN)(Object* pObject, SP& spObject);

#include "Article.hpp"
#include "MiniHeap_t.hpp"
#include "RowHeap_t.hpp"
#include "TrayNL_t.hpp"

// -- -- -- -- -- -- -- --

#include "SP_Article.hpp"


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
