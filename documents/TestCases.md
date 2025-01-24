<h1 align="center"> Quickest path Test Cases </h1>

<p>
<br>

| Created by | Creation date | Last modified |
|:-------------:|:---------------:|:--------:|
| Megnan Lucas | 01/08/2025 | 01/23/2025 |

<br>
</p>

## Test Cases Approval

| Role | Name | Signature | Date |
|------|------|-----------|------|
| Project Manager      | [Alexis SANTOS](https://github.com/Mamoru-fr)                 |  |  |
| Technical Lead       | [Grégory PAGNOUX](https://github.com/Gregory-Pagnoux)         |  |  |
| Quality Assurance    | [Lucas MEGNAN](https://github.com/LucasMegnan)                |  |  |

---

<details>
<summary>

### Details

</summary>

- [I. Introduction](#i-introduction)
  - [A. API tests](#a-api-tests)
  - [B. Integrity \& Connectivity tests](#b-integrity--connectivity-tests)
  - [C. Process tests](#c-process-tests)
  - [D. CSV files tests](#d-csv-files-tests)


</details>

# I. Introduction

| Category ID | Category Name | Description | 
|------------|----------------|-------------|
| 1 | API tests                         |  |
| 2 | Integrity & Connectivity tests    |  |
| 3 | Process tests                     |  |
| 4 | CSV files tests                   |  |

## A. API tests

| Test Case ID | Test Case Name | Test Case Description | Expected Result | Actual Result | Tested | Priority | Pass/Fail |
|--------------|----------------|-----------------------|-----------------|---------------|--------|----------|-----------|
| **1.1** | Blank space during the Pre-prossessing | When we add a file or a value during the set-up (y/n or a int) put a blank space before | Nothing | Nothing append and we can put the file or an answer after. | Tested on Mac and Windows. | Low | ✅ |
| **1.2**| Blank space in the Localhost | In the Localhost link, put a blank and nothing else. | Error message 400 INVALID PARAMS. | Error message 400 INVALID PARAMS. | Tested on Mac and Windows. | Low | ✅ |
| **1.3** | Put a Number Over 30,000,000 in the Request | In the Request, put a number over 30,000,000 for one of point A or B. | send error 404 NO PATH FOUND | send error 404 NO PATH FOUND | Tested on Mac and Windows | Medium | ✅ |
| **1.4** | Put a character in the Localhost | In the Localhost link, put a character instead of a Int. | send error 400 INVALID PARAMS | send error 400 INVALID PARAMS | Tested on Mac and Windows. | Low | ✅ |
| **1.5** | Put a String in the Localhost | In the Localhost link, put a string (multiple of character) instead of a Int. | send error 400 INVALID PARAMS | send error 400 INVALID PARAMS | Tested on Mac and Windows. | Low | ✅ |


## B. Integrity & Connectivity tests

| Test Case ID | Test Case Name | Test Case Description | Expected Result | Actual Result | Tested | Priority | Pass/Fail |
|--------------|----------------|-----------------------|-----------------|---------------|--------|----------|-----------|
| **2.1** | Wrong format | Try to do the integrity test with a wrong format like JPEG | Display an error message "Wrong format, please use .csv format." and ask a new file. | does not display an error message, suggests configuring the API and then closes. | Tested on Mac and windows. | Low | ❌ |
| **2.2** | Node empty | Delete all node costs in the file | Warning message + "Edge deleted in the graph.". | Warning message appears and the edge is deleted, but no message for this is displayed. | Tested on Mac and Windows. | Low | ❌ |
| **2.3** | Free of loops | Verify that the file forms a Directed Acyclic Graph (DAG) and is free of loops. | Error message "integrity check resulted in an error. please ensure that your graph form a Directed Acyclic Graph (DAG) and is free of loops." | Error message "integrity check resulted in an error. please ensure that your graph form a Directed Acyclic Graph (DAG) and is free of loops." | Tested on Mac and Windows. | High | ✅ |
| **2.4** | Connectivity Check | Ensure that the graph is fully connected, meaning it is possible to navigate between any two landmarks. | Error message "connectivity check resulted in an error. please ensure that the graph is fully connected, meaning it is possible to navigate between any two landmarks." | Error message "connectivity check resulted in an error. please ensure that the graph is fully connected, meaning it is possible to navigate between any two landmarks." | Tested on Mac and Windows. | High | ✅ |

## C. Process tests

| Test Case ID | Test Case Name | Test Case Description | Expected Result | Actual Result | Tested | Priority | Pass/Fail |
|--------------|----------------|-----------------------|-----------------|---------------|--------|----------|-----------|
| **3.1** | 10% Heuristics | The returned path should not exceed the shortest path duration by more than 10%. For that we used the file test.csv. | Give the shortest path or one in the limit of 10%. | Give the shortest path or one in the limit of 10%. | Tested on Mac and Windows. | High | ✅ |

## D. CSV files tests

| Test Case ID | Test Case Name | Test Case Description | Expected Result | Actual Result | Tested | Priority | Pass/Fail |
|--------------|----------------|-----------------------|-----------------|---------------|--------|----------|-----------|
| **4.1** | Compilation and running | Verify the program laucnh well. | The program run and initialize. | The program run and initialize. | Tested on Mac and Windows. | High | ✅ |
| **4.2** | Blanck space in the file .csv | space between the comma and a value in the test-blank-space.csv file. | return the file with the path found. | return the file with the path found. | Tested on Mac and Windows. | Low | ✅ |