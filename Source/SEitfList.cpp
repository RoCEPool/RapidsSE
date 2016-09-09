/*============================================================================*/
/* SEitfList.cpp - SEitf List                                                 */
/*============================================================================*/

#include "Interface.h"

extern Heap*        gpSmallHeap;
extern SEitfList*   gpSEitfList;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* SEitfList::operator new(size_t iSize, SEitfList* pSEitfList)
{
  ASSERT(iSize);
  return pSEitfList;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void SEitfList::operator delete(void* pBufferAddress)
{
  vFatal("fatal: SEitfList class cannot be deleted");
  
  ASSERT(0);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SEitfList::SEitfList(void)
: Object(CN_SEITFLIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITFLIST,_FOOTER_);
  
  ASSERT( ! gpSEitfList );
  
  gpSEitfList = this;
  
  Memory::vCheckInitialised(this,sizeof(SEitfList));
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SEITFLIST,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SEitfList::~SEitfList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITFLIST,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* allocate a seitf                                                           */
/*----------------------------------------------------------------------------*/
SEitf* SEitfList::pAllocSEitf(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITFLIST,_FOOTER_);

  SEitf* pSEitf = reinterpret_cast<SEitf*>(gpSmallHeap->pAllocObject(CN_SEITFLIST));
  
  vAppend(pSEitf);
  
  return pSEitf;
}

/*----------------------------------------------------------------------------*/
/* free a seitf                                                               */
/*----------------------------------------------------------------------------*/
void SEitfList::vFreeSEitf(SEitf* pSEitf)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITFLIST,_FOOTER_);
  
  vRemove(pSEitf);
  
  gpSmallHeap->vFreeObject(pSEitf);
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SEitfList::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITFLIST);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/