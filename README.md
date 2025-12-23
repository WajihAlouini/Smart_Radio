# 📡 SmartRadio Studio Engine v3.0

**SmartRadio Studio Engine** is a premium, high-performance desktop application designed for modern radio station management. Built with C++ and the Qt Framework, it provides a seamless interface for managing staff, broadcasts, guests, and sponsorships in real-time.

![Premium UI](https://img.shields.io/badge/UI-Indigo_Glassmorphism-6366f1?style=for-the-badge)
![Tech](https://img.shields.io/badge/Language-C++-00599C?style=for-the-badge&logo=cplusplus)
![Framework](https://img.shields.io/badge/Framework-Qt_6.10-41CD52?style=for-the-badge&logo=qt)

---

## ✨ Core Features

### 🔐 Advanced Authentication Hub

- **Secure Identity**: Multi-stage login and signup flow with real-time field validation.
- **Smart Password Recovery**: 4-digit security code verification via SMTP (Email) or Backend Security Hub override.
- **Visual Identity**: Premium "Indigo Glassmorphism" cards with micro-animations and shadow layering.

### 👤 Profile & Talent Management

- **In-App Editing**: Users can update their credentials, contacts, and security keys without leaving the dashboard.
- **Staff Directory**: Professional HR & Payroll tracking with integrated search and data export.
- **Guest Booking**: Comprehensive talent management for live interview scheduling.

### 📻 Broadcast Operations

- **Live Hub**: Real-time management of radio shows and technical logs.
- **Historique System**: Automated tracking of broadcast history and technical metrics.
- **Arduino Integration**: Direct hardware synchronization for live studio indicators (On-Air signs, VU meters).

### 💎 Revenue & Sponsorship

- **Sponsor Tracking**: Full control over revenue streams and advertising partner management.
- **Live Stats**: Real-time dashboard updates reflecting station performance and database health.

---

## 🛠️ Technical Stack

- **Frontend**: Qt Widgets (custom QSS Styling, Transitions, and Property Animations).
- **Backend**: C++ Object-Oriented Engine with robust error handling and logging.
- **Data Layer**: Oracle Database integration (QODBC/QSQL) for enterprise-grade persistence.
- **Communication**: QSslSocket-based SMTP implementation for secure verification.
- **Hardware**: Serial Port communication (QSerialPort) for Arduino studio hardware.

---

## 📂 Project Architecture

The project follows a modular, enterprise-focused directory structure:

- `core/auth/`: Identity management modules.
- `core/dashboard/`: Main command center.
- `core/database/`: Persistence layer.
- `modules/`: Feature-specific modules (Staff, Broadcast, etc.).
- `utils/ui/`: Design system and notification engine.
- `utils/system/`: Core services (Config, Logger, Validation).
- `utils/network/`: Secure communication protocols.

---

## 🚀 Getting Started

### Prerequisites

- **Qt SDK** (version 6.x recommended)
- **MinGW** or **MSVC** Compiler
- **Oracle Instant Client** (for database connectivity)

### Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/WajihAlouini/Smart_Radio.git
   ```
2. Open `SmartRadio.pro` in **Qt Creator**.
3. Configure your database and SMTP settings in `SmartRadio.ini` (located in AppConfigLocation).
4. Build and Run!

---

## 👨‍💻 Author

**Wajih Alouini**
_Integrated 2nd Year Project - Radio Management Solution_

---

_© 2025 SmartRadio Studio. All rights reserved._
