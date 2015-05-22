// Reaktoro is a C++ library for computational reaction modelling.
//
// Copyright (C) 2014 Allan Leal
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#pragma once

// C++ includes
#include <memory>

namespace Reaktoro {

// Forward declarations
struct OptimumOptions;
class  OptimumProblem;
struct OptimumState;
struct OptimumResult;

struct SimplexState
{
    Vector x, y, zl, zu;

    Indices ibasic, ilower, iupper;
};

class OptimumSolverSimplex
{
public:
    /// Construct a default OptimumSolverSimplex instance
    OptimumSolverSimplex();

    /// Construct a copy of an OptimumSolverSimplex instance
    OptimumSolverSimplex(const OptimumSolverSimplex& other);

    /// Destroy this OptimumSolverSimplex instance
    virtual ~OptimumSolverSimplex();

    /// Assign a copy of an OptimumSolverSimplex instance
    auto operator=(OptimumSolverSimplex other) -> OptimumSolverSimplex&;

    /// Find an initial guess for an optimisation problem
    /// @param problem The definition of the optimisation problem
    /// @param state[in,out] The initial guess and the final state of the equilibrium approximation
    /// @param options The options for the optimisation calculation
    auto feasible(const OptimumProblem& problem, OptimumState& state) -> OptimumResult;

    /// Find an initial guess for an optimisation problem with given options
    /// @param problem The definition of the optimisation problem
    /// @param state[in,out] The initial guess and the final state of the equilibrium approximation
    /// @param options The options for the optimisation calculation
    auto simplex(const OptimumProblem& problem, OptimumState& state) -> OptimumResult;

    /// Find an initial guess for an optimisation problem with given options
    /// @param problem The definition of the optimisation problem
    /// @param state[in,out] The initial guess and the final state of the equilibrium approximation
    /// @param options The options for the optimisation calculation
    auto solve(const OptimumProblem& problem, OptimumState& state) -> OptimumResult;

private:
    struct Impl;

    std::unique_ptr<Impl> pimpl;
};

} // namespace Reaktoro