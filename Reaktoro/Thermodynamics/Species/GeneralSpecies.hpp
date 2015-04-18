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
#include <map>
#include <memory>
#include <string>

namespace Reaktoro {

// Forward declarations
class Element;

/// A type used to describe a chemical species and its attributes.
/// The GeneralSpecies class is used to represent a chemical species. It is an important
/// class in the library, since it defines fundamental attributes of a general
/// chemical species such as its elemental formula, electrical charge and molar mass.
/// @see Phase
/// @ingroup Core
class GeneralSpecies
{
public:
    /// Construct a default GeneralSpecies instance.
    GeneralSpecies();

    /// Construct a copy of an GeneralSpecies instance
    GeneralSpecies(const GeneralSpecies& other);

    /// Destroy this instance
    virtual ~GeneralSpecies();

    /// Assign an GeneralSpecies instance to this instance
    auto operator=(GeneralSpecies other) -> GeneralSpecies&;

    /// Set the name of the species.
    auto setName(std::string name) -> void;

    /// Set the formula of the species.
    auto setFormula(std::string formula) -> void;

    /// Set the elements of the species.
    auto setElements(const std::map<Element, double>& elements) -> void;

    /// Set the molar mass of the species (in units of kg/mol).
    auto setMolarMass(double value) -> void;

    /// Return the number of elements of the chemical species
    auto numElements() const -> unsigned;

    /// Return the name of the chemical species
    auto name() const -> const std::string&;

    /// Return the formula of the chemical species
    auto formula() const -> const std::string&;

    /// Return the elements that compose the chemical species and their coefficients
    auto elements() const -> const std::map<Element, double>&;

    /// Return the molar mass of the chemical species (in units of kg/mol)
    auto molarMass() const -> double;

    /// Return the number of atoms of an element in the chemical species.
    auto elementCoefficient(std::string element) const -> double;

private:
    struct Impl;

    std::unique_ptr<Impl> pimpl;
};

/// Compare two GeneralSpecies instances for less than
auto operator<(const GeneralSpecies& lhs, const GeneralSpecies& rhs) -> bool;

/// Compare two GeneralSpecies instances for equality
auto operator==(const GeneralSpecies& lhs, const GeneralSpecies& rhs) -> bool;

} // namespace Reaktoro