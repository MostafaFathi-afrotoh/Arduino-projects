# System Unification & Logical Flowchart

```mermaid
flowchart TD
    A[Power On / Setup] --> B[Initialize LCD & Bluetooth]
    B --> C[Loop: Check Bluetooth Serial Command]
    
    C -->|Command Received| D{Command Type?}
    D -->|START| E[Set startProcess = True]
    D -->|STOP| F[Set startProcess = False & Safety Shutoff]
    D -->|TEMP/TIME/AIR| G[Update Operating Parameters]
    
    C --> H{Is Process Started?}
    H -->|No| C
    H -->|Yes| I[Read DS18B20 Temp Sensors T1 & T2]
    
    I --> J[Display Data on LCD & Transmit via Bluetooth]
    J --> K{Thermal Control Check}
    
    K -->|T1 < Target| L[Activate Upper Heater]
    K -->|T2 < Target| M[Activate Bottom Heater]
    
    L & M --> N[Update Countdown Timer]
    N --> O{Timer Elapsed?}
    O -->|Yes| F
    O -->|No| C
