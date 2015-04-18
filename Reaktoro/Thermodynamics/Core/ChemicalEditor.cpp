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

#include "ChemicalEditor.hpp"

// Reaktoro includes
#include <Reaktoro/Common/InterpolationUtils.hpp>
#include <Reaktoro/Common/StringUtils.hpp>
#include <Reaktoro/Common/Units.hpp>
#include <Reaktoro/Core/ChemicalSystem.hpp>
#include <Reaktoro/Core/Phase.hpp>
#include <Reaktoro/Core/ReactionSystem.hpp>
#include <Reaktoro/Core/Species.hpp>
#include <Reaktoro/Thermodynamics/Core/Database.hpp>
#include <Reaktoro/Thermodynamics/Core/Thermo.hpp>
#include <Reaktoro/Thermodynamics/Phases/AqueousPhase.hpp>
#include <Reaktoro/Thermodynamics/Phases/GaseousPhase.hpp>
#include <Reaktoro/Thermodynamics/Phases/MineralPhase.hpp>
#include <Reaktoro/Thermodynamics/Reactions/MineralReaction.hpp>

namespace Reaktoro {

struct ChemicalEditor::Impl
{
private:
    /// The database instance
    Database database;

    /// The current state of the aqueous phase instance
    AqueousPhase aqueous_phase;

    /// The current state of the gaseous phase instance
    GaseousPhase gaseous_phase;

    /// The current state of the mineral phase instance
    std::vector<MineralPhase> mineral_phases;

    /// The mineral reactions of the chemical system
    std::vector<MineralReaction> mineral_reactions;

    /// The temperatures for constructing interpolation tables of thermodynamic properties (in units of K).
    std::vector<double> temperatures;

    /// The pressures for constructing interpolation tables of thermodynamic properties (in units of Pa).
    std::vector<double> pressures;

public:
    explicit Impl(const Database& database)
    : database(database)
    {
        // The default temperatures for the interpolation of the thermodynamic properties (in units of celsius)
        temperatures = { 0, 25, 50, 75, 100, 125, 150, 175, 200, 225, 250, 275, 300 };

        // The default pressures for the interpolation of the thermodynamic properties (in units of bar)
        pressures = { 1, 25, 50, 100, 150, 200, 250, 300, 350, 400, 450, 500, 550, 600 };

        // Convert the temperatures and pressures to units of kelvin and pascal respectively
        for(auto& x : temperatures) x = x + 273.15;
        for(auto& x : pressures)    x = x * 1.0e+5;
    }

    auto setTemperatures(std::vector<double> values, std::string units) -> void
    {
        temperatures = values;
        for(auto& x : temperatures) x = units::convert(x, units, "celsius");
    }

    auto setPressures(std::vector<double> values, std::string units) -> void
    {
        pressures = values;
        for(auto& x : pressures) x = units::convert(x, units, "pascal");
    }

    auto addPhase(const AqueousPhase& phase) -> AqueousPhase&
    {
        aqueous_phase = phase;
        aqueous_phase.setName("Aqueous");
        return aqueous_phase;
    }

    auto addPhase(const GaseousPhase& phase) -> GaseousPhase&
    {
        gaseous_phase = phase;
        gaseous_phase.setName("Gaseous");
        return gaseous_phase;
    }

    auto addPhase(const MineralPhase& phase) -> MineralPhase&
    {
        mineral_phases.push_back(phase);
        mineral_phases.back().setName("Mineral#" + std::to_string(mineral_phases.size()));
        return mineral_phases.back();
    }

    auto addAqueousPhase(const std::vector<std::string>& species) -> AqueousPhase&
    {
        // Collect the aqueous species instances from the database
        std::vector<AqueousSpecies> aqueous_species(species.size());
        for(unsigned i = 0; i < species.size(); ++i)
            aqueous_species[i] = database.aqueousSpecies(species[i]);

        // Create the aqueous phase
        aqueous_phase = AqueousPhase(aqueous_species);
        aqueous_phase.setName("Aqueous");

        // Set the default activity models
        aqueous_phase.setActivityModelHKFWater();
        aqueous_phase.setActivityModelHKFChargedSpecies();
        aqueous_phase.setActivityModelDuanSunCO2();

        return aqueous_phase;
    }

    auto addAqueousPhase(const std::string& species) -> AqueousPhase&
    {
        std::vector<std::string> words = split(species, " ");

        return addAqueousPhase(words);
    }

    auto addGaseousPhase(const std::vector<std::string>& species) -> GaseousPhase&
    {
        std::vector<GaseousSpecies> gaseous_species(species.size());

        for(unsigned i = 0; i < species.size(); ++i)
            gaseous_species[i] = database.gaseousSpecies(species[i]);

        gaseous_phase = GaseousPhase(gaseous_species);
        gaseous_phase.setName("Gaseous");

        gaseous_phase.setActivityModelDuanSunCO2();
        gaseous_phase.setActivityModelIdeal("H2O(g)");

        return gaseous_phase;
    }

