#ifndef DetectorConstruction_h
#define DetectorConstruction_h

#include "G4VUserDetectorConstruction.hh"

class G4VPhysicalVolume;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    DetectorConstruction();
    ~DetectorConstruction() override;

    G4VPhysicalVolume* Construct() override;
};

#endif