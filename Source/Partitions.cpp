/*============================================================================*/
/* Partitions.cpp - table partitions                                          */
/*============================================================================*/

#include "DBobjects.h"

#include "StorageEngine.h"

extern Arguments gxArguments;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Partitions::operator new(size_t iSize)
{
  vFatal("fatal: Partitions class cannot be new'd");
  
  //vExitFatal();
  
  exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Partitions::operator delete(void* pBufferAddress)
{
  vFatal("fatal: Partitions class cannot be deleted");
  
  //vExitFatal();
  
  exit(EXIT_FAILURE);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Partitions::Partitions(void)
{
  _poolAddress_(this);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Partitions::~Partitions(void)
{
}

/*----------------------------------------------------------------------------*/
/* get partition                                                              */
/*----------------------------------------------------------------------------*/
Partition* Partitions::pPartition(i8 iPartition)
{
  return &m_xPartition[iPartition];
}

/*----------------------------------------------------------------------------*/
/* tuncate all partitions                                                     */
/*----------------------------------------------------------------------------*/
void Partitions::vTruncate(void)
{
  i8 iPartn;
  for (iPartn = 0; iPartn < gxArguments.m_iPartitions; iPartn++)
    m_xPartition[iPartn].vTruncate();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/