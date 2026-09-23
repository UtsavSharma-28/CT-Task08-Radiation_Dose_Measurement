#include "SteppingAction.hh"

#include "EventAction.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4StepPoint.hh"
#include "G4VProcess.hh"
#include "G4VPhysicalVolume.hh"

// =========================================================
// CONSTRUCTOR
// =========================================================

SteppingAction::SteppingAction(
    EventAction* eventAction
)
    : fEventAction(eventAction)
{
}

// =========================================================
// DESTRUCTOR
// =========================================================

SteppingAction::~SteppingAction()
{
}

// =========================================================
// STEPPING ACTION
// =========================================================

void SteppingAction::UserSteppingAction(
    const G4Step* step
)
{
    auto track = step->GetTrack();

    // =====================================================
    // ONLY GAMMA PHOTONS
    // =====================================================

    if (track->GetDefinition()->GetParticleName()
        != "gamma")
    {
        return;
    }

    // =====================================================
    // ONLY PRIMARY PHOTON
    //
    // Track ID = 1 is the original X-ray photon.
    // Secondary photons are not used for final photon
    // statistics.
    // =====================================================

    if (track->GetTrackID() != 1)
    {
        return;
    }

    // =====================================================
    // GET VOLUMES
    // =====================================================

    auto preVolume =
        step->GetPreStepPoint()->GetPhysicalVolume();

    auto postVolume =
        step->GetPostStepPoint()->GetPhysicalVolume();

    // =====================================================
    // GET PHYSICS PROCESS
    // =====================================================

    auto process =
        step->GetPostStepPoint()
        ->GetProcessDefinedStep();

    if (process != nullptr)
    {
        G4String processName =
            process->GetProcessName();

        // ---------------------------------------------
        // ANY PHYSICS INTERACTION
        // ---------------------------------------------

        if (processName != "Transportation")
        {
            fEventAction->SetInteracted();
        }

        // ---------------------------------------------
        // SCATTERING
        // ---------------------------------------------

        if (processName == "compt" ||
            processName == "Rayl")
        {
            fEventAction->SetScattered();
        }

        // ---------------------------------------------
        // PHOTOELECTRIC ABSORPTION
        // ---------------------------------------------

        if (processName == "phot")
        {
            fEventAction->SetAbsorbed();
        }
    }

    // =====================================================
    // TRANSMISSION
    //
    // Primary photon leaves:
    //
    //       Phantom -> World
    //
    // This means the photon has passed through the phantom.
    // =====================================================

    if (preVolume != nullptr &&
        postVolume != nullptr)
    {
        if (preVolume->GetName() == "Phantom" &&
            postVolume->GetName() == "World")
        {
            fEventAction->SetTransmitted();
        }
    }
}