    auto addGaseousPhase(const std::string& species) -> GaseousPhase&
    {
        std::vector<std::string> words = split(species, " ");

        return addGaseousPhase(words);
    }

    auto addMineralPhase(const std::vector<std::string>& species) -> MineralPhase&
    {
        std::vector<MineralSpecies> mineral_species(species.size());
        for(unsigned i = 0; i < species.size(); ++i)
            mineral_species[i] = database.mineralSpecies(species[i]);

        mineral_phases.push_back(MineralPhase(mineral_species));
        mineral_phases.back().setName("Mineral#" + std::to_string(mineral_phases.size()));

        return mineral_phases.back();
    }

    auto addMineralPhase(const std::string& species) -> MineralPhase&
    {
        std::vector<std::string> words = split(species, " ");

        return addMineralPhase(words);
    }

    auto addReaction(const MineralReaction& reaction) -> MineralReaction&
    {
        mineral_reactions.push_back(reaction);
        return mineral_reactions.back();
    }

    auto addMineralReaction(const std::string& mineral) -> MineralReaction&
    {
        mineral_reactions.push_back(MineralReaction(mineral));
        return mineral_reactions.back();
    }

    auto aqueousPhase() const -> const AqueousPhase&
    {
        return aqueous_phase;
    }

    auto aqueousPhase() -> AqueousPhase&
    {
        return aqueous_phase;
    }

    auto gaseousPhase() const -> const GaseousPhase&
    {
        return gaseous_phase;
    }

    auto gaseousPhase() -> GaseousPhase&
    {
        return gaseous_phase;
    }

    auto mineralPhases() const -> const std::vector<MineralPhase>&
    {
        return mineral_phases;
    }

    auto mineralPhases() -> std::vector<MineralPhase>&
    {
        return mineral_phases;
    }

    template<typename SpeciesType>
    auto convertSpecies(const SpeciesType& species) const -> Species
    {
        Species converted;
        converted.setName(species.name());
        converted.setFormula(species.formula());
        converted.setElements(species.elements());
        converted.setMolarMass(species.molarMass());

        Thermo thermo(database);

        auto standard_gibbs_energy_fn = [&](double T, double P)
        {
            return thermo.standardGibbsEnergy(T, P, species.name());
        };

        auto standard_helmholtz_energy_fn = [&](double T, double P)
        {
            return thermo.standardHelmholtzEnergy(T, P, species.name());
        };

        auto standard_internal_energy_fn = [&](double T, double P)
        {
            return thermo.standardInternalEnergy(T, P, species.name());
        };

        auto standard_enthalpy_fn = [&](double T, double P)
        {
            return thermo.standardEnthalpy(T, P, species.name());
        };

        auto standard_entropy_fn = [&](double T, double P)
        {
            return thermo.standardEntropy(T, P, species.name());
        };

        auto standard_volume_fn = [&](double T, double P)
        {
            return thermo.standardVolume(T, P, species.name());
        };

        auto standard_heat_capacity_fn = [&](double T, double P)
        {
            return thermo.standardHeatCapacity(T, P, species.name());
        };

        if(thermo.checkStandardGibbsEnergy(species.name()))
            converted.setStandardGibbsEnergyFunction(interpolate(temperatures, pressures, standard_gibbs_energy_fn));
        if(thermo.checkStandardHelmholtzEnergy(species.name()))
            converted.setStandardHelmholtzEnergyFunction(interpolate(temperatures, pressures, standard_helmholtz_energy_fn));
        if(thermo.checkStandardInternalEnergy(species.name()))
            converted.setStandardInternalEnergyFunction(interpolate(temperatures, pressures, standard_internal_energy_fn));
        if(thermo.checkStandardEnthalpy(species.name()))
            converted.setStandardEnthalpyFunction(interpolate(temperatures, pressures, standard_enthalpy_fn));
        if(thermo.checkStandardEntropy(species.name()))
            converted.setStandardEntropyFunction(interpolate(temperatures, pressures, standard_entropy_fn));
        if(thermo.checkStandardVolume(species.name()))
            converted.setStandardVolumeFunction(interpolate(temperatures, pressures, standard_volume_fn));
        if(thermo.checkStandardHeatCapacity(species.name()))
            converted.setStandardHeatCapacityFunction(interpolate(temperatures, pressures, standard_heat_capacity_fn));

        return converted;
    }

    template<typename PhaseType>
    auto convertPhase(const PhaseType& phase) const -> Phase
    {
        std::vector<Species> species;
        species.reserve(phase.numSpecies());
        for(const auto& s : phase.species())
            species.push_back(convertSpecies(s));

        std::shared_ptr<PhaseType> phase_ptr(new PhaseType(phase));

        auto concentration_fn = [=](double T, double P, const Vector& n)
        {
            return phase_ptr->concentrations(T, P, n);
        };

        auto activity_coeff_fn = [=](double T, double P, const Vector& n)
        {
            return phase_ptr->activityCoefficients(T, P, n);
        };

        auto activity_fn = [=](double T, double P, const Vector& n)
        {
            return phase_ptr->activities(T, P, n);
        };

        Phase converted;
        converted.setName(phase.name());
        converted.setSpecies(species);
        converted.setConcentractionFunction(concentration_fn);
        converted.setActivityCoefficientFunction(activity_coeff_fn);
        converted.setActivityFunction(activity_fn);

        return converted;
    }

