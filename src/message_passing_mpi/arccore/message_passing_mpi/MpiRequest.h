﻿// -*- tab-width: 2; indent-tabs-mode: nil; coding: utf-8-with-signature -*-
/*---------------------------------------------------------------------------*/
/* MpiRequest.h                                                (C) 2000-2020 */
/*                                                                           */
/* Spécialisation de 'Request' pour MPI.                                     */
/*---------------------------------------------------------------------------*/
#ifndef ARCCORE_MESSAGEPASSINGMPI_MPIREQUEST_H
#define ARCCORE_MESSAGEPASSINGMPI_MPIREQUEST_H
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#include "arccore/message_passing_mpi/MessagePassingMpiGlobal.h"

#include "arccore/message_passing/Request.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Arccore::MessagePassing::Mpi
{
/*!
 * \brief Spécialisation MPI d'une 'Request'.
 *
 * Cette classe permet de garantir qu'une requête MPI est bien construite
 * à partir d'une MPI_Request.
 */
class ARCCORE_MESSAGEPASSINGMPI_EXPORT MpiRequest
: public Request
{
 public:

  MpiRequest() = default;
  MpiRequest(int ret_value,MPI_Request mpi_request)
  : Request(ret_value,mpi_request){}
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

} // End namespace Arccore::MessagePassing

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

#endif  

