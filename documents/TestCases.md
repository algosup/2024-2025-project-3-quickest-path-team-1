<h1 align="center"> Quickest path Test Cases </h1>

<p>
<br>

| Created by | Creation date | Last modified |
|:-------------:|:---------------:|:--------:|
| Megnan Lucas | 01/08/2025 | 02/07/2025 |

<br>
</p>

## Test Cases Approval

| Role | Name | Signature | Date |
|------|------|-----------|------|
| Project Manager      | [Alexis SANTOS](https://github.com/Mamoru-fr)                 |  |  |
| Technical Lead       | [Grégory PAGNOUX](https://github.com/Gregory-Pagnoux)         |  |  |
| Quality Assurance    | [Lucas MEGNAN](https://github.com/LucasMegnan)                | ✅ | 02/07/2025 |

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
  - [A. API tests](#a-api-tests-1)
  - [B. Integrity \& Connectivity tests](#b-integrity--connectivity-tests-1)
  - [C. Process tests](#c-process-tests-1)
  - [D. CSV files tests](#d-csv-files-tests-1)
  - [E. Output](#e-output-1)


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
| **1.03** | Put a Number Over 30,000,000 in the Request. | In the Request, put a number over 30,000,000 for one of point A or B. | Send error 404 NO PATH FOUND. | Send error 404 NO PATH FOUND. | Tested on Mac and Windows. | Medium | ✅ |
| **1.04** | Put a character in the Localhost. | In the Localhost link, put a character instead of a Int. | Send error 400 INVALID PARAMS. | Send error 400 INVALID PARAMS. | Tested on Mac and Windows. | Low | ✅ |
| **1.05** | Put a String in the Localhost. | In the Localhost link, put a string (multiple of character) instead of a Int. | Send error 400 INVALID PARAMS. | Send error 400 INVALID PARAMS. | Tested on Mac and Windows. | Low | ✅ |
| **1.06** | Put a Float in the Localhost. | In the Localhost link, put a float instead of a Int. | Send error 400 INVALID PARAMS. | ERROR 200, the program take the int of the float (ex: input 2.3, the program take 2). | Tested on Mac and Windows. | Medium | ❌ |
| **1.07** | Put a Int & a String in the Localhost. | In the Localhost link, put a Int and a String for a landmark (ex : 12ABC). | Send error 400 INVALID PARAMS. | No ERROR, the program only takes 12. | Tested on Mac and Windows. | Medium | ❌ |
| **1.08** | Put a String & a Int in the Localhost. | In the Localhost link, put a String and a Int for a landmark (ex : ABC12). | Send error 400 INVALID PARAMS. | Send error 400 INVALID PARAMS. | Tested on Mac and Windows. | Medium | ✅ |
| **1.09** | Put a signed number in the Localhost. | In the Localhost link, put a signed number for a landmark (ex : -5 or “-5”). | Send error 400 INVALID PARAMS. | Send error 400 INVALID PARAMS. | Tested on Mac and Windows. | Medium | ✅ |
| **1.10** | Put a 0 in the Localhost. | In the Localhost link, put a 0 for a landmark. | Send error 404 NO PATH FOUND. | Send error 404 NO PATH FOUND. | Tested on Mac and Windows. | Medium | ✅ |
| **1.11** | Input a Int in the Localhost. | Verify if the program work correctly. | The program work correctly. | The program work correctly. | Tested on Mac and Windows. | High | ✅ |
| **1.12** | Put more landmarks than nodes. | When we set up the program on the terminal, put 45 landmarks because we have 39 nodes. | Skipping X landmarks, maximum value has been reach + message. | Make the pre-processor successfully and put landmarks in addition on the first landmark. | Tested on Mac and Windows. | High | ❌ |


## B. Integrity & Connectivity tests

| Test Case ID | Test Case Name | Test Case Description | Expected Result | Actual Result | Tested | Priority | Pass/Fail |
|--------------|----------------|-----------------------|-----------------|---------------|--------|----------|-----------|
| **2.1** | Wrong format. | Try to do the integrity test with a wrong format like JPEG. | Display an error message "Wrong format, please use .csv format." and ask a new file. | Doesn't display an error message, suggests configuring the API and then closes. | Tested on Mac and windows. | Low | ❌ |
| **2.2** | Graph empty. | Delete all node costs in the file. | Warning message + "The graph is empty.". | Warning message appears and the edge is deleted, but no message for this is displayed. | Tested on Mac and Windows. | Low | ❌ |
| **2.3** | Free of loops. | Verify that the file forms a Directed Acyclic Graph (DAG) and is free of loops. | Error message "integrity check resulted in an error. Please ensure that your graph form a Directed Acyclic Graph (DAG) and is free of loops.". | Error message "integrity check resulted in an error. Please ensure that your graph form a Directed Acyclic Graph (DAG) and is free of loops.". | Tested on Mac and Windows. | High | ✅ |
| **2.4** | Connectivity Check. | Ensure that the graph is fully connected, meaning it is possible to navigate between any two landmarks. | Error message "connectivity check resulted in an error. Please ensure that the graph is fully connected, meaning it is possible to navigate between any two landmarks." | Error message "connectivity check resulted in an error. Please ensure that the graph is fully connected, meaning it is possible to navigate between any two landmarks." | Tested on Mac and Windows. | High | ✅ |

## C. Process tests

| Test Case ID | Test Case Name | Test Case Description | Expected Result | Actual Result | Tested | Priority | Pass/Fail |
|--------------|----------------|-----------------------|-----------------|---------------|--------|----------|-----------|
| **3.1** | 10% Heuristics. | The returned path should not exceed the shortest path duration by more than 10%. For that we used the file test.csv. | Give the shortest path or one in the limit of 10%. | Give the shortest path or one in the limit of 10%. | Tested on Mac and Windows. | High | ✅ |

## D. CSV files tests

| Test Case ID | Test Case Name | Test Case Description | Expected Result | Actual Result | Tested | Priority | Pass/Fail |
|--------------|----------------|-----------------------|-----------------|---------------|--------|----------|-----------|
| **4.01** | Compilation and running. | Verify the program laucnh well. | The program run and initialize. | The program run and initialize. | Tested on Mac and Windows. | High | ✅ |
| **4.02** | Blanck space in the file .csv. | Space between the comma and a value in the test-blank-space.csv file. | return the file with the path found. | Return the file with the path found. | Tested on Mac and Windows. | Low | ✅ |
| **4.03** | Input string distance in the file .csv. | Have a string distance value in the test-string-distance.csv file. | Send an ERROR message. | Send error 404 NO PATH FOUND. | Tested on Mac and Windows. | Low | ❌ |
| **4.04** | Input float distance. | Have a float distance value in the test-float-distance.csv file. | Send an ERROR message. | It doesn’t take attention about the float and just take the int of the float (ex : 5.98, the program take 5). | Tested on Mac and Windows. | Medium | ❌ | 
| **4.05** | Signed number distance. | Have a signed distance value in the test-signed-number-distance.csv file. | Send an ERROR message. | The pre-processing block without send ERROR message. | Tested on Mac and Windows. | Medium | ❌ |
| **4.06** | Input string node. | Have a string node value in the test-string-node.csv file. | Send an ERROR message. | **Mac**:  Message “terminating due to uncaught exception of type std::invalid_argument: stoi: no conversion”. **Windows**: Nothing, the program shutdown during the integrity check. | Tested on Mac and Windows. | Medium | ❌ |
| **4.07** | Input float node. | Have a float node value in the test-float-node.csv file. | Send an ERROR message. | It doesn’t take a float and put it like an int. It return the path found. | Tested on Mac and Windows. | Low | ❌ |
| **4.08** | Signed number node. | Have a signed node value in the test-signed-node.csv file (in this file the node 2 is signed everywhere). | Send an ERROR message. | **Mac**: Send error 400 INVALID PARAMS (because we cannot use signed number in the URL). **Windows**: The pre-prossessing stop with the connectivity check. | Tested on Mac and Windows. | High | ❌ |
| **4.09** | Signed one node value. | Have only one signed node value in the test-signed-value-node.csv file (in this file the node 2 is signed only for one edge). | Send an ERROR message. | **Mac**: Message “connectivity check resulted in an error. Please ensure that the graph is fully connected, meaning it is possible to navigate between any two landmarks.” on the pre-processing step. **Windows**: The pre-prossessing doesn’t work. | Tested on Mac and Windows. | Medium | ❌ |

## E. Output

| Test Case ID | Test Case Name | Test Case Description | Expected Result | Actual Result | Tested | Priority | Pass/Fail |
|--------------|----------------|-----------------------|-----------------|---------------|--------|----------|-----------|
| **5.1** | Output XML. | Verify if the program send a XML file as an output. | Send a XML file. | It return a XML file. | Tested on Mac and Windows. | High | ✅ |
| **5.2** | Output JSON. | Verify if the program send a JSON file as an output. | Send a JSON file. | It return a JSON file. | Tested on Mac and Windows. | High | ✅ |
| **5.3** | Output another things. | Verify if the program send a another file as an output. | Send error 400 INVALID ACCEPT HEADER. | Send error 400 INVALID ACCEPT HEADER. | Tested on Mac and Windows. | Medium | ✅ |
| **5.4** | Output comparaison. | Compare the result of XML and JSON. | Their are the same result. | The program return the same things. | Tested on Mac and Windows. | High | ✅ |

# III. Final version

## A. API tests

| Test Case ID | Test Case Name | Test Case Description | Expected Result | Actual Result | Tested | Priority | Pass/Fail |
|--------------|----------------|-----------------------|-----------------|---------------|--------|----------|-----------|
| **1.01** | Blank space during the Pre-prossessing. | When we add a file or a value during the set-up (y/n or a int) put a blank space before. | Nothing. | Nothing happend and we can put the file or an answer after. | Tested on Mac and Windows. | Low | ✅ |
| **1.02**| Blank space in the Localhost. | In the Localhost link, put a blank and nothing else. | Error message 400 INVALID PARAMS. | Error message 400 INVALID PARAMS. | Tested on Mac and Windows. | Low | ✅ |
| **1.03** | Put a Number Over 30,000,000 in the Request. | In the Request, put a number over 30,000,000 for one of point A or B. | Send error 404 NO PATH FOUND. | Send error 404 NO PATH FOUND. | Tested on Mac and Windows. | Medium | ✅ |
| **1.04** | Put a character in the Localhost. | In the Localhost link, put a character instead of a Int. | Send error 400 INVALID PARAMS. | Send error 400 INVALID PARAMS. | Tested on Mac and Windows. | Low | ✅ |
| **1.05** | Put a String in the Localhost. | In the Localhost link, put a string (multiple of character) instead of a Int. | Send error 400 INVALID PARAMS. | Send error 400 INVALID PARAMS. | Tested on Mac and Windows. | Low | ✅ |
| **1.06** | Put a Float in the Localhost. | In the Localhost link, put a float instead of a Int. | Send error 400 INVALID PARAMS. | Send an error message "CODE 400 : invalid params". | Tested on Mac and Windows. | Medium | ✅ |
| **1.07** | Put a Int & a String in the Localhost. | In the Localhost link, put a Int and a String for a landmark (ex : 12ABC). | Send error 400 INVALID PARAMS. | Send an error message "CODE 400 : invalid params". | Tested on Mac and Windows. | Medium | ✅ |
| **1.08** | Put a String & a Int in the Localhost. | In the Localhost link, put a String and a Int for a landmark (ex : ABC12). | Send error 400 INVALID PARAMS. | Send error 400 INVALID PARAMS. | Tested on Mac and Windows. | Medium | ✅ |
| **1.09** | Put a signed number in the Localhost. | In the Localhost link, put a signed number for a landmark (ex : -5 or “-5”). | Send error 400 INVALID PARAMS. | Send error 400 INVALID PARAMS. | Tested on Mac and Windows. | Medium | ✅ |
| **1.10** | Put a 0 in the Localhost. | In the Localhost link, put a 0 for a landmark. | Send error 404 NO PATH FOUND. | Send error 404 NO PATH FOUND. | Tested on Mac and Windows. | Medium | ✅ |
| **1.11** | Input a Int in the Localhost. | Verify if the program work correctly. | The program work correctly. | The program work correctly. | Tested on Mac and Windows. | High | ✅ |
| **1.12** | Put more landmarks than nodes. | When we set up the program on the terminal, put 45 landmarks because we have 39 nodes. | Skipping X landmarks, maximum value has been reach + message. | He select all node and stop at 39 landmarks. | Tested on Mac and Windows. | High | ✅ |
| **1.13** | Choose unexistent search engine. | Put other number than 1 or 2 when you choose unidirectional or bidirectional. | Error message and ask to reselect. | Send error message "[ERROR] invalid input. put 1 or 2". | Tested on Mac and Windows. | Medium | ✅ |
| **1.14** | Enter letter search engine. | Put letter instead of 1 or 2 when you choose unidirectional or bidirectional. | Error message and ask to reselect | Send an error message "[Error] invalid input. please enter a valid integer.". | Medium | ✅ |

## B. Integrity & Connectivity tests

| Test Case ID | Test Case Name | Test Case Description | Expected Result | Actual Result | Tested | Priority | Pass/Fail |
|--------------|----------------|-----------------------|-----------------|---------------|--------|----------|-----------|
| **2.1** | Wrong format. | Try to do the integrity test with a wrong format like JPEG. | Display an error message and ask a new file. | Send this message "ERROR file is not a .csv file" | Tested on Mac and windows. | Low | ✅ |
| **2.2** | Node empty. | Delete all node costs in the file. | Error message + "The graph is empty.". | Send an error message "no valid edges loaded. graph is empty.". | Tested on Mac and Windows. | Low | ✅ |

## C. Process tests

| Test Case ID | Test Case Name | Test Case Description | Expected Result | Actual Result | Tested | Priority | Pass/Fail |
|--------------|----------------|-----------------------|-----------------|---------------|--------|----------|-----------|
| **3.1** | 10% Heuristics. | The returned path should not exceed the shortest path duration by more than 10%. For that we used the file test.csv. | Give the shortest path or one in the limit of 10%. | Give the shortest path or one in the limit of 10%. | Tested on Mac and Windows. | High | ✅ |

## D. CSV files tests

| Test Case ID | Test Case Name | Test Case Description | Expected Result | Actual Result | Tested | Priority | Pass/Fail |
|--------------|----------------|-----------------------|-----------------|---------------|--------|----------|-----------|
| **4.01** | Compilation and running. | Verify the program laucnh well. | The program run and initialize. | The program run and initialize. | Tested on Mac and Windows. | High | ✅ |
| **4.02** | Blanck space in the file .csv. | Space between the comma and a value in the test-blank-space.csv file. | Return the file with the path found. | Return the file with the path found. | Tested on Mac and Windows. | Low | ✅ |
| **4.03** | Input string distance in the file .csv. | Have a string distance value in the test-string-distance.csv file. | The line is skipping and return the program run. | The line is skipping and return the program run. | Tested on Mac and Windows. | Low | ✅ |
| **4.04** | Input float distance. | Have a float distance value in the test-float-distance.csv file. | The line is skipping and return the program run. | The line is skipping and return the program run. | Tested on Mac and Windows. | Medium | ✅ |
| **4.05** | Signed number distance. | Have a signed distance value in the test-signed-number-distance.csv file. | The line is skipping and return the program run. | The line is skipping and return the program run. | Tested on Mac and Windows. | Medium | ✅ |
| **4.06** | Input string node. | Have a string node value in the test-string-node.csv file. | The line is skipping and return the program run. | The line is skipping and return the program run. | Tested on Mac and Windows. | Medium | ✅ |
| **4.07** | Input float node. | Have a float node value in the test-float-node.csv file. | The line is skipping and return the program run. | The line is skipping and return the program run. | Tested on Mac and Windows. | Low | ✅ |
| **4.08** | Signed number node. | Have a signed node value in the test-signed-node.csv file (in this file the node 2 is signed everywhere). | The line is skipping and return the program run. | The line is skipping and return the program run. | Tested on Mac and Windows. | High | ✅ |
| **4.09** | Signed one node value. | Have only one signed node value in the test-signed-value-node.csv file (in this file the node 2 is signed only for one edge). | The line is skipping and return the program run. | The line is skipping and return the program run. | Tested on Mac and Windows. | Medium | ✅ |
| **4.10** | Link alone. | Only one link of the node is correct and create a link alone (link 1 - 2). | The connectivity phase is failed. | The connectivity phase is failed. | Tested on Mac and Windows. | Medium | ✅ |

## E. Output

| Test Case ID | Test Case Name | Test Case Description | Expected Result | Actual Result | Tested | Priority | Pass/Fail |
|--------------|----------------|-----------------------|-----------------|---------------|--------|----------|-----------|
| **5.1** | Output XML. | Verify if the program send a XML file as an output. | Send a XML file. | It return a XML file. | Tested on Mac and Windows. | High | ✅ |
| **5.2** | Output JSON. | Verify if the program send a JSON file as an output. | Send a JSON file. | It return a JSON file. | Tested on Mac and Windows. | High | ✅ |
| **5.3** | Output another things. | Verify if the program send a another file as an output. | Send error 400 INVALID ACCEPT HEADER. | Send error 400 INVALID ACCEPT HEADER. | Tested on Mac and Windows. | Medium | ✅ |
| **5.4** | Output comparaison. | Compare the result of XML and JSON. | Their are the same result. | The program return the same things. | Tested on Mac and Windows. | High | ✅ |