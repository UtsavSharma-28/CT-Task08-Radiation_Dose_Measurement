#include "G4RunManager.hh"
#include "G4PhysListFactory.hh"
#include "G4ios.hh"

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"

int main()
{
    auto runManager = new G4RunManager();

    // Geometry
    runManager->SetUserInitialization(
        new DetectorConstruction()
    );

    // Physics
    G4PhysListFactory factory;

    runManager->SetUserInitialization(
        factory.GetReferencePhysList(
            "FTFP_BERT"
        )
    );

    // Radiation source
    runManager->SetUserAction(
        new PrimaryGeneratorAction()
    );

    // Run action
    auto runAction =
        new RunAction();

    runManager->SetUserAction(
        runAction
    );

    // Event action
    auto eventAction =
        new EventAction(
            runAction
        );

    runManager->SetUserAction(
        eventAction
    );

    // Stepping action
    runManager->SetUserAction(
        new SteppingAction(
            eventAction
        )
    );

    // Initialize Geant4
    runManager->Initialize();

    // -----------------------------------------
 // CT EXPOSURE PARAMETERS
 // -----------------------------------------

    G4double tubeCurrent = 100.0;   // mA
    G4double exposureTime = 1.0;   // seconds

    // Calculate mAs
    G4double mAs =
        tubeCurrent * exposureTime;

    // Simulation calibration:
    // 100 mAs = 10,000 photons
    G4double photonsPerMAs = 100.0;

    G4int numberOfPhotons =
        static_cast<G4int>(
            mAs * photonsPerMAs
            );

    G4cout << G4endl;
    G4cout << "================================" << G4endl;
    G4cout << "       CT EXPOSURE SETTINGS     " << G4endl;
    G4cout << "================================" << G4endl;

    G4cout << "Tube current: "
        << tubeCurrent
        << " mA"
        << G4endl;

    G4cout << "Exposure time: "
        << exposureTime
        << " s"
        << G4endl;

    G4cout << "Tube loading: "
        << mAs
        << " mAs"
        << G4endl;

    G4cout << "Simulated photons: "
        << numberOfPhotons
        << G4endl;

    G4cout << "================================" << G4endl;

    // Run simulation
    runManager->BeamOn(numberOfPhotons);

    delete runManager;

    return 0;
}