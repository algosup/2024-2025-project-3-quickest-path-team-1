<h1 align="center"> Quickest path Test Plan </h1>

<p>
<br>

| Created by | Creation date | Last modified |
|:-------------:|:---------------:|:--------:|
| Megnan Lucas | 01/06/2025 | 01/08/2025 |

<br>
</p>

## Test Plan Approval

| Role | Name | Signature | Date |
|------|------|-----------|------|
| Project Manager      | [Alexis SANTOS](https://github.com/Mamoru-fr)         |  | |
| Technical Lead       | [Grégory PAGNOUX](https://github.com/Gregory-Pagnoux) |  | |
| Quality Assurance    | [Lucas MEGNAN](https://github.com/LucasMegnan)        |  | |

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

The system integration test of the quickest path should validate the requirements perspective like:

- The returned path should not exceed the shortest path duration by more than 10%.
- The API must handle all queries within 1 second on a typical laptop.
- Big O complexity ...
- Verify that the file forms a Directed Acyclic Graph (DAG) and is free of loops.
- Ensure that the graph is fully connected, meaning it is possible to navigate between any two landmarks.
- Demonstrating the ability to handle multiple formats (XML and JSON).
- A utility to verify the integrity of the provided CSV file. 

## B. Scope of Testing

Our scope is to have a high performant software solution that calculate two landmarks in the US. 
Our algorithm needs to include a research system to find the quickest path between this two landmarks.

## C. Product Overview

## D. Definitions / Acronyms

### 1. Definitions

### 2. Acronyms

# II. Plan

## A. Test Team

The following people will be on the system integration test team:

| Role | Name | Level of involvement | Responsibilities |
|------|------|----------------------|------------------|
| Project Manager      | Alexis Santos   | Xhrs  |  |
| Technical Lead       | Grégory Pagnoux | 1hrs  |  |
| Quality Assurance    | Lucas MEGNAN    | 93hrs |  |
<!-- | Software Enginner    | Yann-maël       | Xhrs  |  | -->

## B. Team Reviews

The following reviews will be conducted by the entire test team and the QA. 
Refer to the work schedule for the planned review dates.

• Settlement review.
• Project Charter review.
• review the Management Documentation of every weeks.
• Functional Specifications review.
• Technical Specifications review.
• Code review.
• User Manual review.
• Test plan review.
• Test case review.
<!-- • Test progress review. -->
• Post Mortem review.

## C. Tasks and Deliverables

## D. Environmental Needs

### 1. Test Environment

#### a. Hardware

All test cases will be executed on the SE's and QA's computer.

(1) Mac

(1) Windows

#### b. Network

Wifi: 

#### c. Software

IDE Visual Studio
VScode

### 2. Test Lab

The following items will be needed full-time by the system integration test team:

• 1 whiteboard (large) with markers and erasers.
• 1 screen & a HDMI wire to share informations from computers.

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

## B. Pass / Fail criteria

To pass the system integration test, the following criteria must be met:

## C. Suspension Criteria and Resumption Requirements

### 1. Normal Criteria

### 2. Abnormal Criteria

## D. Defect Management