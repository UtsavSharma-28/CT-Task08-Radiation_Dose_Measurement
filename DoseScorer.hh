#ifndef DoseScorer_h
#define DoseScorer_h

#include "G4VSensitiveDetector.hh"
#include "globals.hh"

#include <vector>

class G4Step;

class DoseScorer : public G4VSensitiveDetector
{
public:
    DoseScorer(const G4String& name);
    ~DoseScorer() override;

    void Initialize(G4HCofThisEvent*) override;

    G4bool ProcessHits(
        G4Step* step,
        G4TouchableHistory*
    ) override;

    G4double GetEnergyDeposit() const;

    // Get energy deposited in a particular voxel
    G4double GetVoxelEnergy(
        G4int x,
        G4int y,
        G4int z
    ) const;
    void Reset();
private:

    // Number of voxels
    static const G4int fNX = 10;
    static const G4int fNY = 10;
    static const G4int fNZ = 10;

    // Total energy deposited
    G4double fEnergyDeposit;

    // Energy deposited in each voxel
    std::vector<G4double> fVoxelEnergy;

    // Convert 3D voxel coordinates to 1D index
    G4int GetIndex(
        G4int x,
        G4int y,
        G4int z
    ) const;
};

#endif