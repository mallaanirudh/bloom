Bloom
> **Lightweight C middleware for ROS 2 ↔ MCU communication.**
>
> Bloom aims to provide a minimal, lightweight MCU API while keeping the underlying protocol and transport layers fully modular and replaceable.
---
🏗️ Architecture Stack
```text
       ROS 2
         ↓
    Bloom Core
         ↓
 Protocol Interface
         ↓
    XRCE Adapter
         ↓
   Micro XRCE-DDS
         ↓
     Transport
         ↓
        MCU
```

