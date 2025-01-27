<h1 align="center"> Quickest path Test Cases </h1>

<p>
<br>

| Created by | Creation date | Last modified |
|:-------------:|:---------------:|:--------:|
| Megnan Lucas | 01/08/2025 | 01/27/2025 |

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
- [II. Prototype version](#ii-prototype-version)
  - [A. API tests](#a-api-tests)
  - [B. Integrity \& Connectivity tests](#b-integrity--connectivity-tests)
  - [C. Process tests](#c-process-tests)
  - [D. CSV files tests](#d-csv-files-tests)
  - [E. Output](#e-output)
- [III. Final version](#iii-final-version)


</details>

# I. Introduction

| Category ID | Category Name | Description | 
|------------|----------------|-------------|
| 1 | API tests                         |  |
| 2 | Integrity & Connectivity tests    |  |
| 3 | Process tests                     |  |
| 4 | CSV files tests                   |  |
| 5 | Output                            |  |

# II. Prototype version

## A. API tests

| Test Case ID | Test Case Name | Test Case Description | Expected Result | Actual Result | Tested | Priority | Pass/Fail |
|--------------|----------------|-----------------------|-----------------|---------------|--------|----------|-----------|
| **1.01** | Blank space during the Pre-prossessing. | When we add a file or a value during the set-up (y/n or a int) put a blank space before. | Nothing. | Nothing happend and we can put the file or an answer after. | Tested on Mac and Windows. | Low | ✅ |
| **1.02**| Blank space in the Localhost. | In the Localhost link, put a blank and nothing else. | Error message 400 INVALID PARAMS. | Error message 400 INVALID PARAMS. | Tested on Mac and Windows. | Low | ✅ |
| **1.03** | Put a Number Over 30,000,000 in the Request. | In the Request, put a number over 30,000,000 for one of point A or B. | send error 404 NO PATH FOUND. | send error 404 NO PATH FOUND. | Tested on Mac and Windows. | Medium | ✅ |
| **1.04** | Put a character in the Localhost. | In the Localhost link, put a character instead of a Int. | send error 400 INVALID PARAMS. | send error 400 INVALID PARAMS. | Tested on Mac and Windows. | Low | ✅ |
| **1.05** | Put a String in the Localhost. | In the Localhost link, put a string (multiple of character) instead of a Int. | send error 400 INVALID PARAMS. | send error 400 INVALID PARAMS. | Tested on Mac and Windows. | Low | ✅ |
| **1.06** | Put a Float in the Localhost. | In the Localhost link, put a float instead of a Int. | send error 400 INVALID PARAMS. | ERROR 200, the program take the int of the float (ex: input 2.3, the program take 2). | Tested on Mac and Windows. | Medium | ❌ |
| **1.07** | Put a Int & a String in the Localhost. | In the Localhost link, put a Int and a String for a landmark (ex : 12ABC). | send error 400 INVALID PARAMS. | No ERROR, the program only takes 12. | Tested on Mac and Windows. | Medium | ❌ |
| **1.08** | Put a String & a Int in the Localhost. | In the Localhost link, put a String and a Int for a landmark (ex : ABC12). | send error 400 INVALID PARAMS. | send error 400 INVALID PARAMS. | Tested on Mac and Windows. | Medium | ✅ |
| **1.09** | Put a signed number in the Localhost. | In the Localhost link, put a signed number for a landmark (ex : -5 or “-5”). | send error 400 INVALID PARAMS. | send error 400 INVALID PARAMS. | Tested on Mac and Windows. | Medium | ✅ |
| **1.10** | Put a 0 in the Localhost. | In the Localhost link, put a 0 for a landmark. | send error 404 NO PATH FOUND. | send error 404 NO PATH FOUND. | Tested on Mac and Windows. | Medium | ✅ |
| **1.11** | Input a Int in the Localhost. | Verify if the program work correctly. | The program work correctly. | The program work correctly. | Tested on Mac and Windows. | High | ✅ |
| **1.12** | Put more landmarks than nodes. | when we set up the program on the terminal, put 45 landmarks because we have 39 nodes. | Skipping X landmarks, maximum value has been reach + message. | make the pre-processor successfully and put landmarks in addition on the first landmark. | Tested on Mac and Windows. | High | ✅ |


## B. Integrity & Connectivity tests

| Test Case ID | Test Case Name | Test Case Description | Expected Result | Actual Result | Tested | Priority | Pass/Fail |
|--------------|----------------|-----------------------|-----------------|---------------|--------|----------|-----------|
| **2.1** | Wrong format. | Try to do the integrity test with a wrong format like JPEG. | Display an error message "Wrong format, please use .csv format." and ask a new file. | does not display an error message, suggests configuring the API and then closes. | Tested on Mac and windows. | Low | ❌ |
| **2.2** | Node empty. | Delete all node costs in the file. | Warning message + "Edge deleted in the graph.". | Warning message appears and the edge is deleted, but no message for this is displayed. | Tested on Mac and Windows. | Low | ❌ |
| **2.3** | Free of loops. | Verify that the file forms a Directed Acyclic Graph (DAG) and is free of loops. | Error message "integrity check resulted in an error. please ensure that your graph form a Directed Acyclic Graph (DAG) and is free of loops.". | Error message "integrity check resulted in an error. please ensure that your graph form a Directed Acyclic Graph (DAG) and is free of loops.". | Tested on Mac and Windows. | High | ✅ |
| **2.4** | Connectivity Check. | Ensure that the graph is fully connected, meaning it is possible to navigate between any two landmarks. | Error message "connectivity check resulted in an error. please ensure that the graph is fully connected, meaning it is possible to navigate between any two landmarks." | Error message "connectivity check resulted in an error. please ensure that the graph is fully connected, meaning it is possible to navigate between any two landmarks." | Tested on Mac and Windows. | High | ✅ |

## C. Process tests

| Test Case ID | Test Case Name | Test Case Description | Expected Result | Actual Result | Tested | Priority | Pass/Fail |
|--------------|----------------|-----------------------|-----------------|---------------|--------|----------|-----------|
| **3.1** | 10% Heuristics. | The returned path should not exceed the shortest path duration by more than 10%. For that we used the file test.csv. | Give the shortest path or one in the limit of 10%. | Give the shortest path or one in the limit of 10%. | Tested on Mac and Windows. | High | ✅ |

## D. CSV files tests

| Test Case ID | Test Case Name | Test Case Description | Expected Result | Actual Result | Tested | Priority | Pass/Fail |
|--------------|----------------|-----------------------|-----------------|---------------|--------|----------|-----------|
| **4.1** | Compilation and running. | Verify the program laucnh well. | The program run and initialize. | The program run and initialize. | Tested on Mac and Windows. | High | ✅ |
| **4.2** | Blanck space in the file .csv. | space between the comma and a value in the test-blank-space.csv file. | return the file with the path found. | return the file with the path found. | Tested on Mac and Windows. | Low | ✅ |
| **4.3** | input string distance in the file .csv | have a string distance value in the test-string-distance.csv file. | send an ERROR message. | send error 404 NO PATH FOUND. | Tested on Mac and Windows. | Low | ❌ |
| **4.4** | input float distance. | have a float distance value in the test-float-distance.csv file. | send an ERROR message. | It doesn’t take attention about the float and just take the int of the float (ex : 5.98, the program take 5). | Tested on Mac and Windows. | Medium | ❌ | 
| **4.5** | signed number distance. | have a signed distance value in the test-signed-number-distance.csv file. | send an ERROR message. | The pre-processing block without send ERROR message. | Tested on Mac and Windows. | Medium | ❌ |
| **4.6** | input string node. | have a string node value in the test-string-node.csv file. | send an ERROR message. | **Mac**:  message “terminating due to uncaught exception of type std::invalid_argument: stoi: no conversion”. **Windows**: Nothing, the program shutdown during the integrity check. | Tested on Mac and Windows. | Medium | ❌ |
| **4.7** | input float node. | have a float node value in the test-float-node.csv file. | send an ERROR message. | it doesn’t take a float and put it like an int. It return the path found. | Tested on Mac and Windows. | Low | ❌ |
| **4.8** | signed number node. | have a signed node value in the test-signed-node.csv file (in this file the node 2 is signed everywhere). | send an ERROR message. | **Mac**: send error 400 INVALID PARAMS (because we cannot use signed number in the URL). **Windows**: The pre-prossessing stop with the connectivity check. | Tested on Mac and Windows. | High | ❌ |
| **4.9** | signed one node value. | have only one signed node value in the test-signed-value-node.csv file (in this file the node 2 is signed only for one edge). | send an ERROR message. | **Mac**: message “connectivity check resulted in an error. please ensure that the graph is fully connected, meaning it is possible to navigate between any two landmarks.” on the pre-processing step. **Windows**: The pre-prossessing doesn’t work | Tested on Mac and Windows | Medium | ❌ |

## E. Output

| Test Case ID | Test Case Name | Test Case Description | Expected Result | Actual Result | Tested | Priority | Pass/Fail |
|--------------|----------------|-----------------------|-----------------|---------------|--------|----------|-----------|
| **5.1** | Output XML. | Verify if the program send a XML file as an output. | send a XML file | It return a XML file. | Tested on Mac and Windows. | High | ✅ |
| **5.2** | Output JSON. | Verify if the program send a JSON file as an output. | send a JSON file. | It return a JSON file. | Tested on Mac and Windows. | High | ✅ |
| **5.3** | Output another things. | Verify if the program send a another file as an output. | send error 400 INVALID ACCEPT HEADER. | send error 400 INVALID ACCEPT HEADER. | Tested on Mac and Windows. | Medium | ✅ |
| **5.4** | Output comparaison. | Compare the result of XML and JSON. | Their are the same result. | the program return the same things. | Tested on Mac and Windows. | High | ✅ |

# III. Final version