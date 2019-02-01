/*============================================================================*/
/* Item.cpp -- buffer item                                                    */
/*============================================================================*/

#include "DBobjects.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Item::operator new(size_t)
{
  vFatal("fatal: Item class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Item::operator new(size_t, Item* aItem)
{
  return aItem;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Item::Item(eCN eClassNo, i8 nSizeItem)
: Header(eClassNo)
{
  m_nSizeItem = static_cast<i4>(nSizeItem);
  
#ifdef WITH_DEBUG_VERSION
  m_iSkipNo = -1;
  m_iIntKey = -1;
#endif
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Item::~Item(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get buffer no                                                              */
/*----------------------------------------------------------------------------*/
i8 Item::iBufferNo(void)
{
  return m_xId.iBuffer();
}

/*----------------------------------------------------------------------------*/
/* get bufferno from id of key                                                */
/*----------------------------------------------------------------------------*/
bn Item::nBufferNo(void)
{
  bn nBufferNo = m_xId.iBuffer();
  
  return nBufferNo;
}

/*----------------------------------------------------------------------------*/
/* get address of data area                                                   */
/*----------------------------------------------------------------------------*/
char* Item::aDataArea(void)
{
  char* aData = aAdd<char*>(this,sizeof(Item));
  
  return pPoolCheck<char*>(aData);
}

/*----------------------------------------------------------------------------*/
/* get rowid                                                                  */
/*----------------------------------------------------------------------------*/
Id& Item::xId(void)
{
  return m_xId;
}

/*----------------------------------------------------------------------------*/
/* get rowid                                                                  */
/*----------------------------------------------------------------------------*/
void Item::vId(Id xId)
{
  m_xId = xId;
}

/*----------------------------------------------------------------------------*/
/* get item in memory after this item                                         */
/*----------------------------------------------------------------------------*/
Item* Item::aItemAfter(void)
{
  i8 nItemBytes = nSizeItem();
  
  i8 nNEObytes = iNEOsize();
  
  i8 nItemPlusNEO = nItemBytes + nNEObytes;
  
  Item* aAfter = aAdd<Item*>(this,nItemPlusNEO);
  
  return aAfter;
}

/*----------------------------------------------------------------------------*/
/* get item number                                                            */
/*----------------------------------------------------------------------------*/
i8 Item::iItemNo(void)
{
  return m_xId.iItem();
}

/*----------------------------------------------------------------------------*/
/* get address of next entry offset array                                     */
/*----------------------------------------------------------------------------*/
NEO* Item::aNEO(void)
{
  //Footer* aFooter = this->aFooter();
  
  //NEO* aNEO = pAdd<NEO*>(aFooter,sizeof(Footer));
  
  NEO* aNEO = aAdd<NEO*>(this, nSizeItem());
  
  return aNEO;
}

/*----------------------------------------------------------------------------*/
/* get a next entry offset                                                    */
/*----------------------------------------------------------------------------*/
i4 Item::iNEO(i8 iOffset)
{
  ASSERT(iOffset >= 0 && iOffset < nOffsets());
  
  //NEO* aNEO = this->aNEO();
  
  //i4 iNEO = aNEO->m_iOffset[iOffset];
  
  i4 iNEO = *(aAdd<i4*>(this, nSizeItem() + sizeof(i4) * iOffset));
  
  return iNEO;
}

/*----------------------------------------------------------------------------*/
/* set a next entry offset                                                    */
/*----------------------------------------------------------------------------*/
void Item::vNEO(i8 iOffset, i4 iNEO)
{
  ASSERT(iOffset >= 0 && iOffset < nOffsets());
  
  //NEO* aNEO = this->aNEO();
  
  //aNEO->m_iOffset[iOffset] = iNEO;
  
  *(aAdd<i4*>(this, nSizeItem() + sizeof(i4) * iOffset)) = iNEO;
  
}

/*----------------------------------------------------------------------------*/
/* dump the NEO after this entry                                              */
/*----------------------------------------------------------------------------*/
void Item::vNEOdump(void)
{
  NEO* aNEO = this->aNEO();
  vTrace("Entry %p offsets: %lld NEO: %p"_Stz,
         this,nOffsets(),aNEO);
  
  for (i8 i_Offset = 0; i_Offset < nOffsets(); i_Offset++)
    vTrace(" %lld: %d"_Stz,i_Offset,aNEO->m_iOffset[i_Offset]);
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* get size of the NEO after this entry                                       */
/*----------------------------------------------------------------------------*/
i8 Item::iNEOsize(void)
{
  i8 nBytes = sizeof(i4) * nOffsets();
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* get number of offsets                                                      */
/*----------------------------------------------------------------------------*/
i8 Item::nOffsets(void)
{
  return m_xId.nOffsets();
}

/*----------------------------------------------------------------------------*/
/* set number of offsets                                                      */
/*----------------------------------------------------------------------------*/
void Item::vOffsets(i8 nOffsets)
{
  m_xId.vOffsets(nOffsets);
}

/*----------------------------------------------------------------------------*/
/* get size in bytes of this item                                             */
/*----------------------------------------------------------------------------*/
i8 Item::nSizeItem(void)
{
  return (i8)m_nSizeItem;
}

/*----------------------------------------------------------------------------*/
/* test for valid entry                                                       */
/*----------------------------------------------------------------------------*/
bool Item::bValidItem(void)
{  
  if (! bValidMagic()) {
    vTrace("Not valid magic\n"_Stz);
    return false;
  }
  
  if (CN_ENTRY != eClass()) {
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
