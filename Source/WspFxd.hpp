/*============================================================================*/
/* WspFxd.hpp -- fixed-sized workspace                                        */
/*============================================================================*/
#pragma once

/*
 
 Description:
 
 */

namespace Magic
{
  template<int LENGTH>
  class WspFxd : public Wsp
  {
  private:
    char m_cArea[LENGTH]; // fixed byte area
    i8   m_nAreaUsed;     // bytes used in the area (must be after m_cArea)
  
  public:
    WspFxd(void);

    char* aAllot(i8 nBytes);
    void  vAppend(void* aSrc, i8 nBytes);
    char* aArea(void);
    void  vFill(char cByte);
    i8    nFree(void);
    void  vGive(i8 nBytes);
    void  vInit(void);
    char* aLatter(void);
    i8    nLength(void);
    i8    nSize(void);
    void  vTake(i8 nBytes);
    i8    nUsed(void);
    void  vUsed(i8 nBytes);
    void  vZero(void);
  };
}

using namespace Magic;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
WspFxd<LENGTH>::WspFxd(void)
{
  m_nAreaUsed = 0;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspFxd<LENGTH>::vInit(void)
{
  m_nAreaUsed = 0;
}

/*----------------------------------------------------------------------------*/
/* allot bytes from byte area                                                 */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
char* WspFxd<LENGTH>::aAllot(i8 nBytes)
{
  char* aAllotted = aLatter();
  
  vTake(nBytes);
  
  return aAllotted;
}

/*----------------------------------------------------------------------------*/
/* append source bytes to byte area                                           */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspFxd<LENGTH>::vAppend(void* aSrc, i8 nBytes)
{
  ASSERT(nUsed() + nBytes <= LENGTH);
  
  void* aDst = aLatter();

  MemLib::vCopy(aDst,aSrc,nBytes);
  
  vTake(nBytes);
}

/*----------------------------------------------------------------------------*/
/* get address of byte area                                                   */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
char* WspFxd<LENGTH>::aArea(void)
{
  char* aArea = Wsp::aArea();
  
  return aArea;
}

/*----------------------------------------------------------------------------*/
/* fill byte area with given byte                                             */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspFxd<LENGTH>::vFill(char cByte)
{
  Wsp::vFill(cByte,LENGTH);
}

/*----------------------------------------------------------------------------*/
/* get number of free bytes in byte area                                      */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
i8 WspFxd<LENGTH>::nFree(void)
{
  i8 nFree = LENGTH - nUsed();
  
  return nFree;
}

/*----------------------------------------------------------------------------*/
/* get length of byte area                                                    */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
i8 WspFxd<LENGTH>::nLength(void)
{
  return LENGTH;
}

/*----------------------------------------------------------------------------*/
/* get address of next available chunk in byte area                           */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
char* WspFxd<LENGTH>::aLatter(void)
{
  char* aNext = &aArea()[nUsed()];
  
  return aNext;
}

/*----------------------------------------------------------------------------*/
/* get total size of this WspFxd                                              */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
i8 WspFxd<LENGTH>::nSize(void)
{
  return sizeof(WspFxd<LENGTH>);
}

/*----------------------------------------------------------------------------*/
/* take chunk from byte area                                                  */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspFxd<LENGTH>::vTake(i8 nBytes)
{
  m_nAreaUsed += nBytes;
  
  ASSERT(m_nAreaUsed <= LENGTH);
}

/*----------------------------------------------------------------------------*/
/* give chunk back to byte area                                               */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspFxd<LENGTH>::vGive(i8 nBytes)
{
  ASSERT(m_nAreaUsed <= LENGTH);

  m_nAreaUsed -= nBytes;
}

/*----------------------------------------------------------------------------*/
/* get number of used bytes                                                   */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspFxd<LENGTH>::vUsed(i8 nBytes)
{
  m_nAreaUsed = nBytes;
  
  ASSERT(m_nAreaUsed <= LENGTH);
}

/*----------------------------------------------------------------------------*/
/* get number of used bytes                                                   */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
i8 WspFxd<LENGTH>::nUsed(void)
{
  return m_nAreaUsed;
}

/*----------------------------------------------------------------------------*/
/* zero fill the byte area                                                    */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspFxd<LENGTH>::vZero(void)
{
  Wsp::vZero(LENGTH);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
