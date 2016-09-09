/*============================================================================*/
/* SEitfCursor.cpp - cursor over SEitf list                                   */
/*============================================================================*/

#include "Interface.h"

extern Heap*        gpSmallHeap;
extern SEitfList*   gpSEitfList;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* SEitfCursor::operator new(size_t iSize, SEitfCursor* pSEitfCursor)
{
  ASSERT(iSize);
  return pSEitfCursor;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void SEitfCursor::operator delete(void* pSEitfCursorAddress)
{
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SEitfCursor::SEitfCursor(List_t<SEitf>* pList)
: Object(CN_SEITFCURSOR),
  ListCursor_t<SEitf>(pList)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITFCURSOR,_FOOTER_);
  
  Memory::vCheckInitialised(this,sizeof(SEitfCursor));
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SEITFCURSOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SEitfCursor::~SEitfCursor(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITFCURSOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SEitfCursor::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITFCURSOR);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/