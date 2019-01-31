/*============================================================================*/
/* EntWrk.hpp -- Working Entry (contains an embedded object)                  */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class EntWrk : public Object
  {
  friend class Entry;
  friend class Table;
  
  static const i8 C_WSP_ENTWRK = C_17KB;
  
  private:
    Id    m_xId;       // id of row, or referenced entry in another buffer
    i8    m_nOffsets;  // number of offsets (0..3)
  
    char* m_aEmbedded; // address of embedded object. this may be different
                       // from m_xWsp.aArea() to allow for the object
                       // not to have to be copied into the wsp
  
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // key and row workspace
    Wsp_t<C_WSP_ENTWRK> m_xWsp; // embedded object workspace
  
    Footer m_xFooter;
  
  public:
     EntWrk(eCN eClass);
    ~EntWrk(void) noexcept(false);
  
    char*  aEmbedded(void);
    void   vEmbedded(char* aEmbedded);
    i8     nEntrySize(void);
    Id&    xId(void);
    void   vId(Id& xId);
    char*  aLatter(void);
    i8     nOffsets(void);
    i8     nOffsetSize(void);
    void   vOffsets(i8 nOffsets);
    void   vRandomOffsets(void);
    void   vReset(void);
    void   vTake(i8 iSize);

    Footer* aFooter(void);
  
  protected:
    i8     nEmbeddedSize(void);
    void   vEmbeddedSize(i8 iSize);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
