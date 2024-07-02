﻿// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
//-----------------------------------------------------------------------------
// Copyright 2000-2024 CEA (www.cea.fr) IFPEN (www.ifpenergiesnouvelles.com)
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: Apache-2.0
//-----------------------------------------------------------------------------
/*---------------------------------------------------------------------------*/
/* ICriteriaLoadBalanceMng.h                                   (C) 2000-2024 */
/*                                                                           */
/* Interface pour un gestionnaire des critères d'équilibre de charge des     */
/* maillages.                                                                */
/*---------------------------------------------------------------------------*/
#ifndef ARCANE_CORE_ICRITERIALOADBALANCEMNG_H
#define ARCANE_CORE_ICRITERIALOADBALANCEMNG_H
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#include "arcane/core/ArcaneTypes.h"
#include "arcane/core/VariableTypes.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Arcane
{

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*!
 * \brief Interface d'enregistrement des variables pour l'equilibrage de charge.
 */
class ICriteriaLoadBalanceMng
{
 public:

  virtual ~ICriteriaLoadBalanceMng() = default; //!< Libère les ressources.

 public:

  virtual void addMass(VariableCellInt32& count, const String& entity="") =0;
  virtual void addCriterion(VariableCellInt32& count) =0;
  virtual void addCriterion(VariableCellReal& count) =0;
  virtual void addCommCost(VariableFaceInt32& count, const String& entity="") =0;

  virtual void setMassAsCriterion(bool active = true) = 0;
  virtual void setNbCellsAsCriterion(bool active = true) = 0;
  virtual void setCellCommContrib(bool active = true) = 0;
  virtual void setComputeComm(bool active = true) = 0;
  virtual const VariableFaceReal& commCost() const = 0;
  virtual const VariableCellReal& massWeight() const = 0;
  virtual const VariableCellReal& massResWeight() const = 0;
  virtual const VariableCellArrayReal& mCriteriaWeight() const = 0;

  virtual bool cellCommContrib() const = 0;
  virtual Integer nbCriteria() = 0;

  virtual void reset() =0;
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

} // End namespace Arcane

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#endif
