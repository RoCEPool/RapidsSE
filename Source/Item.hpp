/*============================================================================*/
/* Item.hpp -- buffer item                                                    */
/*============================================================================*/
#pragma once

namespace MiniHeaps
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class NEO // next entry offset
  {
  public:
    i4 m_iOffset[1];  // offset where the entry is
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Item : public Header
  {
  protected:
    Id  m_xId;       // item id
    i4  m_nSizeItem; // size of this item
  
  public:
//#ifdef WITH_DEBUG_VERSION
  i8  m_iSkipNo;    // verify of order of skip items
  i8  m_iIntKey;    // key debug hack
//#endif

  public:
    void* operator new(size_t);
    void* operator new(size_t, Item*);
  
    Item(eCN eClassNo, i8 nSizeItem);
    ~Item(void) noexcept(false);
  
    i8    iBufferNo(void);
    bn    nBufferNo(void);
    char* aDataArea(void);
    Id&   xId(void);
    void  vId(Id xId);
    Item* aItemAfter(void);
    i8    iItemNo(void);
    NEO*  aNEO(void);
    i4    iNEO(i8 i_Offset);
    void  vNEO(i8 i_Offset, i4 iNEO);
    void  vNEOdump(void);
    i8    iNEOsize(void);
    i8    nOffsets(void);
    void  vOffsets(i8 nOffsets);
    Item* aRefdItem(void);
    i8    nSizeItem(void);
    bool  bValidItem(void);
  };
  
  /*--------------------------------------------------------------------------*/
  /* safe cast                                                                */
  /*--------------------------------------------------------------------------*/
  inline Item* _aItem(void* aAddr)
  {
    Item* aItem = reinterpret_cast<Item*>(aAddr);
    
    return aItem;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
