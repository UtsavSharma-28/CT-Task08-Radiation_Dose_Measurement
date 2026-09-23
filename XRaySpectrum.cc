#include "XRaySpectrum.hh"

#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

XRaySpectrum::XRaySpectrum()
{
}

G4double XRaySpectrum::SampleEnergy()
{
    // -----------------------------------------
    // Simplified 120 kVp spectrum
    // -----------------------------------------

    const G4double energies[] =
    {
        20, 25, 30, 35, 40,
        45, 50, 55, 60, 65,
        70, 75, 80, 85, 90,
        95, 100, 105, 110, 115, 120
    };

    const G4double weights[] =
    {
        0.02, 0.08, 0.18, 0.32, 0.48,
        0.62, 0.75, 0.86, 0.95, 1.00,
        0.98, 0.92, 0.84, 0.75, 0.65,
        0.55, 0.44, 0.32, 0.20, 0.10, 0.03
    };

    const G4int nBins = 21;

    // -----------------------------------------
    // Simplified 2 mm aluminum filtration
    //
    // Low-energy photons are more strongly
    // attenuated than high-energy photons.
    // -----------------------------------------

    const G4double transmission[] =
    {
        0.01, 0.03, 0.08, 0.16, 0.27,
        0.39, 0.50, 0.60, 0.68, 0.75,
        0.80, 0.84, 0.87, 0.89, 0.91,
        0.93, 0.94, 0.95, 0.96, 0.97, 0.98
    };

    // -----------------------------------------
    // Combine spectrum with filtration
    // -----------------------------------------

    G4double filteredWeights[21];

    G4double totalWeight = 0.0;

    for (G4int i = 0; i < nBins; i++)
    {
        filteredWeights[i] =
            weights[i] * transmission[i];

        totalWeight += filteredWeights[i];
    }

    // -----------------------------------------
    // Random sampling from filtered spectrum
    // -----------------------------------------

    G4double randomValue =
        G4UniformRand() * totalWeight;

    G4double cumulative = 0.0;

    for (G4int i = 0; i < nBins; i++)
    {
        cumulative += filteredWeights[i];

        if (randomValue <= cumulative)
        {
            return energies[i] * keV;
        }
    }

    return 70.0 * keV;
}