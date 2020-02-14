#include "PETScInternalLinearAlgebra.h"

#include <ALIEN/Kernels/PETSc/PETScBackEnd.h>

#include <ALIEN/Core/Backend/LinearAlgebraT.h>
#include <ALIEN/Data/Space.h>
#include <ALIEN/Kernels/PETSc/DataStructure/PETScInit.h>
#include <ALIEN/Kernels/PETSc/DataStructure/PETScInternal.h>
#include <ALIEN/Kernels/PETSc/DataStructure/PETScMatrix.h>
#include <ALIEN/Kernels/PETSc/DataStructure/PETScVector.h>

#include <arccore/message_passing_mpi/MpiMessagePassingMng.h>

/*---------------------------------------------------------------------------*/

namespace Alien {

template class ALIEN_EXTERNALPACKAGES_EXPORT LinearAlgebra<BackEnd::tag::petsc>;

IInternalLinearAlgebra<PETScMatrix, PETScVector>*
PETScInternalLinearAlgebraFactory(Arccore::MessagePassing::IMessagePassingMng* pm)
{
  return new PETScInternalLinearAlgebra(pm);
}

/*---------------------------------------------------------------------------*/

PETScInternalLinearAlgebra::PETScInternalLinearAlgebra(
    Arccore::MessagePassing::IMessagePassingMng* pm)
{
  PETScInternal::initPETSc();
  // Devrait faire le PETScInitialize qui est actuellement dans le solveur
  // Attention, cette initialisation serait globale et non restreinte à cet objet
  if (pm != nullptr && pm->commSize() > 1) {
    auto mpi_mng = dynamic_cast<Arccore::MessagePassing::Mpi::MpiMessagePassingMng*>(pm);
    PETSC_COMM_WORLD = *static_cast<const MPI_Comm*>(mpi_mng->getMPIComm());
  }
}

/*---------------------------------------------------------------------------*/

PETScInternalLinearAlgebra::~PETScInternalLinearAlgebra()
{
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

Arccore::Real
PETScInternalLinearAlgebra::norm0(const PETScVector& vx) const
{
  Arccore::Real result = 0;
  VecNorm(vx.internal()->m_internal, NORM_INFINITY, &result);
  return result;
}

/*---------------------------------------------------------------------------*/

Arccore::Real
PETScInternalLinearAlgebra::norm1(const PETScVector& vx) const
{
  Arccore::Real result = 0;
  VecNorm(vx.internal()->m_internal, NORM_1, &result);
  return result;
}

/*---------------------------------------------------------------------------*/

Arccore::Real
PETScInternalLinearAlgebra::norm2(const PETScVector& vx) const
{
  Arccore::Real result = 0;
  VecNorm(vx.internal()->m_internal, NORM_2, &result);
  return result;
}

/*---------------------------------------------------------------------------*/

void
PETScInternalLinearAlgebra::mult(
    const PETScMatrix& ma, const PETScVector& vx, PETScVector& vr) const
{
  MatMult(
      ma.internal()->m_internal, vx.internal()->m_internal, vr.internal()->m_internal);
}

/*---------------------------------------------------------------------------*/

void
PETScInternalLinearAlgebra::axpy(
    const Arccore::Real& alpha, const PETScVector& vx, PETScVector& vr) const
{
  VecAXPY(vr.internal()->m_internal, alpha, vx.internal()->m_internal);
}

/*---------------------------------------------------------------------------*/

void
PETScInternalLinearAlgebra::aypx(
    const Arccore::Real& alpha, PETScVector& vy, const PETScVector& vx) const
{
  VecAYPX(vy.internal()->m_internal, alpha, vx.internal()->m_internal);
}

/*---------------------------------------------------------------------------*/

void
PETScInternalLinearAlgebra::copy(const PETScVector& vx, PETScVector& vr) const
{
  VecCopy(vx.internal()->m_internal, vr.internal()->m_internal);
}

/*---------------------------------------------------------------------------*/

Arccore::Real
PETScInternalLinearAlgebra::dot(const PETScVector& vx, const PETScVector& vy) const
{
  Arccore::Real result = 0;
  VecDot(vx.internal()->m_internal, vy.internal()->m_internal, &result);
  return result;
}

/*---------------------------------------------------------------------------*/

void
PETScInternalLinearAlgebra::scal(const Arccore::Real& alpha, PETScVector& vx) const
{
  VecScale(vx.internal()->m_internal, alpha);
}

/*---------------------------------------------------------------------------*/

void
PETScInternalLinearAlgebra::diagonal(const PETScMatrix& a, PETScVector& x) const
{
  MatGetDiagonal(a.internal()->m_internal, x.internal()->m_internal);
}

/*---------------------------------------------------------------------------*/

void
PETScInternalLinearAlgebra::reciprocal(PETScVector& x) const
{
  VecReciprocal(x.internal()->m_internal);
}

/*---------------------------------------------------------------------------*/

void
PETScInternalLinearAlgebra::pointwiseMult(
    const PETScVector& x, const PETScVector& y, PETScVector& w) const
{
  VecPointwiseMult(
      w.internal()->m_internal, x.internal()->m_internal, y.internal()->m_internal);
}

/*---------------------------------------------------------------------------*/

} // namespace Alien

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
