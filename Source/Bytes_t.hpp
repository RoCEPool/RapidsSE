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
    byte*  m_ayLatter;      // address of next available byte
    byte   m_yaArea[BYTES]; // byte area
    
    // must be last
    Footer m_xFooter;
    
  public:    
    Bytes_t(void);
   ~Bytes_t(void) noexcept(false);
  
    void vAppend(Stz* szStr);
    void vPrintAll(void);

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
  
  m_ayLatter = m_yaArea;
  
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_yaArea,BYTES,CN_BYTES_T);
#else
  m_caArea[0] = '\0';
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
void Bytes_t<BYTES>::vAppend(Stz* szStr)
{
  _precondition_(_MAGIC_,CN_BYTES_T,_FOOTER_);
  
  for (byte* ayByte = (byte*) szStr; * ayByte; )
    * m_ayLatter++ = * szStr++;

  * m_ayLatter++ = '\0';

  _postcondition_(_MAGIC_,CN_BYTES_T,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* print all embedded strings                                                 */
/*----------------------------------------------------------------------------*/
template <int BYTES>
void Bytes_t<BYTES>::vPrintAll(void)
{
  _precondition_(_MAGIC_,CN_BYTES_T,_FOOTER_);
  
  for (byte* ayByte = m_yaArea; ayByte < m_ayLatter; ayByte++)
    vTrace("%c"_Stz,* ayByte);
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
