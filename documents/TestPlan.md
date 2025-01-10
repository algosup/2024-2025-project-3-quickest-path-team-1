<h1 align="center"> SmashThePath Test Plan </h1>

<p>
<br>

| Created by | Creation date | Last modified |
|:-------------:|:---------------:|:--------:|
| Megnan Lucas | 01/06/2025 | 01/10/2025 |

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
- **Versatility**:
  - Demonstrating the ability to handle multiple formats (XML and JSON).
  - A utility to verify the integrity of the provided CSV file. 
- **Algorithm Performance**:
  - Evaluate the Big O complexity and ensure it meets the project goals.

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

# II. Plan

## A. Test Team

The following people will be on the system integration test team:

| Role | Name | Level of involvement | Responsibilities |
|------|------|----------------------|------------------|
| Project Manager      | Alexis Santos   | Xhrs  |  |
| Technical Leader       | Grégory Pagnoux | 1hrs  |  |
| Quality Assurance    | Lucas MEGNAN    | 93hrs |  |
<!-- | Software Enginner    | Yann-maël       | Xhrs  |  | -->

## B. Team Reviews

The following reviews will be conducted by minimal 1 person of the test team (Project Manager or Technical Leader) and the QA. 
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
- Intel Core i7-1255U	
- 16 GB	
- Windows 11 Pro 22H2	

#### b. Network

Wifi: 

#### c. Software

IDE Visual Studio
VScode

### 2. Test Lab

The following items will be needed full-time by the system integration test team:

- 1 whiteboard (large) with markers and erasers.
- 1 screen & a HDMI wire to share informations from computers.

# III. Approach

## A. Assumptions / Contraints

### 1. Assumptions

### 2. Contraints

## B. Test Tools



## C. Test Data

# IV. Testing Procedures

## A. Test Execution

### 1. Test Cases

For each requirement, system feature, or algorithm to be tested, the tester will execute a set of pre-defined test cases.
Each test case will have a series of actions and expected results. 
As each action is performed, the results are evaluated.
If the observed results are equal to the expected results, a checkmark is placed in the “pass” column.
If the observed results are not equal to the expected results, a checkmark is placed in the “fail” column.

### 2. Order of Testing

The order of testing will be driven primarily by the build order. Within each build, the following
order of testing will be followed:

## B. Pass / Fail criteria

To pass the system integration test, the following criteria must be met:

## C. Suspension Criteria and Resumption Requirements

### 1. Normal Criteria

### 2. Abnormal Criteria

## D. Defect Management