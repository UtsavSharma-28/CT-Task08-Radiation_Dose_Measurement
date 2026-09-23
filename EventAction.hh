#ifndef EventAction_h
#define EventAction_h

#include "G4UserEventAction.hh"
#include "globals.hh"

class RunAction;
class G4Event;

class EventAction : public G4UserEventAction
{
public:

    EventAction(RunAction* runAction);
    ~EventAction() override;

    void BeginOfEventAction(
        const G4Event* event
    ) override;

    void EndOfEventAction(
        const G4Event* event
    ) override;

    // Photon interaction
    void SetInteracted();
    void SetScattered();

    // Final photon fate
    void SetAbsorbed();
    void SetTransmitted();
    void SetUnclassified();

private:

    RunAction* fRunAction;

    G4bool fInteracted;
    G4bool fScattered;

    G4bool fAbsorbed;
    G4bool fTransmitted;
    G4bool fUnclassified;
};

#endif