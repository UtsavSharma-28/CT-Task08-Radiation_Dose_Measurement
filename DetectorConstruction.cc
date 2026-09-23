#include "DetectorConstruction.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"

#include "DoseScorer.hh"
#include "G4SDManager.hh"

DetectorConstruction::DetectorConstruction() {}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    // Get access to standard materials
    G4NistManager* nist = G4NistManager::Instance();

    // ---------------- WORLD ----------------

    // World material = air
    G4Material* worldMaterial =
        nist->FindOrBuildMaterial("G4_AIR");

    // World size = 2 m × 2 m × 2 m
    G4double worldSize = 2.0 * m;

    G4Box* solidWorld =
        new G4Box(
            "World",
            worldSize / 2,
            worldSize / 2,
            worldSize / 2
        );

    G4LogicalVolume* logicWorld =
        new G4LogicalVolume(
            solidWorld,
            worldMaterial,
            "World"
        );

    G4VPhysicalVolume* physWorld =
        new G4PVPlacement(
            nullptr,
            G4ThreeVector(),
            logicWorld,
            "World",
            nullptr,
            false,
            0,
            true
        );

    // ---------------- WATER PHANTOM ----------------

    // Phantom material = water
    G4Material* water =
        nist->FindOrBuildMaterial("G4_WATER");

    // Phantom dimensions = 30 cm × 30 cm × 30 cm
    G4double phantomSize = 30.0 * cm;

    G4Box* solidPhantom =
        new G4Box(
            "Phantom",
            phantomSize / 2,
            phantomSize / 2,
            phantomSize / 2
        );

    G4LogicalVolume* logicPhantom =
        new G4LogicalVolume(
            solidPhantom,
            water,
            "Phantom"
        );

    new G4PVPlacement(
        nullptr,
        G4ThreeVector(),
        logicPhantom,
        "Phantom",
        logicWorld,
        false,
        0,
        true
    );
	// ---------------- DOSE SCORER ----------------

auto doseScorer = new DoseScorer("DoseScorer");

G4SDManager::GetSDMpointer()
    ->AddNewDetector(doseScorer);

logicPhantom->SetSensitiveDetector(doseScorer);

    return physWorld;
}