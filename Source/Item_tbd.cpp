/*============================================================================*/
/* Item_tbd.cpp -- buffer item                                                    */
/*============================================================================*/

#include "Data.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Item_tbd::operator new(size_t)
{
  vFatal("fatal: Item_tbd class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Item_tbd::operator new(size_t, Item_tbd* aItem)
{
  return aItem;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Item_tbd::Item_tbd(eCN eClassNo, i8 nSizeItem)
: Header(eClassNo)
{
  m_nSizeItem_WithoutOffsets = static_cast<i4>(nSizeItem);
  
#ifdef WITH_DEBUG_VERSION
  m_iSkipNo = -1;
  //m_iIntKey = -1;
#endif
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Item_tbd::~Item_tbd(void) noexcept(false)
{
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get rowid                                                                  */
/*----------------------------------------------------------------------------*/
Id& Item_tbd::xId(void)
{
  return m_xId;
}

/*----------------------------------------------------------------------------*/
/* get rowid                                                                  */
/*----------------------------------------------------------------------------*/
void Item_tbd::vId(Id xId)
{
  m_xId = xId;
}

/*----------------------------------------------------------------------------*/
/* get item in memory after this item                                         */
/*----------------------------------------------------------------------------*/
Item_tbd* Item_tbd::aItemAfter(void)
{
  i8 nItemBytes = nSizeItem_WithoutOffsets();
  
  i8 nNEObytes = iNEOsize();
  
  i8 nItemPlusNEO = nItemBytes + nNEObytes;
  
  Item_tbd* aAfter = aAdd<Item_tbd*>(this,nItemPlusNEO);
  
  return aAfter;
}

/*----------------------------------------------------------------------------*/
/* get item number                                                            */
/*----------------------------------------------------------------------------*/
i8 Item_tbd::iItemNo(void)
{
  return m_xId.iItem();
}

/*----------------------------------------------------------------------------*/
/* get a next entry offset                                                    */
/*----------------------------------------------------------------------------*/
i4 Item_tbd::iNEO(i8 iOffset)
{
  ASSERT(iOffset >= 0 && iOffset < nLevels());
  
  //NEO* aNEO = this->aNEO();
  
  //i4 iNEO = aNEO->m_iOffset[iOffset];
  
  i4 iNEO = *(aAdd<i4*>(this, nSizeItem_WithoutOffsets() + sizeof(i4) * iOffset));
  
  return iNEO;
}

/*----------------------------------------------------------------------------*/
/* get size of the NEO after this entry                                       */
/*----------------------------------------------------------------------------*/
i8 Item_tbd::iNEOsize(void)
{
  i8 nBytes = sizeof(i4) * nLevels();
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* set a next entry offset                                                    */
/*----------------------------------------------------------------------------*/
void Item_tbd::vNEO(i8 iOffset, i4 iNEO)
{
  ASSERT(iOffset >= 0 && iOffset < nLevels());
  
  //NEO* aNEO = this->aNEO();
  
  //aNEO->m_iOffset[iOffset] = iNEO;
  
  *(aAdd<i4*>(this, nSizeItem_WithoutOffsets() + sizeof(i4) * iOffset)) = iNEO;
  
}

/*----------------------------------------------------------------------------*/
/* dump the NEO after this entry                                              */
/*----------------------------------------------------------------------------*/
void Item_tbd::vNEOdump(void)
{
  NEO* aNEO = this->aNEO();
  vTrace("Item_tbd %p levels: %lld NEO: %p"_Stz,
         this,nLevels(),aNEO);
  
  for (i8 i_Level = 0; i_Level < nLevels(); i_Level++)
    vTrace(" %lld: %d"_Stz,i_Level,aNEO->m_iOffset[i_Level]);
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* get number of levels                                                       */
/*----------------------------------------------------------------------------*/
i8 Item_tbd::nLevels(void)
{
  return m_xId.nLevels();
}

/*----------------------------------------------------------------------------*/
/* set number of levels                                                       */
/*----------------------------------------------------------------------------*/
void Item_tbd::vLevels(i8 nLevels)
{
  m_xId.vLevels(nLevels);
}

/*----------------------------------------------------------------------------*/
/* get size in bytes of this item                                             */
/*----------------------------------------------------------------------------*/
i8 Item_tbd::nSizeData(void)
{
  return (i8)(m_nSizeItem_WithoutOffsets - sizeof(Item_tbd));
}

/*----------------------------------------------------------------------------*/
/* get size in bytes of this item                                             */
/*----------------------------------------------------------------------------*/
i8 Item_tbd::nSizeItem_WithOffsets(void)
{
  i8 nBytes = nSizeItem_WithoutOffsets() + iNEOsize();
  
  return nBytes;
}
/*----------------------------------------------------------------------------*/
/* get size in bytes of this item                                             */
/*----------------------------------------------------------------------------*/
i8 Item_tbd::nSizeItem_WithoutOffsets(void)
{
  return (i8)m_nSizeItem_WithoutOffsets;
}

/*----------------------------------------------------------------------------*/
/* test for valid entry                                                       */
/*----------------------------------------------------------------------------*/
bool Item_tbd::bValidItem(void)
{  
  if (! bValidMagic()) {
    vTrace("Not valid magic\n"_Stz);
    return false;
  }
  
  if ( ! (CN_ENTRY == eClass() || CN_FREEENTRY == eClass()) ) {
    vTrace("Not valid class\n"_Stz);
    return false;
  }
  
#ifdef USE_LW_FOOOTER
  if (! aFooter()->bValidFooter()) {
    vTrace("Not valid footer\n"_Stz);
    return false;
  }
#endif
  
  return true;
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get address of next entry offset array                                     */
/*----------------------------------------------------------------------------*/
NEO* Item_tbd::aNEO(void)
{
  NEO* aNEO = aAdd<NEO*>(this, nSizeItem_WithoutOffsets());
  
  return aNEO;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
