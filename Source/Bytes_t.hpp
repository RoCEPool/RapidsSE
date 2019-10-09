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
    Stz* szGet(i8 i_Area);
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
