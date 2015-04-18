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
#include <string>

// Reaktoro includes
#include <Reaktoro/Common/Matrix.hpp>

namespace Reaktoro {

// Forward declarations
class ChemicalSystem;

/// Provides a computational representation of the state of a multiphase chemical system.
/// The chemical state of a multiphase system is defined by its temperature @f$(T)@f$,
/// pressure @f$(P)@f$, and molar composition @f$(\mathbf{n})@f$.
///
/// **Usage**
///
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/// // Create a ChemicalState instance, where system is a ChemicalSystem instance
/// ChemicalState state(system);
///
/// // Set the temperature and pressure states
/// state.setTemperature(60.0, "celsius");
/// state.setPressure(  180.0, "bar");
///
/// // Set the amount of some species
/// state.set( "H2O(l)",  1.0, "kg");
/// state.set(    "Na+",  1.0, "mol");
/// state.set(    "Cl-",  1.0, "mol");
/// state.set("CO2(aq)",  0.5, "mol");
/// state.set("Calcite", 10.0, "g");
///
/// // Output the chemical state instance
/// std::cout << state << std::endl;
/// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/// @see ChemicalSystem
/// @ingroup Core
class ChemicalState
{
public:
    /// Construct a default ChemicalState instance
    ChemicalState();

    /// Construct a ChemicalState instance
    /// @param system The chemical system instance
    explicit ChemicalState(const ChemicalSystem& system);

    /// Construct a copy of a ChemicalState instance
    ChemicalState(const ChemicalState& other);

    /// Destroy the instance
    virtual ~ChemicalState();

    /// Assign a ChemicalState instance to this instance
    auto operator=(ChemicalState other) -> ChemicalState&;

    /// Set the temperature of the chemical state (in units of K)
    auto setTemperature(double val) -> void;

    /// Set the temperature of the chemical state with given units
    auto setTemperature(double val, std::string units) -> void;

    /// Set the pressure of the chemical state (in units of Pa)
    auto setPressure(double val) -> void;

    /// Set the pressure of the chemical state with given units
    auto setPressure(double val, std::string units) -> void;

    /// Set the molar amounts of the species with a single value (in units of mol)
    /// @param val The single molar amounts of the species
    auto setSpeciesAmounts(double val) -> void;

    /// Set the molar amounts of the species (in units of mol)
    /// @param n The vector of molar amounts of the species
    auto setSpeciesAmounts(const Vector& n) -> void;

    /// Set the molar amounts of species given by their indices (in units of mol)
    /// @param n The vector of molar amounts of the species
    /// @param indices The indices of the species to be set
    auto setSpeciesAmounts(const Vector& n, const Indices& indices) -> void;

    /// Set the molar amount of a species (in units of mol)
    /// @param index The index of the species
    /// @param amount The molar amount of the species
    auto setSpeciesAmount(Index index, double amount) -> void;

    /// Set the molar amount of a species (in units of mol)
    /// @param name The name of the species
    /// @param amount The amount of the species
    auto setSpeciesAmount(std::string name, double amount) -> void;

    /// Set the amount of a species with given units
    /// @param index The index of the species
    /// @param amount The amount of the species
    /// @param units The units of the amount (must be convertible to either mol or gram)
    auto setSpeciesAmount(Index index, double amount, std::string units) -> void;

    /// Set the amount of a species with given units
    /// @param name The name of the species
    /// @param amount The amount of the species
    /// @param units The units of the amount (must be convertible to either mol or gram)
    auto setSpeciesAmount(std::string name, double amount, std::string units) -> void;

    /// Set dual potentials of the elements (in units of J/mol)
    /// @param y The Lagrange multipliers with respect to the equilibrium mass balance constraints
    auto setElementPotentials(const Vector& y) -> void;

    /// Set dual potentials of the species (in units of J/mol)
    /// @param z The Lagrange multipliers with respect to the equilibrium bound constraints (in units of J/mol)
    auto setSpeciesPotentials(const Vector& z) -> void;

    /// Set the volume of the chemical system by adjusting the molar amounts of all species equally.
    /// @param volume The volume of the chemical system (in units of m3)
    auto setVolume(double volume) -> void;

    /// Set the volume of a phase by adjusting the molar amounts of its species.
    /// @param index The index of the phase
    /// @param volume The volume of the phase (in units of m3)
    auto setPhaseVolume(Index index, double volume) -> void;

    /// Set the volume of a phase by adjusting the molar amounts of its species.
    /// @param name The name of the phase
    /// @param volume The volume of the phase (in units of m3)
    auto setPhaseVolume(std::string name, double volume) -> void;

    /// Scale the molar amounts of the species by a given scalar.
    /// @param scalar The scale factor of the molar amounts
    auto scaleSpeciesAmounts(double scalar) -> void;

    /// Scale the molar amounts of the species in a phase by a given scalar.
    /// @param index The index of the phase
    /// @param scalar The scale factor of the molar amounts
    auto scaleSpeciesAmountsInPhase(Index index, double scalar) -> void;

    /// Return the chemical system instance
    auto system() const -> const ChemicalSystem&;

    /// Return the temperature of the chemical state (in units of K)
    auto temperature() const -> double;

    /// Return the pressure of the chemical state (in units of Pa)
    auto pressure() const -> double;

    /// Return the molar amounts of the chemical species (in units of mol)
    auto speciesAmounts() const -> const Vector&;

