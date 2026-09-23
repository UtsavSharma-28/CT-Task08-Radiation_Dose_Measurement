#include "RunAction.hh"

#include "DoseScorer.hh"

#include "G4Run.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

#include <fstream>
#include <iomanip>
#include <cfloat>

// =========================================================
// CONSTRUCTOR
// =========================================================

RunAction::RunAction()
    : fTotalEnergyDeposit(0.0),
    fAbsorbedPhotons(0),
    fScatteredPhotons(0),
    fTransmittedPhotons(0),
    fUnclassifiedPhotons(0)
{
}

// =========================================================
// DESTRUCTOR
// =========================================================

RunAction::~RunAction()
{
}

// =========================================================
// BEGIN OF RUN
// =========================================================

void RunAction::BeginOfRunAction(
    const G4Run*
)
{
    fTotalEnergyDeposit = 0.0;

    fAbsorbedPhotons = 0;
    fScatteredPhotons = 0;
    fTransmittedPhotons = 0;
    fUnclassifiedPhotons = 0;

    // -----------------------------------------
    // Reset dose scorer
    // -----------------------------------------

    auto sensitiveDetector =
        G4SDManager::GetSDMpointer()
        ->FindSensitiveDetector("DoseScorer");

    auto doseScorer =
        dynamic_cast<DoseScorer*>(
            sensitiveDetector
            );

    if (doseScorer != nullptr)
    {
        doseScorer->Reset();
    }
}

// =========================================================
// ADD ENERGY
// =========================================================

void RunAction::AddEnergyDeposit(
    G4double energy
)
{
    fTotalEnergyDeposit += energy;
}

// =========================================================
// ADD ABSORBED PHOTON
// =========================================================

void RunAction::AddAbsorbedPhoton()
{
    fAbsorbedPhotons++;
}

// =========================================================
// ADD SCATTERED PHOTON
// =========================================================

void RunAction::AddScatteredPhoton()
{
    fScatteredPhotons++;
}

// =========================================================
// ADD TRANSMITTED PHOTON
// =========================================================

void RunAction::AddTransmittedPhoton()
{
    fTransmittedPhotons++;
}

// =========================================================
// ADD UNCLASSIFIED PHOTON
// =========================================================

void RunAction::AddUnclassifiedPhoton()
{
    fUnclassifiedPhotons++;
}

// =========================================================
// END OF RUN
// =========================================================

