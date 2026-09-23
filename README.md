# CT-Task08-Radiation_Dose_Measurement

## 1. Module Overview

This module focuses on the simulation and estimation of radiation dose produced during a CT scan using the **Geant4 Monte Carlo toolkit**. The simulation models the transport and interaction of X-ray photons inside a water-equivalent phantom. Individual photons are tracked through the phantom, and their interactions are used to determine how radiation energy is deposited.

The module also implements **3D voxel-based dose scoring**. The phantom is divided into a 10 × 10 × 10 voxel grid, and the energy deposited in each voxel is recorded. The deposited energy is then converted into absorbed dose using the mass of the corresponding voxel.

The overall objective is to build a foundation for **CT radiation dosimetry**, which can later be extended toward realistic CT scanner geometry, clinical X-ray spectra, CTDI estimation, and patient-specific dose estimation.

---

## 2. Inputs & Outputs

### Inputs

- **X-ray photons:** Primary photons are generated using the implemented X-ray source and energy spectrum.
- **Photon energy:** The source supports photon energies corresponding to the simulated CT X-ray spectrum.
- **Number of photons:** The number of primary photons can be specified for each simulation run.
- **Water phantom:** A 30 cm × 30 cm × 30 cm water-equivalent phantom is used for the current simulation.
- **Voxel grid:** The phantom is divided into a 10 × 10 × 10 grid, resulting in 1,000 voxels.
- **Geant4 physics:** Photon transport and interactions are simulated using Geant4 physics models.

### Outputs

- **Total energy deposited:** Total radiation energy deposited inside the phantom, reported in MeV.
- **Absorbed dose:** Dose calculated from deposited energy and phantom/voxel mass.
- **Photon fate classification:** Photons are classified according to their final behavior:
  - Absorbed
  - Scattered
  - Transmitted
  - Unclassified
- **3D voxel energy distribution:** Energy deposited in each voxel.
- **3D voxel dose distribution:** Absorbed dose calculated for individual voxels.
- **Simulation statistics:** Number of photons, non-zero voxels, voxel size, voxel mass, and total voxel energy.

The basic absorbed-dose relation is:

$$
D = \frac{E_{\mathrm{dep}}}{m}
$$

where:

- $D$ = absorbed dose in Gy
- $E_{\mathrm{dep}}$ = deposited energy in joules
- $m$ = mass in kilograms

For conversion of simulated energy:

$$
1\ \mathrm{MeV} = 1.602176634 \times 10^{-13}\ \mathrm{J}
$$

---

## 3. Current Progress

- [x] Set up Geant4 simulation environment
- [x] Implemented world and phantom geometry
- [x] Implemented primary X-ray photon generation
- [x] Implemented photon transport and interaction physics
- [x] Implemented total energy-deposition calculation
- [x] Implemented absorbed-dose calculation
- [x] Implemented photon fate classification
- [x] Implemented 3D voxel-based dose scoring
- [x] Implemented voxel energy statistics
- [x] Tested the simulation with different numbers of photons
- [x] Verified photon fate accounting
- [x] Generated a 3D voxel dose distribution
- [ ] Validate results against experimental/reference CT dosimetry data
- [ ] Implement realistic CT scanner geometry
- [ ] Implement CT rotational source motion
- [ ] Improve the X-ray spectrum and filtration model
- [ ] Implement CTDI-based validation
- [ ] Extend the simulation toward patient-specific CT dose estimation

### Current Test Result

A representative simulation using **10,000 primary photons** produced:

| Parameter | Result |
|---|---:|
| Number of photons | 10,000 |
| Total energy deposited | 555.643 MeV |
| Phantom mass | 27 kg |
| Absorbed dose | 3.29718 × 10⁻¹² Gy |
| Voxel grid | 10 × 10 × 10 |
| Number of voxels | 1,000 |
| Voxel size | 3 cm |
| Voxel mass | 0.027 kg |

The simulation also produces photon-fate statistics and a 3D voxel energy/dose distribution.

---

## 4. How We Will Validate Results

### A. Photon Conservation

The final photon classifications should account for the primary photons:

$$
N_{\mathrm{absorbed}} +
N_{\mathrm{scattered}} +
N_{\mathrm{transmitted}} +
N_{\mathrm{unclassified}}
\approx
N_{\mathrm{primary}}
$$

For example, for a simulation with 10,000 primary photons, the final-fate count should account for the complete set of simulated photons.

### B. Energy Deposition

The sum of the energy deposited in all individual voxels will be compared with the independently calculated total deposited energy.

$$
E_{\mathrm{total}} =
\sum_{i=1}^{N} E_i
$$

where $E_i$ represents the energy deposited in voxel $i$.

### C. Dose Calculation

The absorbed dose will be verified using:

$$
D = \frac{E_{\mathrm{dep}}}{m}
$$

with appropriate conversion of energy from MeV to joules.

### D. Voxel Validation

The following quantities will be checked:

- Correct number of voxels
- Correct voxel dimensions
- Correct voxel volume
- Correct voxel mass
- No negative energy deposition values
- No NaN or infinite values
- Consistency between voxel energy sum and total deposited energy

### E. Physical Validation

The simulated results will eventually be compared with established CT dosimetry quantities such as **CTDI** and with published Monte Carlo CT dose simulations.

---

## 5. Code Files

The simulation is implemented in **C++ using Geant4**.

### Main Source Files

| File | Function |
|---|---|
| `main.cc` | Initializes and runs the Geant4 simulation |
| `DetectorConstruction.cc/.hh` | Defines the world, phantom, and simulation geometry |
| `PrimaryGeneratorAction.cc/.hh` | Generates primary X-ray photons |
| `RunAction.cc/.hh` | Handles run-level initialization and final statistics |
| `EventAction.cc/.hh` | Collects event-level information |
| `SteppingAction.cc/.hh` | Tracks particle steps and photon interactions |
| `DoseScorer.cc/.hh` | Records energy deposition in voxels and calculates the 3D dose distribution |
| `XRaySpectrum.hh` | Contains the implemented X-ray energy spectrum |
| `CMakeLists.txt` | Configures the project build using CMake |

---

## 6. Project Workflow

```text
        X-ray Photon Source
                │
                ▼
       Photon Energy Sampling
                │
                ▼
        Geant4 Particle Transport
                │
                ▼
      Photon–Matter Interactions
                │
        ┌───────┼────────┐
        ▼       ▼        ▼
     Absorbed Scattered Transmitted
        │       │        │
        └───────┼────────┘
                ▼
        Energy Deposition
                │
                ▼
        3D Voxel Scoring
                │
                ▼
       Voxel Energy Distribution
                │
                ▼
          Absorbed Dose
                │
                ▼
       3D Dose Distribution
```

---

## 7. Future Work

The current simulation provides the basic Monte Carlo dose-estimation framework. Future development will focus on:

- Realistic CT X-ray spectra
- Bow-tie filtration
- CT collimation
- Rotating X-ray source
- CTDI phantom simulation
- Comparison with measured CTDI values
- More realistic anthropomorphic phantoms
- Organ-specific dose estimation
- Patient-specific CT dose estimation
- Dose visualization and analysis
- Comparison with published Monte Carlo and experimental results

---

## 8. References

The development of this module is informed by published work on Monte Carlo CT dosimetry and patient-specific CT dose estimation.

- Geant4-based Monte Carlo methods for CT dose and CTDI estimation.
- Monte Carlo simulation studies of CT radiation dose.
- Patient-specific CT dose estimation using Monte Carlo methods.
- Deep-learning approaches for reproducing Monte Carlo patient-specific dose estimates.