    auto createChemicalSystem() const -> ChemicalSystem
    {
        std::vector<Phase> phases;
        phases.reserve(2 + mineral_phases.size());

        if(aqueous_phase.numSpecies())
            phases.push_back(convertPhase(aqueous_phase));

        if(gaseous_phase.numSpecies())
            phases.push_back(convertPhase(gaseous_phase));

        for(const MineralPhase& mineral_phase : mineral_phases)
            phases.push_back(convertPhase(mineral_phase));

        return ChemicalSystem(phases);
    }

    auto createReactionSystem() const -> ReactionSystem
    {
        ChemicalSystem system = createChemicalSystem();

        std::vector<Reaction> reactions;
        for(const MineralReaction& rxn : mineral_reactions)
            reactions.push_back(createReaction(rxn, system));

        return ReactionSystem(reactions);
    }
};

ChemicalEditor::ChemicalEditor(const Database& database)
: pimpl(new Impl(database))
{}

ChemicalEditor::ChemicalEditor(const ChemicalEditor& other)
: pimpl(new Impl(*other.pimpl))
{}

ChemicalEditor::~ChemicalEditor()
{}

auto ChemicalEditor::operator=(const ChemicalEditor& other) -> ChemicalEditor&
{
    pimpl.reset(new Impl(*other.pimpl));
    return *this;
}

auto ChemicalEditor::setTemperatures(std::vector<double> values, std::string units) -> void
{
    pimpl->setTemperatures(values, units);
}

auto ChemicalEditor::setPressures(std::vector<double> values, std::string units) -> void
{
    pimpl->setPressures(values, units);
}

auto ChemicalEditor::addPhase(const AqueousPhase& phase) -> AqueousPhase&
{
    return pimpl->addPhase(phase);
}

auto ChemicalEditor::addPhase(const GaseousPhase& phase) -> GaseousPhase&
{
    return pimpl->addPhase(phase);
}

auto ChemicalEditor::addPhase(const MineralPhase& phase) -> MineralPhase&
{
    return pimpl->addPhase(phase);
}

auto ChemicalEditor::addReaction(const MineralReaction& reaction) -> MineralReaction&
{
    return pimpl->addReaction(reaction);
}

auto ChemicalEditor::addAqueousPhase(const std::vector<std::string>& species) -> AqueousPhase&
{
    return pimpl->addAqueousPhase(species);
}

auto ChemicalEditor::addAqueousPhase(const std::string& species) -> AqueousPhase&
{
    return pimpl->addAqueousPhase(species);
}

auto ChemicalEditor::addGaseousPhase(const std::vector<std::string>& species) -> GaseousPhase&
{
    return pimpl->addGaseousPhase(species);
}

auto ChemicalEditor::addGaseousPhase(const std::string& species) -> GaseousPhase&
{
    return pimpl->addGaseousPhase(species);
}

auto ChemicalEditor::addMineralPhase(const std::vector<std::string>& species) -> MineralPhase&
{
    return pimpl->addMineralPhase(species);
}

auto ChemicalEditor::addMineralPhase(const std::string& species) -> MineralPhase&
{
    return pimpl->addMineralPhase(species);
}

auto ChemicalEditor::addMineralReaction(const std::string& mineral) -> MineralReaction&
{
    return pimpl->addMineralReaction(mineral);
}

auto ChemicalEditor::aqueousPhase() const -> const AqueousPhase&
{
    return pimpl->aqueousPhase();
}

auto ChemicalEditor::aqueousPhase() -> AqueousPhase&
{
    return pimpl->aqueousPhase();
}

auto ChemicalEditor::gaseousPhase() const -> const GaseousPhase&
{
    return pimpl->gaseousPhase();
}

auto ChemicalEditor::gaseousPhase() -> GaseousPhase&
{
    return pimpl->gaseousPhase();
}

auto ChemicalEditor::mineralPhases() const -> const std::vector<MineralPhase>&
{
    return pimpl->mineralPhases();
}

auto ChemicalEditor::mineralPhases() -> std::vector<MineralPhase>&
{
    return pimpl->mineralPhases();
}

auto ChemicalEditor::createChemicalSystem() const -> ChemicalSystem
{
    return pimpl->createChemicalSystem();
}

auto ChemicalEditor::createReactionSystem() const -> ReactionSystem
{
    return pimpl->createReactionSystem();
}

ChemicalEditor::operator ChemicalSystem() const
{
    return createChemicalSystem();
}

ChemicalEditor::operator ReactionSystem() const
{
    return createReactionSystem();
}

} // namespace Reaktoro