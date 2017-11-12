/*============================================================================*/
/* SEitfCursor.cpp - cursor over SEitf list                                   */
/*============================================================================*/

#include "Interface.h"

extern SEitfList*   gpSEitfList;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
SEitfCursor* Interface::_pSEitfCursor(void* aAddr)
{
  SEitfCursor* pSEitfCursor = reinterpret_cast<SEitfCursor*>(aAddr);
  
  RASSERT( !pSEitfCursor || CN_SEITFCURSOR == pSEitfCursor->eClass() );
  
  return pSEitfCursor;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* SEitfCursor::operator new(size_t iSize, SEitfCursor* pSEitfCursor)
{
  RASSERT(iSize);
  return pSEitfCursor;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void SEitfCursor::operator delete(void* pSEitfCursorAddress)
{
  RASSERT(pSEitfCursorAddress);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SEitfCursor::SEitfCursor(List_t<SEitf>* pList)
: Object(CN_SEITFCURSOR),
  ListCursor_t<SEitf>(pList)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITFCURSOR,_FOOTER_);
  
  _checkInitialised_(CN_SEITFCURSOR);
  
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
Footer* SEitfCursor::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITFCURSOR);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
