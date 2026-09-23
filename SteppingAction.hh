#ifndef SteppingAction_h
#define SteppingAction_h

#include "G4UserSteppingAction.hh"

class G4Step;
class EventAction;

class SteppingAction : public G4UserSteppingAction
{
public:
    SteppingAction(
        EventAction* eventAction
    );

    ~SteppingAction() override;

    void UserSteppingAction(
        const G4Step* step
    ) override;

private:
    EventAction* fEventAction;
};

#endif