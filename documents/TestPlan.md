<h1 align="center"> SmashThePath Test Plan </h1>

<p>
<br>

| Created by | Creation date | Last modified |
|:-------------:|:---------------:|:--------:|
| Megnan Lucas | 01/06/2025 | 01/20/2025 |

<br>
</p>

## Test Plan Approval

| Role | Name | Signature | Date |
|------|------|-----------|------|
| Project Manager      | [Alexis SANTOS](https://github.com/Mamoru-fr)         |  |  |
| Technical Lead       | [Grégory PAGNOUX](https://github.com/Gregory-Pagnoux) |  |  |
| Quality Assurance    | [Lucas MEGNAN](https://github.com/LucasMegnan)        |  |  |

<details>
<summary>

### Table Of Content

</summary>

- [I. Introduction](#i-introduction)
  - [A. Test Objectives](#a-test-objectives)
  - [B. Scope of Testing](#b-scope-of-testing)
  - [C. Product Overview](#c-product-overview)
  - [D. Definitions / Acronyms](#d-definitions--acronyms)
    - [1. Definitions](#1-definitions)
    - [2. Acronyms](#2-acronyms)
  - [E. References](#e-references)
- [II. Plan](#ii-plan)
  - [A. Test Team](#a-test-team)
  - [B. Team Reviews](#b-team-reviews)
  - [C. Tasks and Deliverables](#c-tasks-and-deliverables)
  - [D. Environmental Needs](#d-environmental-needs)
    - [1. Test Environment](#1-test-environment)
      - [a. Hardware](#a-hardware)
      - [b. Network](#b-network)
      - [c. Software](#c-software)
    - [2. Test Lab](#2-test-lab)
- [III. Approach](#iii-approach)
  - [A. Assumptions / Contraints](#a-assumptions--contraints)
    - [1. Assumptions](#1-assumptions)
    - [2. Contraints](#2-contraints)
  - [B. Test Tools](#b-test-tools)
  - [C. Test Data](#c-test-data)
- [IV. Testing Procedures](#iv-testing-procedures)
  - [A. Test Execution](#a-test-execution)
    - [1. Test Cases](#1-test-cases)
    - [2. Order of Testing](#2-order-of-testing)
  - [B. Pass / Fail criteria](#b-pass--fail-criteria)
  - [C. Suspension Criteria and Resumption Requirements](#c-suspension-criteria-and-resumption-requirements)
    - [1. Normal Criteria](#1-normal-criteria)
    - [2. Abnormal Criteria](#2-abnormal-criteria)
  - [D. Defect Management](#d-defect-management)

</details>

# I. Introduction

## A. Test Objectives

The system integration test of the quickest path must validate the following requirements:

- **Functional Accuracy**:
  - The returned path should not exceed the shortest path duration by more than 10%.
  - The API must handle all queries within 1 second on a typical laptop.
- **Data Integrity**:
  - Verify that the file forms a Directed Acyclic Graph (DAG) and is free of loops.
  - Ensure that the graph is fully connected, meaning it is possible to navigate between any two landmarks.
  - A utility to verify the integrity of the provided CSV file. 
- **Versatility**:
  - Demonstrating the ability to handle multiple formats (XML and JSON).

## B. Scope of Testing

The scope is to **develop and validate a high-performance algorithm in C++** that calculates the quickest path between two landmarks in the United States. This includes:

- **Functional** testing of the REST API.
- **Performance** testing to ensure adherence to time constraints.
- **Data validation** to verify the **integrity** of the dataset.
- **Compatibility** testing across different operating systems (Windows and macOS).

## C. Product Overview

Our software enginner aim to make **travel easier** with faster pathing using the language C++ and the software Visual Studio. 
The users can use this to gain time and travel more effectively and easily across the United States.

## D. Definitions / Acronyms

### 1. Definitions

### 2. Acronyms

| Acronyms | Definition |
|----------|------------|
| QA | Quality Assurance |
| TL | Technical Leader |
| SE | Software Engineer |
| PR | Pull Request |
| OS | Operating System |
| IDE | Integrated Development Environment |

## E. References

# II. Plan

## A. Test Team

The following people will be on the system integration test team:

| Role | Name | Level of involvement | Responsibilities |
|------|------|----------------------|------------------|
| Technical Leader     | Grégory Pagnoux | 5hrs  |  |
| Project Manager      | Alexis Santos   | 4hrs  |  |
| Quality Assurance    | Lucas MEGNAN    | 93hrs |  |
| Software Enginner    | Yann-maël       | 4hrs  |  |

## B. Team Reviews

The following reviews will be conducted by the QA, and for the developement part the TL and SE will support the QA. 
Refer to the work schedule for the planned review dates.

- Settlement review.
- Project Charter review.
- review the Management Documentation of every weeks.
- Functional Specifications review.
- Technical Specifications review.
- Code review.
- User Manual review.
- Test plan review.
- Test case review.
- Post Mortem review.

## C. Tasks and Deliverables

| Task | Start | Stop | Deliveable(s) |
|------|-------|------|---------------|
| Issue template | 01/09/2025 | 01/10/2025 | Issue on GitHub |
| PR template | 01/16/2025 | 01/17/2025 | Merge branch on GitHub |
| Test case design |  |  | Test cases |
| Build 1: Data integrity | 01/20/2025 | 01/21/2025 | Build 1 tested |
| Build 2: Functional accuracy | 01/21/2025 | 01/22/2025 |  Build 2 tested |
| Build 3: Versatility | 01/22/2025 | 01/23/2025 | Build 3 tested |

## D. Environmental Needs

### 1. Test Environment

#### a. Hardware

Our team members have different computer OS, which are Windows and MacOs, that gives us the opportunity of testing that the program is working properly on the different OS. But also the environment is different and so the steps to be capable to test are different.

The tests will be run on the following environments:

(1) Apple MacBook Air M3 2024
- Apple M3
- 16 GB
- macOS Sequoia 15.1.1

(1) Lenovo ThinkBook 14	
- Intel Core i7-1065G7
- 16 GB	
- Windows 11 Pro 21H2	

#### b. Network

Wifi: 

#### c. Software

IDE Visual Studio

### 2. Test Lab

The following items will be needed full-time by the system integration test team:

- 1 whiteboard (large) with markers and erasers.
- 1 screen & a HDMI wire to share informations from computers.

# III. Approach

## A. Assumptions / Contraints

### 1. Assumptions

### 2. Contraints

## B. Test Tools

GitHub Issue 

// TODO

## C. Test Data

# IV. Testing Procedures

## A. Test Execution

### 1. Test Cases

For each requirement, system feature, or algorithm to be tested, the tester will execute a set of pre-defined test cases.
Each test case will have a series of actions and expected results. 
As each action is performed, the results are evaluated.
If the observed results are equal to the expected results, a checkmark ✅ is placed in the “Pass/Fail” column.
If the observed results are not equal to the expected results, a checkmark ❌ is placed in the “Pass/Fail” column.

### 2. Order of Testing

The order of testing will be driven primarily by the build order. Within each build, the following
order of testing will be followed:

- Build 1: Data integrity
  - A. Verify that the file forms a Directed Acyclic Graph (DAG) and is free of loops.
  - B. Ensure that the graph is fully connected, meaning it is possible to navigate between any two landmarks.
- Build 2: Funtional accuracy
  - A. The returned path should not exceed the shortest path duration by more than 10%.
  - B. The API must handle all queries within 1 second on a typical laptop.
- Build 3: Versatility
  - A. Demonstrating the ability to handle multiple formats (XML and JSON).

## B. Pass / Fail criteria

To pass the system integration test for the shortest path calculation software, the following criteria must be met:

**Core Functionality**:
- The system can calculate the quickest path between any two landmarks in a bidirectional positivly weighted graph within the performance goals.
- The system exposes its functionality via a REST API with:
  - Correct handling of input landmark IDs.
  - Accurate travel time and ordered path output in XML and JSON formats.
  - The system supports bidirectional connections between landmarks as per the provided dataset.

**Data Integrity**:
- The system ensures graph validation, confirming that the dataset (.csv) forms a Directed Acyclic Graph (DAG) without loops.
- After transforming the graph in an Undirected Cyclic Graph (UCG) perform a connectivity check to guarantee the graph allows navigation between any two landmarks.

**Performance**:
- The API must respond to all queries within one second on a typical laptop.
- The system adheres to the 10% approximation rule, ensuring that the travel time does not exceed 10% of the shortest path duration.

**Ease of Use**:
- The REST API documentation is complete and user-friendly, enabling developers to integrate with minimal effort.
- Error messages are informative and assist in troubleshooting input issues.

**Security and Reliability**:
- Adequate measures prevent unauthorized access to the system.
- The system is resilient to failures, supporting recovery through documented procedures.

**Integration and Extensibility**:
- The system can integrate seamlessly with external tools or systems that utilize the REST API.
- The design allows future enhancements without extensive rework.

**Testing**:
- All test cases in the provided suite for correctness, performance, and compliance with the 10% approximation rule are passed.
- Validation tools confirm the integrity of the dataset without errors.

## C. Suspension Criteria and Resumption Requirements

### 1. Normal Criteria

### 2. Abnormal Criteria

## D. Defect Management