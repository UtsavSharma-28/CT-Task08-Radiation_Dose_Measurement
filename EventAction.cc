#include "EventAction.hh"
#include "RunAction.hh"

#include "G4Event.hh"

// =========================================================
// CONSTRUCTOR
// =========================================================

EventAction::EventAction(
    RunAction* runAction
)
    : fRunAction(runAction),
    fInteracted(false),
    fScattered(false),
    fAbsorbed(false),
    fTransmitted(false),
    fUnclassified(false)
{
}

// =========================================================
// DESTRUCTOR
// =========================================================

EventAction::~EventAction()
{
}

// =========================================================
// BEGIN OF EVENT
// =========================================================

void EventAction::BeginOfEventAction(
    const G4Event*
)
{
    fInteracted = false;
    fScattered = false;

    fAbsorbed = false;
    fTransmitted = false;
    fUnclassified = false;
}

// =========================================================
// INTERACTION
// =========================================================

void EventAction::SetInteracted()
{
    fInteracted = true;
}

// =========================================================
// SCATTERING
// =========================================================

void EventAction::SetScattered()
{
    fScattered = true;
}

// =========================================================
// ABSORPTION
// =========================================================

void EventAction::SetAbsorbed()
{
    fAbsorbed = true;
}

// =========================================================
// TRANSMISSION
// =========================================================

void EventAction::SetTransmitted()
{
    fTransmitted = true;
}

// =========================================================
// UNCLASSIFIED
// =========================================================

void EventAction::SetUnclassified()
{
    fUnclassified = true;
}

// =========================================================
// END OF EVENT
// =========================================================

void EventAction::EndOfEventAction(
    const G4Event*
)
{
    // ---------------------------------------------
    // FINAL FATE OF PRIMARY PHOTON
    // ---------------------------------------------

    if (fTransmitted)
    {
        fRunAction->AddTransmittedPhoton();
    }
    else if (fAbsorbed)
    {
        fRunAction->AddAbsorbedPhoton();
    }
    else
    {
        fRunAction->AddUnclassifiedPhoton();
    }

    // ---------------------------------------------
    // SCATTERING IS A SEPARATE STATISTIC
    // ---------------------------------------------

    if (fScattered)
    {
        fRunAction->AddScatteredPhoton();
    }
}