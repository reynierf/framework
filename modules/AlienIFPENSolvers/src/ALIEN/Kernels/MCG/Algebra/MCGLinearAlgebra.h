/*
 * HypreLinearAlgebra.h
 *
 *  Created on: Jun 13, 2012
 *      Author: gratienj
 */
#ifndef ALIEN_KERNEL_MCG_ALGEBRA_MCGLINEARALGEBRA_H
#define ALIEN_KERNEL_MCG_ALGEBRA_MCGLINEARALGEBRA_H

#include <ALIEN/Utils/Precomp.h>

#include <ALIEN/Kernels/MCG/MCGBackEnd.h>
#include <ALIEN/Core/Backend/LinearAlgebra.h>
#include <ALIEN/Kernels/MCG/Algebra/MCGInternalLinearAlgebra.h>

/*---------------------------------------------------------------------------*/

namespace Alien {

// typedef LinearAlgebra<BackEnd::tag::mcgsolver> MCGLinearAlgebra;
typedef MCGInternalLinearAlgebra MCGLinearAlgebra;

} // namespace Alien

/*---------------------------------------------------------------------------*/

#endif /* ALIEN_LINEARALGEBRA2_HYPREIMPL_HYPRELINEARALGEBRA_H */
