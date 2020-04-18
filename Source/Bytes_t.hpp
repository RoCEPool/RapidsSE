/*============================================================================*/
/* Bytes_t.hpp -- fixed-length byte array variable (template)                 */
/*============================================================================*/
#pragma once

namespace Esses
{
  template <int BYTES>
  class Bytes_t : public Object
  {
  private:
    i8     m_i_Latter;      // offset of next available byte
    byte   m_yaArea[BYTES]; // byte area
    
    // must be last
    Footer m_xFooter;
    
  public:    
    Bytes_t(void);
   ~Bytes_t(void) noexcept(false);
  
    i8   iAppend(Stz* szStr);
    void vCut(i8 i_At, i8 nBytes);
    void vCut(Stz* szWhereToCut, i8 nBytes);
    Stz* szGet(i8 i_Area);
    Stz* szLatter(void);
    void vListAll(void);
    void vPrintAll(void);
    void vReset(void);
  
    Footer* aFooter(void);
  };
  
  void* _pBytes_t(void* aAddr);
}

using namespace Correctness;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template <int BYTES>
Bytes_t<BYTES>::Bytes_t(void)
: Object(CN_BYTES_T)
{
  _precondition_(_MAGIC_,CN_BYTES_T,_FOOTER_);
  
  m_i_Latter = 0;
  
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_yaArea,BYTES,CN_BYTES_T);
#else
  m_yaArea[0] = '\0';
#endif

  _checkInitialisedSize_(BYTES);
  
  _postcondition_(_MAGIC_,CN_BYTES_T,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
template <int BYTES>
Bytes_t<BYTES>::~Bytes_t(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_BYTES_T,_FOOTER_);
  
  _postcondition_(_MAGIC_,CN_BYTES_T,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append given string                                                        */
/*----------------------------------------------------------------------------*/
template <int BYTES>
i8 Bytes_t<BYTES>::iAppend(Stz* szStr)
{
  _precondition_(_MAGIC_,CN_BYTES_T,_FOOTER_);
  
  i8 i_String = m_i_Latter;
  
  for (byte* ayByte = (byte*) szStr; * ayByte; )
    m_yaArea[m_i_Latter++] = * ayByte++;

  m_yaArea[m_i_Latter++] = '\0';

  _postcondition_(_MAGIC_,CN_BYTES_T,_FOOTER_);
  
  return i_String;
}

/*----------------------------------------------------------------------------*/
/* cut nBytes starting at given offset                                        */
/*----------------------------------------------------------------------------*/
template <int BYTES>
void Bytes_t<BYTES>::vCut(i8 i_At, i8 nBytes)
{
  _precondition_(_MAGIC_,CN_BYTES_T,_FOOTER_);
  
  Stz* szWhereToCut = szGet(i_At);

  vCut(szWhereToCut,nBytes);
}

/*----------------------------------------------------------------------------*/
/* cut nBytes starting at given offset                                        */
/*----------------------------------------------------------------------------*/
template <int BYTES>
void Bytes_t<BYTES>::vCut(Stz* szWhereToCut, i8 nBytes)
{
  _precondition_(_MAGIC_,CN_BYTES_T,_FOOTER_);
  
  Stz* szDst = szWhereToCut;
  Stz* szSrc = aAdd<Stz*>(szWhereToCut,nBytes);
  
  MemLib::vMove(szDst,szSrc,nBytes,CN_BYTES_T);
}

/*----------------------------------------------------------------------------*/
/* get address of the later portion                                           */
/*----------------------------------------------------------------------------*/
template <int BYTES>
Stz* Bytes_t<BYTES>::szLatter(void)
{
  _precondition_(_MAGIC_,CN_BYTES_T,_FOOTER_);
  
  Stz* szLater = szGet(m_i_Latter);
  
  return szLater;
}

/*----------------------------------------------------------------------------*/
/* get string at given subscript in m_yaArea                                  */
/*----------------------------------------------------------------------------*/
template <int BYTES>
Stz* Bytes_t<BYTES>::szGet(i8 i_Area)
{
  _precondition_(_MAGIC_,CN_BYTES_T,_FOOTER_);
  
  char* acStr = (char*)& m_yaArea[i_Area];
  
  return (Stz*)acStr;
}

/*----------------------------------------------------------------------------*/
/* list all embedded strings                                                  */
/*----------------------------------------------------------------------------*/
template <int BYTES>
void Bytes_t<BYTES>::vListAll(void)
{
  _precondition_(_MAGIC_,CN_BYTES_T,_FOOTER_);
  
  byte* ayWild = & m_yaArea[m_i_Latter];
  
  bool bMarkup = true;

  i8 i_String = 0;
  for (byte* ayByte = m_yaArea; ayByte < ayWild; ayByte++) {
  
    if (bMarkup) {
      bMarkup = false;
      vTrace("[%02lld-%04lld] "_Stz);
    }

    if (* ayByte) {
      vTrace("%c"_Stz,* ayByte);
      continue;
    }
    
    vTrace("\n"_Stz);
    bMarkup = true;
  }
}

/*----------------------------------------------------------------------------*/
/* print all embedded strings                                                 */
/*----------------------------------------------------------------------------*/
template <int BYTES>
void Bytes_t<BYTES>::vPrintAll(void)
{
  _precondition_(_MAGIC_,CN_BYTES_T,_FOOTER_);
  
  for (byte* ayByte = m_yaArea; ayByte < & m_yaArea[m_i_Latter]; ayByte++)
    if (* ayByte)
      vTrace("%c"_Stz,* ayByte);
    else
    if (ayByte + 1 < & m_yaArea[m_i_Latter])
      vTrace(","_Stz);

  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* reset                                                                      */
/*----------------------------------------------------------------------------*/
template <int BYTES>
void Bytes_t<BYTES>::vReset(void)
{
  _precondition_(_MAGIC_,CN_BYTES_T,_FOOTER_);
  
  m_i_Latter = 0;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
template <int BYTES>
Footer* Bytes_t<BYTES>::aFooter(void)
{
  _precondition_(_MAGIC_,CN_BYTES_T);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
