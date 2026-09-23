#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class XRaySpectrum;

class PrimaryGeneratorAction
    : public G4VUserPrimaryGeneratorAction
{
public:
    PrimaryGeneratorAction();

    ~PrimaryGeneratorAction() override;

    void GeneratePrimaries(
        G4Event* event
    ) override;

private:
    G4ParticleGun* fParticleGun;

    XRaySpectrum* fSpectrum;

    G4double fSourceRadius;
    G4double fCurrentAngle;
};

#endif