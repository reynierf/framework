#include "IFPSolverInternal.h"
/* Author : havep at Fri Jul 20 13:21:25 2012
 * Generated by createNew
 */


#include <IFPSolver.h>
#include <ALIEN/Core/Impl/MultiVectorImpl.h>

/*---------------------------------------------------------------------------*/

BEGIN_IFPSOLVERINTENRAL_NAMESPACE

/*---------------------------------------------------------------------------*/

const MultiMatrixImpl * MatrixInternal::m_static_multi_impl = NULL;
bool VectorInternal::m_static_representation_switch = false;
const MultiVectorImpl * VectorInternal::m_static_multi_impl = NULL;
int VectorInternal::m_static_init_rhs = 0 ;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

MatrixInternal::
MatrixInternal(const MultiMatrixImpl * matrix_impl, Arccore::Int64 timestamp)
  : m_timestamp(timestamp)
{ 
  if (m_static_multi_impl != NULL)
    throw Arccore::FatalErrorException(A_FUNCINFO,"Cannot build two instances of vector with IFPSolver");
  m_static_multi_impl = matrix_impl;
}

/*---------------------------------------------------------------------------*/

MatrixInternal::
~MatrixInternal() 
{ 
  F2C(ifpmatrixfreedata)();
  F2C(ifpsolverfreegraph)();
  m_static_multi_impl = NULL;
}

void MatrixInternal::init()
{
  m_filled = false ;
  m_extra_filled = false ;
}
/*---------------------------------------------------------------------------*/

VectorInternal::
VectorInternal(const MultiVectorImpl * vector_impl)
  : m_offset(0)
  , m_local_size(0)
  , m_rows(0)
  , m_filled(false)
  , m_extra_filled(false)
{
  //if (m_static_multi_impl != NULL)
  //  throw FatalErrorException(A_FUNCINFO,"Cannot build two instances of vector with IFPSolver");
  m_static_multi_impl = vector_impl;
}

/*---------------------------------------------------------------------------*/

VectorInternal::
~VectorInternal()
{
  delete [] m_rows ;
  //m_static_multi_impl = NULL;
}

/*---------------------------------------------------------------------------*/

void
VectorInternal::
setRepresentationSwitch(const bool s)
{
  m_static_representation_switch = s;
}

/*---------------------------------------------------------------------------*/

bool
VectorInternal::
hasRepresentationSwitch()
{
  return m_static_representation_switch;
}

void
VectorInternal::
initRHS(const bool s)
{
    m_static_init_rhs = s?1:0;
}

int
VectorInternal::isRHS()
{
  return m_static_init_rhs ;
}

/*---------------------------------------------------------------------------*/

END_IFPSOLVERINTERNAL_NAMESPACE

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