void RunAction::EndOfRunAction(
    const G4Run* run
)
{
    // =====================================================
    // PHANTOM
    // =====================================================

    G4double phantomVolume =
        30.0 * cm *
        30.0 * cm *
        30.0 * cm;

    G4double phantomMass =
        phantomVolume *
        (1.0 * g / cm3);

    // =====================================================
    // VOXEL
    // =====================================================

    G4double voxelSize = 3.0 * cm;

    G4double voxelVolume =
        voxelSize *
        voxelSize *
        voxelSize;

    G4double voxelMass =
        voxelVolume *
        (1.0 * g / cm3);

    // =====================================================
    // GET DOSE SCORER
    // =====================================================

    auto sensitiveDetector =
        G4SDManager::GetSDMpointer()
        ->FindSensitiveDetector("DoseScorer");

    auto doseScorer =
        dynamic_cast<DoseScorer*>(
            sensitiveDetector
            );

    G4double totalEnergy = 0.0;
    G4double voxelEnergySum = 0.0;

    G4double maximumVoxelEnergy = 0.0;
    G4double minimumVoxelEnergy = DBL_MAX;

    G4double maximumVoxelDose = 0.0;
    G4double minimumVoxelDose = DBL_MAX;

    G4double totalVoxelDose = 0.0;

    G4int maxX = 0;
    G4int maxY = 0;
    G4int maxZ = 0;

    G4int minX = 0;
    G4int minY = 0;
    G4int minZ = 0;

    G4int nonZeroVoxels = 0;

    // =====================================================
    // CREATE VOXEL DOSE CSV
    // =====================================================

    std::ofstream csvFile("voxel_dose.csv");

    csvFile << "X,Y,Z,Energy_MeV,Dose_Gy\n";

    if (doseScorer != nullptr)
    {
        for (G4int x = 0; x < 10; x++)
        {
            for (G4int y = 0; y < 10; y++)
            {
                for (G4int z = 0; z < 10; z++)
                {
                    G4double energy =
                        doseScorer->GetVoxelEnergy(
                            x, y, z
                        );

                    G4double voxelDose =
                        energy / voxelMass;

                    csvFile
                        << x << ","
                        << y << ","
                        << z << ","
                        << std::setprecision(10)
                        << energy / MeV << ","
                        << voxelDose / gray
                        << "\n";
                }
            }
        }
    }

    csvFile.close();

    G4cout << G4endl;

    G4cout
        << "Voxel dose CSV file created: "
        << "voxel_dose.csv"
        << G4endl;

    // =====================================================
    // VOXEL ANALYSIS
    // =====================================================

    if (doseScorer != nullptr)
    {
        totalEnergy =
            doseScorer->GetEnergyDeposit();

        for (G4int x = 0; x < 10; x++)
        {
            for (G4int y = 0; y < 10; y++)
            {
                for (G4int z = 0; z < 10; z++)
                {
                    G4double energy =
                        doseScorer->GetVoxelEnergy(
                            x, y, z
                        );

                    voxelEnergySum += energy;

                    if (energy > 0.0)
                    {
                        nonZeroVoxels++;

                        G4double voxelDose =
                            energy / voxelMass;

                        totalVoxelDose += voxelDose;

                        // ---------------------------------
                        // Maximum
                        // ---------------------------------

                        if (energy > maximumVoxelEnergy)
                        {
                            maximumVoxelEnergy = energy;

                            maximumVoxelDose =
                                voxelDose;

                            maxX = x;
                            maxY = y;
                            maxZ = z;
                        }

                        // ---------------------------------
                        // Minimum
                        // ---------------------------------

                        if (energy < minimumVoxelEnergy)
                        {
                            minimumVoxelEnergy = energy;

                            minimumVoxelDose =
                                voxelDose;

                            minX = x;
                            minY = y;
                            minZ = z;
                        }
                    }
                }
            }
        }
    }

    // =====================================================
    // MEAN VOXEL DOSE
    // =====================================================

    G4double meanVoxelDose = 0.0;

    if (nonZeroVoxels > 0)
    {
        meanVoxelDose =
            totalVoxelDose /
            nonZeroVoxels;
    }

    // =====================================================
    // TOTAL PHANTOM DOSE
    // =====================================================

    G4double dose =
        totalEnergy / phantomMass;

    // =====================================================
    // RESULTS
    // =====================================================

    G4cout << G4endl;

    G4cout
        << "================================"
        << G4endl;

    G4cout
        << "     CT DOSE SIMULATION RESULT"
        << G4endl;

    G4cout
        << "================================"
        << G4endl;

    G4cout
        << "Number of photons: "
        << run->GetNumberOfEvent()
        << G4endl;

    G4cout
        << "Total energy deposited: "
        << totalEnergy / MeV
        << " MeV"
        << G4endl;

    G4cout
        << "Phantom mass: "
        << phantomMass / kg
        << " kg"
        << G4endl;

    G4cout
        << "Absorbed dose: "
        << dose / gray
        << " Gy"
        << G4endl;

    // =====================================================
    // PHOTON STATISTICS
    // =====================================================

    G4cout << G4endl;

    G4cout
        << "================================"
        << G4endl;

    G4cout
        << "        PHOTON STATISTICS"
        << G4endl;

    G4cout
        << "================================"
        << G4endl;

    G4cout
        << "Absorbed photons: "
        << fAbsorbedPhotons
        << G4endl;

    G4cout
        << "Scattered photons: "
        << fScatteredPhotons
        << G4endl;

    G4cout
        << "Transmitted photons: "
        << fTransmittedPhotons
        << G4endl;

    G4cout
        << "Unclassified photons: "
        << fUnclassifiedPhotons
        << G4endl;

    // -----------------------------------------
    // Final fate check
    // -----------------------------------------

    G4int classifiedPhotons =
        fAbsorbedPhotons +
        fTransmittedPhotons +
        fUnclassifiedPhotons;

    G4cout << G4endl;

    G4cout
        << "Final-fate total: "
        << classifiedPhotons
        << G4endl;

    G4cout
        << "Expected photons: "
        << run->GetNumberOfEvent()
        << G4endl;

    // =====================================================
    // 3D VOXEL DOSE
    // =====================================================

    G4cout << G4endl;

    G4cout
        << "================================"
        << G4endl;

    G4cout
        << "       3D VOXEL DOSE"
        << G4endl;

    G4cout
        << "================================"
        << G4endl;

    G4cout
        << "Voxel grid: "
        << "10 x 10 x 10"
        << G4endl;

    G4cout
        << "Number of voxels: "
        << 1000
        << G4endl;

    G4cout
        << "Voxel size: "
        << 3.0
        << " cm"
        << G4endl;

    G4cout
        << "Voxel mass: "
        << voxelMass / kg
        << " kg"
        << G4endl;

    G4cout
        << "Non-zero voxels: "
        << nonZeroVoxels
        << G4endl;

    G4cout
        << "Sum of voxel energies: "
        << voxelEnergySum / MeV
        << " MeV"
        << G4endl;

    G4cout << G4endl;

    // =====================================================
    // MAXIMUM VOXEL
    // =====================================================

    G4cout
        << "Maximum voxel energy: "
        << maximumVoxelEnergy / MeV
        << " MeV"
        << G4endl;

    G4cout
        << "Maximum voxel dose: "
        << maximumVoxelDose / gray
        << " Gy"
        << G4endl;

    G4cout
        << "Maximum dose position: ("
        << maxX << ", "
        << maxY << ", "
        << maxZ << ")"
        << G4endl;

    G4cout << G4endl;

    // =====================================================
    // MINIMUM VOXEL
    // =====================================================

    if (nonZeroVoxels > 0)
    {
        G4cout
            << "Minimum non-zero voxel energy: "
            << minimumVoxelEnergy / MeV
            << " MeV"
            << G4endl;

        G4cout
            << "Minimum non-zero voxel dose: "
            << minimumVoxelDose / gray
            << " Gy"
            << G4endl;

        G4cout
            << "Minimum dose position: ("
            << minX << ", "
            << minY << ", "
            << minZ << ")"
            << G4endl;
    }

    // =====================================================
    // MEAN DOSE
    // =====================================================

    G4cout << G4endl;

    G4cout
        << "Mean dose of non-zero voxels: "
        << meanVoxelDose / gray
        << " Gy"
        << G4endl;

    G4cout
        << "================================"
        << G4endl;
}