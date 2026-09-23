#include "DoseScorer.hh"

#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4TouchableHistory.hh"
#include "G4SystemOfUnits.hh"

#include <algorithm>

DoseScorer::DoseScorer(const G4String& name)
    : G4VSensitiveDetector(name),
    fEnergyDeposit(0.0),
    fVoxelEnergy(fNX* fNY* fNZ, 0.0)
{
}

DoseScorer::~DoseScorer()
{
}

void DoseScorer::Reset()
{
    // Reset total energy for a new run
    fEnergyDeposit = 0.0;

    // Reset all voxel energies for a new run
    std::fill(
        fVoxelEnergy.begin(),
        fVoxelEnergy.end(),
        0.0
    );
}

void DoseScorer::Initialize(G4HCofThisEvent*)
{
    // IMPORTANT:
    // Do NOT reset fEnergyDeposit or fVoxelEnergy here.
    //
    // Initialize() is called for every event.
    // We want to accumulate dose over the complete run.
}

G4bool DoseScorer::ProcessHits(
    G4Step* step,
    G4TouchableHistory*
)
{
    G4double energyDeposit =
        step->GetTotalEnergyDeposit();

    if (energyDeposit <= 0.0)
    {
        return false;
    }

    // -----------------------------------------
    // Total energy deposited during the run
    // -----------------------------------------

    fEnergyDeposit += energyDeposit;

    // -----------------------------------------
    // Get local position inside phantom
    // -----------------------------------------

    auto touchable =
        step->GetPreStepPoint()->GetTouchable();

    G4ThreeVector localPosition =
        touchable->GetHistory()
        ->GetTopTransform()
        .TransformPoint(
            step->GetPreStepPoint()->GetPosition()
        );

    // -----------------------------------------
    // Phantom dimensions
    // -----------------------------------------

    G4double phantomSize = 30.0 * cm;

    G4double voxelSize =
        phantomSize / fNX;

    // -----------------------------------------
    // Convert position to voxel coordinates
    // -----------------------------------------

    G4int x =
        static_cast<G4int>(
            (localPosition.x()
                + phantomSize / 2.0)
            / voxelSize
            );

    G4int y =
        static_cast<G4int>(
            (localPosition.y()
                + phantomSize / 2.0)
            / voxelSize
            );

    G4int z =
        static_cast<G4int>(
            (localPosition.z()
                + phantomSize / 2.0)
            / voxelSize
            );

    // -----------------------------------------
 // Add energy to voxel
 // -----------------------------------------

 // Keep boundary positions inside the last voxel
    if (x == fNX) x = fNX - 1;
    if (y == fNY) y = fNY - 1;
    if (z == fNZ) z = fNZ - 1;

    if (x >= 0 && x < fNX &&
        y >= 0 && y < fNY &&
        z >= 0 && z < fNZ)
    {
        G4int index =
            GetIndex(x, y, z);

        fVoxelEnergy[index] +=
            energyDeposit;
    }

    return true;
}

G4int DoseScorer::GetIndex(
    G4int x,
    G4int y,
    G4int z
) const
{
    return x
        + fNX * (y + fNY * z);
}

G4double DoseScorer::GetEnergyDeposit() const
{
    return fEnergyDeposit;
}

G4double DoseScorer::GetVoxelEnergy(
    G4int x,
    G4int y,
    G4int z
) const
{
    if (x < 0 || x >= fNX ||
        y < 0 || y >= fNY ||
        z < 0 || z >= fNZ)
    {
        return 0.0;
    }

    return fVoxelEnergy[
        GetIndex(x, y, z)
    ];
}