# Technical Specifications

![-](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

<details>
<summary>📖 Table of content</summary>

- [Technical Specifications](#technical-specifications)
  - [I. Document](#i-document)
    - [A. Information](#a-information)
    - [B. History](#b-history)
    - [C. Overview](#c-overview)
  - [II. Solution](#ii-solution)
    - [A. Descritpion](#a-descritpion)
      - [1. Architectural Components](#1-architectural-components)
        - [a. API](#a-api)
        - [b. Data Management Layer](#b-data-management-layer)
      - [2. Workflow Example](#2-workflow-example)
      - [3. Technology Stack](#3-technology-stack)
    - [B. Technical constraints](#b-technical-constraints)
      - [1. Writing convention](#1-writing-convention)
      - [2. C++ coding convention](#2-c-coding-convention)
    - [C. How it works ?](#c-how-it-works-)
    - [D. Program architecture diagram](#d-program-architecture-diagram)
  - [III. Quality Control](#iii-quality-control)
    - [A. Documentation and Reporting](#a-documentation-and-reporting)
    - [B. Development](#b-development)
  - [IV. Further considerations](#iv-further-considerations)
    - [A. Cost estimation](#a-cost-estimation)
      - [1. Software](#1-software)
      - [2. Time \& Human](#2-time--human)
    - [B. Security](#b-security)
    - [C. Accessibility](#c-accessibility)
  - [V. Success evaluation](#v-success-evaluation)
  - [Glossary](#glossary)

</details>

![-](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

## I. Document

### A. Information

| Document writer | Grégory PAGNOUX |
|---|---|
| Issue date | 01/27/2025 |
| Reviewer | Lucas MEGNAN |
| date | 01/../2025 |

### B. History

| Version | Edits completed by | Date | Description of edit |
|---|---|---|---|
| 1.1 | Grégory PAGNOUX | 01/06/2025 | Template, [Writing convention](#1-writing-convention), [Quality Control](#iii-quality-control) part A, [Success evaluation](#v-success-evaluation) |

### C. Overview

<!--TODO-->

This project encourages you to explore and implement efficient algorithms tailored to handle large-scale datasets, while also considering real-world constraints like speed and accuracy.

## II. Solution

### A. Descritpion

<!--TODO-->

#### 1. Architectural Components

##### a. API

<!--TODO-->

##### b. Data Management Layer

<!--TODO-->

This layer focuses on data storage, security, and regulatory compliance, especially concerning health data.

**Local Data Storage**

**Temporary Data Cache**: Stores session data temporarily on the mirror, discarding it once synced to the smartphone.
**Data Retention Policy**: Implements time-based deletion for data, following GDPR and other regional privacy laws.

**Data Syncing and Backup**

**Smartphone Sync**: Ensures user data is synced to their smartphone when connected. Data is encrypted during transfer.
**User-Controlled Data Backup**: Allows users to enable/disable data backups on the smartphone app, giving them control over data retention and sharing.

**Security and Privacy Management**

**Compliance with GDPR**: Ensures data collection, storage, and deletion processes align with GDPR. Users can control their data visibility and deletion settings in the app.

#### 2. Workflow Example

<!--TODO-->

**Power On**: When the user powers on the mirror, the Device Control Manager initializes the mirror’s display and checks connectivity status.
**Health Metric Measurement**: When the user selects a function, like pulse monitoring, the Device Control Manager activates the relevant sensor, and the Application Layer processes and displays results in real-time.
**Data Syncing with Smartphone**: If the smartphone is connected, the Bluetooth Communication Module sends data to the app, where it’s displayed on the dashboard and saved for future reference.
**Data Privacy**: All sensitive data is temporarily stored on the mirror and deleted after syncing, or per user preference, aligned with privacy regulations.

#### 3. Technology Stack

<!--TODO-->

**Frontend (UI)**: HTML/CSS/JavaScript (for Mirror UI), Java or Swift (for mobile app UI)
**Backend (Logic & Processing)**: Arduino for embedded system logic, Java (mobile app backend)
**Data Storage**: SQLite (on-device), local storage on the app
Communication Protocols: Bluetooth Low Energy (BLE) for mirror-to-phone data sync
**Security**: GDPR compliance modules

This architecture allows Phoenix to be a connected, privacy-conscious, and user-friendly health monitoring solution, integrating multiple health sensors into a modular design for ease of future updates and enhancements.

### B. Technical constraints

#### 1. Writing convention

| Notation | How | Usage | Example |
| :-: | :-: | :-: | :-: |
| flatcase | we attach each word and in lowercase | naming folders | foldername |
| PascalCase | we attach each word and write the first letter of each word in uppercase | naming files | FileName |
| comment = ```<!--word-->``` | write your comment inside to have a reminder of your informations without have it visible on the document | organise and summarise informations that you need to put on each part of your document | ```<!--The following declaration creates a query. It doesn't appear on the document.-->``` |
| Titles = I.A.1.a | the first index is the most general title and the last one is to have the most detail part (preceded by #) | to have a clear idea of the organisation of the document | <pre> ```# I. Title name``` <br> ```## A. Title name``` <br> ```### 1. Title name``` <br> ```#### a. Title name``` </pre> |
| Table of content | at the beginning of the document | find a specific part of the document without going through it all |  |
| Glossary | at the end of the document | understand some word that we don't know without loose time on google |  |

You can have also [Markdown convention](https://www.markdownguide.org/basic-syntax/).

#### 2. C++ coding convention

<!--TODO-->

The language used to develop the project is C++

| Notation | How | Usage | Example |
| :-: | :-: | :-: | :-: |
| comment = // | start each line with two slashes and an uppercase letter and finish the comment by a period. | give more information of the code | <pre>```// The following declaration creates a query. It doesn't run the query.```</pre> |
| comment = ```/*paragraphe*/``` | start each line with two slashes and an uppercase letter and finish the comment by a period. | give more information of the code | <pre>```/*The following declaration creates a query.``` <br> ```It doesn't run the query.*/```</pre> |
| snake_case |  |  | variable_name |
| Pascal_Snake_Case |  |  | Fonction_Name |
|  |  |  |  |

*source : []()*

### C. How it works ?

<!--TODO-->

**1. Tanks.cs file**

Define every function/method/file we use that is external to the page.

```cs
using System;
using System.Collections;
using System.Threading.Tasks;
```

![-](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/aqua.png)

Inside the namespace KrugApp, we start by define a new class "Tank" accessible

```cs
namespace KrugApp
{
    public class Tank
    {
        ...
    }
}
```

### D. Program architecture diagram

![Program Architechture Diagram](images/)

## III. Quality Control

The Quality Control (QC) of the project will be mainly done by the Quality Assurance (QA) of our team.

### A. Documentation and Reporting

To ensure that the whole team has the same idea of the project, the [Functional](FunctionalSpecifications.md) and [Technical](TechnicalSpecifications.md) are drawn up.

Maintain a comprehensive record of the process. This should include:
Weekly Reports: A consolidated report of the week's progress, challenges, and learnings.
Meetings: to have a debriefing of everything that has been done and needs to be done.

### B. Development

<!--TODO-->

## IV. Further considerations

### A. Cost estimation

#### 1. Software

<!--TODO-->

#### 2. Time & Human

| Number of people | work time/person | TOTAL |
| :-: | :-: | :-: |
| 6 | 94 h 30 | 567 h |

### B. Security

<!--TODO-->

### C. Accessibility

<!--TODO-->

## V. Success evaluation

The program is scored according to the following criteria :

- **C++ Source Code**: Including comments and clear documentation. The code has to be of your own creation, and you should not use libraries beside STL and what is required for the Web server.
- **Time and Space Complexity**: Big O notation for the main algorithms.
- **REST API Implementation**: Demonstrating the ability to handle multiple formats (XML and JSON).
- **Test Suite**: Tests to validate correctness, performance, and compliance with the 10% approximation rule.
- **Data Validation Tool**: A utility to verify the integrity of the provided CSV file.

![-](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

## Glossary

[^1]: []()
.
