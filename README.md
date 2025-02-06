# SmashThePath

<details>

<summary> Table of content </summary>

- [SmashThePath](#smashthepath)
- [Team Members](#team-members)
- [Overview](#overview)
- [What is a Quickest Path ?](#what-is-a-quickest-path-)
- [What is C++ ?](#what-is-c-)
- [Deadlines](#deadlines)
- [How to Use](#how-to-use)
- [Project Documents](#project-documents)

</details>

# Team Members

|Photo                                                                                     |Name             |Role              |GitHub                                                                                                                                                                                                                                                                                                                                                                                                             |LinkedIn                                                                                                                                                                                                                                      |
|:----------------------------------------------------------------------------------------:|:---------------:|:----------------:|:-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------:|:--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------:|
|<img src="https://ca.slack-edge.com/T019N8PRR7W-U07D74YDG95-51023ff903b0-512" width="200">|Alexis SANTOS    |Project Manager   |<a href="https://github.com/Mamoru-fr">  <picture><source media="(prefers-color-scheme: dark)" srcset="https://github.com/algosup/2023-2024-project-3-virtual-processor-team-2/raw/main/documents/images/management/github_light_logo.png"><source media="(prefers-color-scheme: light)" srcset="documents/images/management/github_dark_logo.png"><img alt="GitHub Logo"  style="width:50px"></picture></a>       | <a href="https://www.linkedin.com/in/alexis-santos-83481031b/"><img alt="Linkedin Icon" src="https://github.com/algosup/2023-2024-project-3-virtual-processor-team-2/raw/main/documents/images/management/linkedin_logo.png" width="100px">  |
|<img src="https://ca.slack-edge.com/T0871HD8PPG-U086WKXDVHD-cb7be3785c03-192" width="200">|Loïc NOGUES      |Program Manager   |<a href="https://github.com/Loic-nogues">  <picture><source media="(prefers-color-scheme: dark)" srcset="https://github.com/algosup/2023-2024-project-3-virtual-processor-team-2/raw/main/documents/images/management/github_light_logo.png"><source media="(prefers-color-scheme: light)" srcset="documents/images/management/github_dark_logo.png"><img alt="GitHub Logo"  style="width:50px"></picture></a>     | <a href="https://www.linkedin.com/in/loic-nogues-459606339/"><img alt="Linkedin Icon" src="https://github.com/algosup/2023-2024-project-3-virtual-processor-team-2/raw/main/documents/images/management/linkedin_logo.png" width="100px">    |
|<img src="https://ca.slack-edge.com/T07NMGKN89J-U07NG76JG21-c0a56378ea45-512" width="200">|Grégory PAGNOUX  |Technical Leader  |<a href="https://github.com/Gregory-Pagnoux">  <picture><source media="(prefers-color-scheme: dark)" srcset="https://github.com/algosup/2023-2024-project-3-virtual-processor-team-2/raw/main/documents/images/management/github_light_logo.png"><source media="(prefers-color-scheme: light)" srcset="documents/images/management/github_dark_logo.png"><img alt="GitHub Logo"  style="width:50px"></picture></a> | <a href="https://www.linkedin.com/in/grégory-pagnoux-313b3a251/"><img alt="Linkedin Icon" src="https://github.com/algosup/2023-2024-project-3-virtual-processor-team-2/raw/main/documents/images/management/linkedin_logo.png" width="100px">|
|<img src="https://ca.slack-edge.com/T0871HD8PPG-U0875BLK3PF-g5b656b3184b-192" width="200">|Yann-Maël BOUTON |Software Engineer |<a href="https://github.com/devnjoyer">  <picture><source media="(prefers-color-scheme: dark)" srcset="https://github.com/algosup/2023-2024-project-3-virtual-processor-team-2/raw/main/documents/images/management/github_light_logo.png"><source media="(prefers-color-scheme: light)" srcset="documents/images/management/github_dark_logo.png"><img alt="GitHub Logo"  style="width:50px"></picture></a>       | <a href=""><img alt="Linkedin Icon" src="https://github.com/algosup/2023-2024-project-3-virtual-processor-team-2/raw/main/documents/images/management/linkedin_logo.png" width="100px">                                                      | 
|<img src="https://ca.slack-edge.com/T0871HD8PPG-U087FQUB4BW-ab023db594b4-192" width="200">|Lucas MEGNAN     |Quality Assurance |<a href="https://github.com/LucasMegnan">  <picture><source media="(prefers-color-scheme: dark)" srcset="https://github.com/algosup/2023-2024-project-3-virtual-processor-team-2/raw/main/documents/images/management/github_light_logo.png"><source media="(prefers-color-scheme: light)" srcset="documents/images/management/github_dark_logo.png"><img alt="GitHub Logo"  style="width:50px"></picture></a>     | <a href="https://www.linkedin.com/in/lucas-megnan/"><img alt="Linkedin Icon" src="https://github.com/algosup/2023-2024-project-3-virtual-processor-team-2/raw/main/documents/images/management/linkedin_logo.png" width="100px">             |
|<img src="https://ca.slack-edge.com/T0871HD8PPG-U087D5NNFSN-gd49a1525f78-192" width="200">|Lena DE GERMAIN  |Technical Writer  |<a href="https://github.com/lenadg18">  <picture><source media="(prefers-color-scheme: dark)" srcset="https://github.com/algosup/2023-2024-project-3-virtual-processor-team-2/raw/main/documents/images/management/github_light_logo.png"><source media="(prefers-color-scheme: light)" srcset="documents/images/management/github_dark_logo.png"><img alt="GitHub Logo"  style="width:50px"></picture></a>        | <a href="https://www.linkedin.com/in/lena-degermain-5535a032a/"><img alt="Linkedin Icon" src="https://github.com/algosup/2023-2024-project-3-virtual-processor-team-2/raw/main/documents/images/management/linkedin_logo.png" width="100px"> |

# Overview

This project is an ALGOSUP project, driven by students. This project aims to create an API to find the quickest path of a travel. For this project we use the program language C++. 

# What is a Quickest Path ?

The Quickest Path refers to the path in a network or graph that minimizes the total time required to travel from a starting node (source) to a destination node (target). Unlike the "shortest path," which minimizes distance or cost, the quickest path accounts for time-dependent factors such as speed, traffic, or weights on the edges that represent time rather than distance.

# What is C++ ?

C++ is a **compiled programming language** enabling programming in multiple paradigms, including procedural programming, object-oriented programming and generic programming. Its **high performance** and compatibility with the C language make it one of the most widely used programming languages for **performance-critical applications**.

# Deadlines

| Name                       | Deadline   |
|----------------------------|------------|
|**Functional Specification**| 01/20/2025 |
|**Technical Specification** | 01/27/2025 |
|**Code**                    | 01/31/2025 |
|**Test Cases / Test Plan**  | 02/07/2025 |
|**User Manual**             | 02/07/2025 |
  
# How to Use

To use our API, feel free to check the documentation here : User Manual 

# Project Documents

|Name                     | Link                                                               |
|-------------------------|--------------------------------------------------------------------|
|Functional Specifications|[FunctionalSpecifications.md](documents\FunctionalSpecifications.md)|
|Technical Specifications |[TechnicalSpecifications.md](documents\TechnicalSpecifications.md)  |
|Test Plan                |[TestPlan.md](documents\TestPlan.md)                                |
|Test Cases               |[TestCases.md](documents\TestCases.md)                                                                    |
|User Manual              |[UserManual]()                                                      |
|Settlement               |[Settlement.md](documents\management\Settlement.md)                 |
|Project Charter          |[ProjectCharter.md](documents\management\ProjectCharter.md)         |
|Weekly Report            |[Weekly Report](documents\management\weeklyreport)                  |
|Post Mortem              |[PostMortem.md](documents\management\PostMortem.md)                 |
