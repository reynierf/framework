#ifndef ARCGEOSIM_NUMERICS_LINEARSOLVER_PETSCSOLVERIMPL_IPETSCPC_H
#define ARCGEOSIM_NUMERICS_LINEARSOLVER_PETSCSOLVERIMPL_IPETSCPC_H
/* Author : havep at Fri Jun 27 21:33:46 2008
 * Generated by createNew
 */

#include <ALIEN/Kernels/PETSc/PETScPrecomp.h>
#include <petscpc.h>

#include <ALIEN/Data/Space.h>
#include <ALIEN/Distribution/MatrixDistribution.h>

// For old PETSc version
#ifndef MATSOLVERSUPERLU_DIST
#define MATSOLVERSUPERLU_DIST MAT_SOLVER_SUPERLU_DIST
#endif // MATSOLVERSUPERLU_DIST

#ifndef MATSOLVERSUPERLU
#define MATSOLVERSUPERLU MAT_SOLVER_SUPERLU
#endif // MATSOLVERSUPERLU

#ifndef MATSOLVERPETSC
#define MATSOLVERPETSC MAT_SOLVER_PETSC
#endif // MATSOLVERPETSC

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Alien {

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

class BaseParameterMng;

class IPETScPC
{
 public:
  IPETScPC() {}

  virtual ~IPETScPC() {}

 public:
  //! Initialisation
  virtual void configure(PC & pc,
                         const ISpace& space,
                         const MatrixDistribution& distribution) = 0;

  //! Check need of KSPSetUp before calling this PC configure
  virtual bool needPrematureKSPSetUp() const = 0;
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

} // namespace Alien

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#endif /* ARCGEOSIM_NUMERICS_LINEARSOLVER_PETSCSOLVERIMPL_IPETSCPC_H */
