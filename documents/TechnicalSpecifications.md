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
  - [A. Description](#a-description)
    - [1. Architectural Components](#1-architectural-components)
      - [a. Documents](#a-documents)
      - [b. API](#b-api)
      - [c. Data Management](#c-data-management)
    - [2. Workflow Example](#2-workflow-example)
    - [3. Technology Stack](#3-technology-stack)
  - [B. How it works ?](#b-how-it-works-)
  - [C. Program architecture diagram](#c-program-architecture-diagram)
- [III. Quality Control](#iii-quality-control)
  - [A. Documentation and Reporting](#a-documentation-and-reporting)
  - [B. Development](#b-development)
- [IV. Further considerations](#iv-further-considerations)
  - [A. Technical constraints](#a-technical-constraints)
    - [1. Writing convention](#1-writing-convention)
    - [2. C++ coding convention](#2-c-coding-convention)
  - [B. Cost estimation](#b-cost-estimation)
    - [1. Software](#1-software)
    - [2. Time \& Human](#2-time--human)
  - [C. Security](#c-security)
  - [D. Accessibility](#d-accessibility)
- [V. Success evaluation](#v-success-evaluation)
- [Glossary](#glossary)

</details>

![-](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

# I. Document

## A. Information

| Document writer | Grégory PAGNOUX |
|---|---|
| Issue date | 01/27/2025 |
| Reviewer | Lucas MEGNAN |
| date | 01/../2025 |

## B. History

| Version | Edits completed by | Date | Description of edit |
|---|---|---|---|
| 1.1 | Grégory PAGNOUX | 01/06/2025 | Template, [Writing convention](#1-writing-convention), [Quality Control](#iii-quality-control) part A, [Success evaluation](#v-success-evaluation) |
| 1.2 | Grégory PAGNOUX | 01/08/2025 | [Development](#b-development), [Software](#1-software), [Time & human](#2-time--human), [Overview](#c-overview), [Documents](#a-documents) |
| 1.3 | Grégory PAGNOUX | 01/14/2025 | [Coding convention](#2-c-coding-convention) |

## C. Overview

SmashThePath aim to make travel easier with faster pathing using the language C++ and the software VScode. The users can use this to gain time and travel more effectivilly and easily across the United States.

This project is an oppurtunity to dive into the very famous laguage C++.
Our main goal is to make a very easy and faster way to travel with an API.

# II. Solution

## A. Description

<!--TODO-->

### 1. Architectural Components

#### a. Documents

```txt
📦 2024-2025-project-3-quickest-path-team-1
└── 📁 beta
|    └── 📝 a_star.cpp
|    └── 📝 api.cpp
|    └── 📝 connectivity.cpp
|    └── 📝 incl.h
|    └── 📝 integrity.cpp
|    └── 📝 loader.cpp
|    └── 📝 log.cpp
|    └── 📝 main.cpp
└── 📁 documents
|    └── 📁 images
|    └── 📁 management
|    |   └── 📁 weeklyreport
|    |   |   └── 📝 SummaryTeamAnswers.md
|    |   |   └── 📝 WeeklyReport1.md
|    |   |   └── 📝 WeeklyReport2.md
|    |   |   └── 📝 WeeklyReport3.md
|    |   |   └── 📝 WeeklyReport4.md
|    |   |   └── 📝 WeeklyReport5.md
|    |   |   └── 📝 WeeklyReport6.md
|    |   |   └── 📝 WeeklyReportTemplate.md
|    |   └── 📝 DocumentManagement.md
|    |   └── 📝 PostMortem.md
|    |   └── 📝 ProjectCharter.md
|    |   └── 📝 Settlement.md
|    └── 📝 FunctionalSpecifications.md
|    └── 📝 TechnicalSpecifications.md
|    └── 📝 TestCase.md
|    └── 📝 TestPlan.md
└── 📝 README.MD 
```

#### b. API

<!--TODO-->

#### c. Data Management

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

### 2. Workflow Example

<!--TODO-->

**Link data file**: When the user link the data file, the algorithm treat all information to facilitate and have the quickest research.
**launch the algorithm**: When the user launch the algorithm, 
**research time**: 
**path displaying**: 

### 3. Technology Stack

<!--TODO-->

**Frontend (UI)**: This side of the product isn't required by the client. Informations are simply display as a list of all point that we need to follow to go on the point B. All points are display like : {Point 1, Point 2, distance}
**Backend (Logic & Processing)**: 

## B. How it works ?

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

## C. Program architecture diagram

![Program Architecture Diagram](images/)

# III. Quality Control

The Quality Control (QC) of the project will be mainly done by the Quality Assurance (QA) of our team.

## A. Documentation and Reporting

To ensure that the whole team has the same idea of the project, the [Functional](FunctionalSpecifications.md) and [Technical](TechnicalSpecifications.md) are drawn up.

Maintain a comprehensive record of the process. This should include:
Weekly Reports: A consolidated report of the week's progress, challenges, and learnings.
Meetings: to have a debriefing of everything that has been done and needs to be done.

## B. Development

The product is control task by task and report on the [Test Case](TestCase.md) as planned on the [Test Plan](TestPlan.md).

# IV. Further considerations

## A. Technical constraints

### 1. Writing convention

| Notation | How | Usage | Example |
| :-: | :-: | :-: | :-: |
| flatcase | we attach each word and in lowercase | naming folders | foldername |
| PascalCase | we attach each word and write the first letter of each word in uppercase | naming files | FileName |
| comment = ```<!--word-->``` | write your comment inside to have a reminder of your informations without have it visible on the document | organise and summarise informations that you need to put on each part of your document | ```<!--The following declaration creates a query. It doesn't appear on the document.-->``` |
| Titles = I.A.1.a | the first index is the most general title and the last one is to have the most detail part (preceded by #) | to have a clear idea of the organisation of the document | <pre> ```# I. Title name``` <br> ```## A. Title name``` <br> ```### 1. Title name``` <br> ```#### a. Title name``` </pre> |
| Table of content | at the beginning of the document | find a specific part of the document without going through it all |  |
| Glossary | at the end of the document | understand some word that we don't know without loose time on google |  |

You can have also [Markdown convention](https://www.markdownguide.org/basic-syntax/).

### 2. C++ coding convention

The language used to develop the project is C++

| Notation | How | Usage | Example |
| :-: | :-: | :-: | :-: |
| comment = // | start each line with two slashes and an uppercase letter and finish the comment by a period. | give more information of the code | <pre>```// The following declaration creates a query. It doesn't run the query.```</pre> |
| comment = ```/*paragraphe*/``` | start each line with two slashes and an uppercase letter and finish the comment by a period. | give more information of the code | <pre>```/*The following declaration creates a query.``` <br> ```It doesn't run the query.*/```</pre> |
| snake_case | each words are separated by an underscore | name variable | variable_name |
| camelCase | each words are concatenate and all words start by an uppercase except the first one | name functions | functionName |
| Interface (suffix) | put the word "Interface" at the end | named interface |  |
| Abstract (prefix) | put the word "Abstract" at the beginning | named abstract base classes |  |
| g_ (prefix) | put the letter "g" at the beginning separated by an underscore | named global variables |  |
| s_ (prefix) | put the letter "s" at the beginning separated by an underscore | named static class variables |  |
| c_ (prefix) | put the letter "c" at the beginning separated by an underscore | named global constants |  |

*source : [Gromacs](https://manual.gromacs.org/5.1-current/dev-manual/naming.html#:~:text=would%20be%20better.-,C%2B%2B%20code)*

## B. Cost estimation

### 1. Software

The software used to develop the project, we use Visual Studio and Visual Studio Code and the data file used is a CSV (but the alghorithm needs to be compatible with other types of data documents). Those development software are used on schoool or personnal Windows and MacOS. We need an internet connection given by the ALGOSUP's wifi or personnal fiber for those who work at home.

### 2. Time & Human

Minimum work time to accomplish the project:

| Number of people | work time/person | TOTAL |
| :-: | :-: | :-: |
| 6 | 77 h | 472 h |

## C. Security

<!--TODO-->

## D. Accessibility

<!--TODO-->

# V. Success evaluation

The program is scored according to the following criteria :

- **C++ Source Code**: Including comments and clear documentation. The code has to be of your own creation, and you should not use libraries beside STL and what is required for the Web server.
- **Time and Space Complexity**: Big O notation for the main algorithms.
- **REST API Implementation**: Demonstrating the ability to handle multiple formats (XML and JSON).
- **Test Suite**: Tests to validate correctness, performance, and compliance with the 10% approximation rule.
- **Data Validation Tool**: A utility to verify the integrity of the provided CSV file.

![-](https://raw.githubusercontent.com/andreasbm/readme/master/assets/lines/rainbow.png)

# Glossary

[^1]: []()
.