    /// Return the dual potentials of the elements (in units of J/mol)
    auto elementPotentials() const -> const Vector&;

    /// Return the dual potentials of the species (in units of J/mol)
    auto speciesPotentials() const -> const Vector&;

    /// Return the molar amount of a chemical species (in units of mol)
    /// @param index The index of the species
    auto speciesAmount(Index index) const -> double;

    /// Return the molar amount of a chemical species (in units of mol)
    /// @param name The name of the species
    auto speciesAmount(std::string name) const -> double;

    /// Return the amount of a chemical species with given units
    /// @param index The index of the species
    /// @param units The units of the species amount
    auto speciesAmount(Index index, std::string units) const -> double;

    /// Return the amount of a chemical species with given units
    /// @param name The name of the species
    /// @param units The units of the species amount
    auto speciesAmount(std::string name, std::string units) const -> double;

    /// Return the molar amounts of the elements (in units of mol)
    auto elementAmounts() const -> Vector;

    /// Return the molar amounts of the elements in a phase (in units of mol)
    /// @param index The index of the phase
    auto elementAmountsInPhase(Index index) const -> Vector;

    /// Return the molar amounts of the elements in a set of species (in units of mol)
    /// @param indices The indices of the species
    auto elementAmountsInSpecies(const Indices& indices) const -> Vector;

    /// Return the molar amount of an element (in units of mol)
    /// @param index The index of the element
    auto elementAmount(Index index) const -> double;

    /// Return the molar amount of an element (in units of mol)
    /// @param name The name of the element
    auto elementAmount(std::string name) const -> double;

    /// Return the amount of an element with given units
    /// @param index The index of the element
    /// @param units The units of the element amount
    auto elementAmount(Index index, std::string units) const -> double;

    /// Return the amount of an element with given units
    /// @param name The name of the element
    /// @param units The units of the element amount
    auto elementAmount(std::string name, std::string units) const -> double;

    /// Return the molar amount of an element in a given phase (in units of mol)
    /// @param ielement The index of the element
    /// @param iphase The index of the phase
    auto elementAmountInPhase(Index ielement, Index iphase) const -> double;

    /// Return the molar amount of an element in a given phase (in units of mol)
    /// @param element The name of the element
    /// @param phase The name of the phase
    auto elementAmountInPhase(std::string element, std::string phase) const -> double;

    /// Return the amount of an element in a given phase with given units
    /// @param ielement The index of the element
    /// @param iphase The index of the phase
    /// @param units The units of the element amount
    auto elementAmountInPhase(Index ielement, Index iphase, std::string units) const -> double;

    /// Return the amount of an element in a given phase with given units
    /// @param element The name of the element
    /// @param phase The name of the phase
    /// @param units The units of the element amount
    auto elementAmountInPhase(std::string element, std::string phase, std::string units) const -> double;

    /// Return the molar amount of an element in a set of species (in units of mol)
    /// @param ielement The index of the element
    /// @param ispecies The indices of the species
    auto elementAmountInSpecies(Index ielement, const Indices& ispecies) const -> double;

    /// Return the amount of an element in a set of species with given units
    /// @param ielement The index of the element
    /// @param ispecies The indices of the species
    /// @param units The units of the element amount
    auto elementAmountInSpecies(Index ielement, const Indices& ispecies, std::string units) const -> double;

private:
    struct Impl;

    std::unique_ptr<Impl> pimpl;
};

/// Outputs a ChemicalState instance.
auto operator<<(std::ostream& out, const ChemicalState& state) -> std::ostream&;

/// Add two ChemicalState instances.
auto operator+(const ChemicalState& l, const ChemicalState& r) -> ChemicalState;

/// Multiply a ChemicalState instance by a scalar (from the left).
auto operator*(double scalar, const ChemicalState& state) -> ChemicalState;

/// Multiply a ChemicalState instance by a scalar (from the right).
auto operator*(const ChemicalState& state, double scalar) -> ChemicalState;

/// Return the value of a quantity in a chemical state.
/// This method requires a string representing the quantity of interest. The options are:
///
/// | String           | Quantity                                | Example                  |
/// |:----------------:|:---------------------------------------:|:------------------------:|
/// | `n[`*species*`]` | the molar amount of species named *species* | `n[H2O(l)]`, `n[CO2(g)]` |
/// | `b[`*element*`]` | the molar amount of element named *element* | `b[H]`, `b[C]`, `b[O]`   |
/// | `b[`*element*`][`*phase*`]` | the molar amount of element named *element* in a phase named *phase* | `b[Na][Aqueous]`, `b[C][Gaseous]`|
/// | `pH` | the pH of the aqueous phase | `pH` |
/// | `a[`*species*`]` | the activity of the species named *species* | `a[H+]`, `a[CO2(aq)]` |
/// | `g[`*species*`]` | the activity coefficient of the species named *species* | `g[H+]`, `g[CO2(aq)]` |
/// | `m[`*species*`]` | the molality of the aqueous species named *species* | `m[Na+]`, `m[HCO3-]` |
///
/// The above strings can be combined with the units of the extracted quantity. For example, the molar amount of species `H+` can be extracted in `mmol` units as `n[H+]:mmol`.
/// Note that extracting the pH of the aqeous phase and the molality of one of its species will only succeed as long as the aqueous phase is called `Aqueous`.
/// @param state The chemical state
/// @param str The string representing the quantity
auto extract(const ChemicalState& state, std::string quantity) -> double;

} // namespace Reaktoro