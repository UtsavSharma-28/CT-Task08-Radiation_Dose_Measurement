#ifndef RunAction_h
#define RunAction_h

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;

class RunAction : public G4UserRunAction
{
public:

    RunAction();
    ~RunAction() override;

    // Energy
    void AddEnergyDeposit(G4double energy);

    // Photon statistics
    void AddAbsorbedPhoton();
    void AddScatteredPhoton();
    void AddTransmittedPhoton();
    void AddUnclassifiedPhoton();

    void BeginOfRunAction(
        const G4Run* run
    ) override;

    void EndOfRunAction(
        const G4Run* run
    ) override;

private:

    G4double fTotalEnergyDeposit;

    G4int fAbsorbedPhotons;
    G4int fScatteredPhotons;
    G4int fTransmittedPhotons;
    G4int fUnclassifiedPhotons;
};

#endif