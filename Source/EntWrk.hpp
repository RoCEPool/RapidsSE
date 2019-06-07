/*============================================================================*/
/* EntWrk.hpp -- Working Ent (contains an embedded object)                  */
/*============================================================================*/
#pragma once

namespace Data
{
  class EntWrk : public Object
  {
  friend class Ent;
  friend class Table;
  
  static const i8 C_WSP_ENTWRK = C_17KB;
  
  private:
    Id    m_xId;       // id of row, or referenced entry in another buffer
  
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
  
    i8     iBufferNo(void);
    void   vBufferNo(i8 iBufferNo);
    char*  aEmbedded(void);
    void   vEmbedded(char* aEmbedded);
    i8     nEntrySize(void);
    Id&    xId(void);
    void   vId(Id& xId);
    i8     iItemNo(void);
    void   vItemNo(i8 iItemNo);
    char*  aLatter(void);
    i8     nLevels(void);
    i8     nOffsetSize(void);
    void   vLevels(i8 nLevels);
    void   vRandomLevels(void);
    void   vReset(void);
    void   vTake(i8 iSize);

    Footer* aFooter(void);
  
  protected:
    i8     nDataSize(void);
    void   vDataSize(i8 nDataBytes);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
