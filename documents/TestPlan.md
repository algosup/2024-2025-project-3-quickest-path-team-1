<h1 align="center"> SmashThePath Test Plan </h1>

<p>
<br>

| Created by | Creation date | Last modified |
|:-------------:|:---------------:|:--------:|
| Megnan Lucas | 01/06/2025 | 02/07/2025 |

<br>
</p>

## Test Plan Approval

| Role | Name | Signature | Date |
|------|------|-----------|------|
| Project Manager      | [Alexis SANTOS](https://github.com/Mamoru-fr)         | ✅ | 02/07/2025 |
| Technical Lead       | [Grégory PAGNOUX](https://github.com/Gregory-Pagnoux) | ✅ | 02/07/2025 |
| Quality Assurance    | [Lucas MEGNAN](https://github.com/LucasMegnan)        | ✅ | 02/07/2025 |

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
  - [D. Testing Tools](#d-testing-tools)
    - [1. Test Environment](#1-test-environment)
      - [a. Hardware](#a-hardware)
      - [b. Network](#b-network)
      - [c. Software](#c-software)
    - [2. Test Lab](#2-test-lab)
- [III. GitHub Issues \& Pull Request](#iii-github-issues--pull-request)
  - [A. GitHub Issues](#a-github-issues)
  - [B. Pull Request](#b-pull-request)
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

| Words | Definition | Sources |
|-------|------------|---------|
| **API**      | API stands for application programming interface, which is a set of definitions and protocols for building and integrating application software. | [Red Hat API](https://www.redhat.com/en/topics/api/what-are-application-programming-interfaces) |
| **REST API** | A REST API is an application programming interface (API) that follows the design principles of the REST architectural style. REST is short for representational state transfer, and is a set of rules and guidelines about how you should build a web API. | [Red Hat REST API](https://www.redhat.com/en/topics/api/what-is-a-rest-api) |
| **Directed Acyclic Graph (DAG)**  | It consists of vertices and edges (also called arcs), with each edge directed from one vertex to another, such that following those directions will never form a closed loop. A directed graph is a DAG if and only if it can be topologically ordered, by arranging the vertices as a linear ordering that is consistent with all edge directions. | [Wikipedia DAG](https://en.wikipedia.org/wiki/Directed_acyclic_graph) |
| **Undirected Cyclic Graph (UCG)** | In an undirected cyclic graph, the edges have no direction, and the cycle can go in any direction. | [GeeksforGeeks](https://www.geeksforgeeks.org/what-is-cyclic-graph/) |
| **Comma-Separated Values (CSV)**  | Comma-separated values (CSV) is a text file format that uses commas to separate values, and newlines to separate records. A CSV file stores tabular data (numbers and text) in plain text, where each line of the file typically represents one data record. Each record consists of the same number of fields, and these are separated by commas in the CSV file. If the field delimiter itself may appear within a field, fields can be surrounded with quotation marks. | [Wikipedia CSV](https://en.wikipedia.org/wiki/Comma-separated_values) |
| **Extensible Markup Language (XML)**  | Extensible Markup Language (XML) is a markup language and file format for storing, transmitting, and reconstructing data. | [Wikipedia XML](https://en.wikipedia.org/wiki/XML) |
| **JavaScript Object Notation (JSON)** | Open standard file format and data interchange format that uses human-readable text to store and transmit data objects consisting of name–value pairs and arrays (or other serializable values). It is a commonly used data format with diverse uses in electronic data interchange, including that of web applications with servers. | [Wikipedia JSON](https://en.wikipedia.org/wiki/JSON) |
| **C++**      | a high-level, general-purpose programming language created by Danish computer scientist Bjarne Stroustrup. First released in 1985 as an extension of the C programming language, it has since expanded significantly over time; as of 1997, C++ has object-oriented, generic, and functional features, in addition to facilities for low-level memory manipulation for systems like microcomputers or to make operating systems like Linux or Windows. It is usually implemented as a compiled language, and many vendors provide C++ compilers, including the Free Software Foundation, LLVM, Microsoft, Intel, Embarcadero, Oracle, and IBM. | [Wikipedia C++](https://en.wikipedia.org/wiki/C%2B%2B) |
| **Insomnia** | Insomnia is a powerful REST client that allows you to send HTTP requests and view responses from REST APIs. It is designed to be easy to use and helps developers debug and test their APIs more effectively. With Insomnia, you can create and save requests, organize them into workspaces, and share them with others. | [ProAbano Insomnia](https://docs.proabono.com/documentation/api-overview/what-is-insomnia-the-api-rest-client/) |

### 2. Acronyms

| Acronyms | Definition |
|----------|------------|
| QA | Quality Assurance  |
| TL | Technical Leader   |
| SE | Software Engineer  |
| PR(s) | Pull Request(s) |
| OS | Operating System   |
| VS | Visual Studio      |
| IDE | Integrated Development Environment |
| API | Application Programming Interface  |

## E. References

# II. Plan

## A. Test Team

The following people will be on the system integration test team:

| Role | Name | Level of involvement | Responsibilities |
|------|------|----------------------|------------------|
| Technical Leader     | Grégory Pagnoux | 5hrs  | Help the QA to reviews the code and validate PR from the branch dev. Help to do test cases. |
| Project Manager      | Alexis Santos   | 4hrs  | Reviews all documents when a PR is ask from the branch document with the QA. Help to do test cases. |
| Quality Assurance    | Lucas MEGNAN    | 93hrs | Review code and documents, prepare and doing test cases, make template for PR and Issues, make Issues if necessary and validate PRs. |

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
| Issue template                          | 01/09/2025 | 01/10/2025 | Issue on GitHub        |
| PR template                             | 01/16/2025 | 01/17/2025 | Merge branch on GitHub |
| Test case design                        | 01/17/2025 | 01/20/2025 | Test cases             |
| Build 1: API tests                      | 01/20/2025 | 01/21/2025 | Build 1 tested         |
| Build 2: Integrity & Connectivity tests | 01/21/2025 | 01/22/2025 | Build 2 tested         |
| Build 3: Process tests                  | 01/22/2025 | 01/23/2025 | Build 3 tested         |
| Build 4: CSV files tests                | 01/23/2025 | 01/23/2025 | Build 4 tested         |
| Build 5: Output                         | 01/23/2025 | 01/23/2025 | Build 5 tested         |

## D. Testing Tools

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

A Wifi with a good and stable connection.

#### c. Software

- IDE Visual Studio
- VS code
- Insomnia
- g++ / clang++

### 2. Test Lab

The following items will be needed full-time by the system integration test team:

- 1 whiteboard (large) with markers and erasers.
- 1 screen & a HDMI wire to share informations from computers.

# III. GitHub Issues & Pull Request

## A. GitHub Issues

GitHub Issues are used to track, manage, and resolve defects or enhancements. The following process must be followed when opening an issue:

1. **Issue Creation**:
   - Each issue must have a clear and descriptive title.
   - Provide a detailed description of the problem, including steps to reproduce it if applicable.
   - Assign the issue to the relevant team members and add appropriate labels (e.g., bug, enhancement, documentation).

2. **Issue Triage**:
   - The QA or technical lead will review issues and prioritize them based on severity and impact.
   - High-priority issues should be addressed before moving on to new feature development.

3. **Issue Resolution**:
   - Developers must provide comments and updates on the issue’s progress.
   - Once fixed, the issue should be linked to the corresponding pull request.

4. **Issue Closure**:
   - After verification and testing, the issue is marked as resolved and closed by the QA.
   - If the issue persists, it should be reopened and assigned for further investigation.

Template of **document issue**:

![docIssue](images/docIssue.png)

Template of **code issue**:

![bugIssue](images/bugIssue.png)

## B. Pull Request

Pull requests are used to propose, review, and merge changes into the main codebase. The following process must be followed when submitting a PR:

1. **Branching**: Developers must create a new feature or bug-fix branch from the latest main branch before making any modifications.

2. **Code Commit Guidelines**: Each commit should be atomic, well-documented, and follow the project's coding standards.

3. **Pull Request Creation**:
   - PRs must include a clear title and description summarizing the changes.
   - Link any relevant issues that the PR addresses.
   - Ensure that all unit and integration tests pass before submission.

4. **Review Process**:
   - At least one reviewer (QA or technical lead) must approve the PR before merging.
   - Reviewers should check for correctness, readability, adherence to coding standards, and the absence of regressions.
   - Any requested changes must be addressed before approval.

5. **Merging**:
   - Once approved, the developer or the reviewer can merge the PR.
   - Squash and merge should be preferred for small changes; standard merge for larger features.
   - Delete the feature branch after merging to keep the repository clean.

6. **Post-Merge Actions**:
   - Ensure the latest changes are pulled into local environments.
   - Monitor production (if applicable) for unexpected issues after deployment.

Template of **PR**:

![PullRequest](images/pullRequest.png)

# IV. Testing Procedures

## A. Test Execution

### 1. Test Cases

For each requirement, system feature, or algorithm to be tested, the tester will execute a set of pre-defined test cases.
Each test case will have a series of actions and expected results.
As each action is performed, the results are evaluated.
If the observed results are equal to the expected results, a checkmark ✅ is placed in the “Pass/Fail” column.
If the observed results are not equal to the expected results, a checkmark ❌ is placed in the “Pass/Fail” column.

We've used various CSV files for the test cases, which are available in the test-csv-file folder. When a file is used for a test case, you can see which file is being used in the category description.

### 2. Order of Testing

The order of testing will be driven primarily by the build order. Within each build, the following
order of testing will be followed:

- **Build 1**: Integrity & Connectivity
  - A. Ensure that the data structures are correctly formed and all entries are valid.
  - B. Verify that the test environment correctly loads and processes input data.

- **Build 2**: API Functionality
  - A. Validate API responses against expected outputs.
  - B. Ensure query response times are within performance thresholds.

- **Build 3**: Process Testing
  - A. Verify end-to-end processing of inputs through the system.
  - B. Confirm the correctness of transformation and computation logic.

- **Build 4**: CSV File Handling
  - A. Test proper parsing and loading of CSV data files.
  - B. Validate error handling for malformed CSV entries.

- **Build 5**: Output Validation
  - A. Ensure final outputs conform to expected formatting and accuracy requirements.
  - B. Compare actual results with predefined expected values.

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

At the end of each day (5:00 p.m.) testing will be suspended.
At that time, all test cases executed during the day should be marked as such.
The system integration test team will initiate a backup routine to save the day’s updated test files.

When all test cases have been executed, the test will be suspended and the results documented in the test cases document.

### 2. Abnormal Criteria

As a general guideline, if the defect backlog continually increases over a ten days period, testing should be suspended.
This will allow the developers time to fix existing defects without the pressure and confusion of new defects being added to the backlog.

When a change is being migrated to the test environment, the system integration test team leader must be notified in advance to schedule a time for the move.
After the move has been completed, a retest of previously tested functions should be performed.

If a critical processing unit is found to have severe defects (as defined by the defect reporting process), testing should be suspended until the defects have been fixed.
When the fixed unit is moved back into the test environment, any previously performed tests that affect the unit should be performed again to ensure new defects were not created as a result of the fix.

## D. Defect Management

The test team will use the designated defect tracking system for reporting, maintaining, tracking, and overall management of defects. Change management procedures have been developed and are described in the test plan.

- **High**: Defects that critically impact system functionality. Testing should not proceed to the next phase until corrective measures have been implemented.

- **Medium**: Defects that result in incorrect or invalid information. Examples include miscalculation of payroll values or improper data validation, which may lead to data corruption.

- **Low**: Defects that are cosmetic in nature and do not impact functionality. Examples include UI misalignment or minor deviations from design specifications.
