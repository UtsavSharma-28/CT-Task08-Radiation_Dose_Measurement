#include "PrimaryGeneratorAction.hh"

#include "XRaySpectrum.hh"

#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4Event.hh"
#include "G4ios.hh"

#include <cmath>

PrimaryGeneratorAction::PrimaryGeneratorAction()
    : fParticleGun(nullptr),
    fSpectrum(nullptr),
    fSourceRadius(50.0 * cm),
    fCurrentAngle(0.0)
{
    // -----------------------------------------
    // Particle gun
    // -----------------------------------------

    fParticleGun =
        new G4ParticleGun(1);

    // -----------------------------------------
    // X-ray photon
    // -----------------------------------------

    auto photon =
        G4ParticleTable::GetParticleTable()
        ->FindParticle("gamma");

    fParticleGun->SetParticleDefinition(
        photon
    );

    // -----------------------------------------
    // X-ray spectrum
    // -----------------------------------------

    fSpectrum =
        new XRaySpectrum();
}


PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fSpectrum;
    delete fParticleGun;
}


void PrimaryGeneratorAction::GeneratePrimaries(
    G4Event* event
)
{
    // -----------------------------------------
    // Event number
    // -----------------------------------------

    G4int eventID =
        event->GetEventID();


    // -----------------------------------------
    // CT rotation angle
    // -----------------------------------------

    G4double angle =
        eventID * 1.0 * deg;

    angle =
        std::fmod(
            angle,
            360.0 * deg
        );


    // -----------------------------------------
    // Convert angle to radians
    // -----------------------------------------

    G4double angleRad =
        angle / rad;


    // -----------------------------------------
    // Source position
    // -----------------------------------------

    G4double x =
        fSourceRadius *
        std::cos(angleRad);

    G4double y =
        fSourceRadius *
        std::sin(angleRad);

    G4double z = 0.0;

    G4ThreeVector sourcePosition(
        x, y, z
    );


    // -----------------------------------------
    // Direction toward phantom center
    // -----------------------------------------

    G4ThreeVector direction =
        -sourcePosition.unit();


    // -----------------------------------------
    // SAMPLE X-RAY ENERGY
    // -----------------------------------------

    G4double photonEnergy =
        fSpectrum->SampleEnergy();


    // -----------------------------------------
    // Set photon
    // -----------------------------------------

    fParticleGun->SetParticleEnergy(
        photonEnergy
    );

    fParticleGun->SetParticlePosition(
        sourcePosition
    );

    fParticleGun->SetParticleMomentumDirection(
        direction
    );


    // -----------------------------------------
    // Demonstration output
    // -----------------------------------------

    if (eventID < 10)
    {
        G4cout
            << "Event "
            << eventID
            << " | Angle = "
            << angle / deg
            << " deg"
            << " | Energy = "
            << photonEnergy / keV
            << " keV"
            << G4endl;
    }


    // -----------------------------------------
    // Generate photon
    // -----------------------------------------

    fParticleGun->GeneratePrimaryVertex(
        event
    );
}