# Projet Maze

## Description

This project implements a maze generator and solver in C. It has two main components:

1. **The main program** `maze` : generates and displays a maze based on specified dimensions.
2. **Unit tests** : automates the verification of the program's functionality.   

---

## Table des matières

1. [Compiling the Project](#compiling-the-project)
2. [Running the Main Program](#running-the-main-program)
   - [Run `maze`](#run-maze)
3. [Running Unit Tests](#running-unit-tests)
   - [Run All Unit Tests](#run-all-unit-tests)
   - [Run a Specific Test](#run-a-specific-test)
   - [Run Specific Features](#testing-specific-features)
4. [Cleaning Compiled Files](#cleaning-compiled-files)
5. [Project Structure](#project-structure)
6. [Note](#note)

---

## Compiling the Project

### Step 1: Compile the Program

To compile the project, open a terminal and navigate to the directory containing the source files. Then, run the following command:

```bash
make
```

This will generate two executables:

- `maze` : the main program.
- `maze_test` : the executable for running unit tests.

---

## Running the Main Program

Once the project is compiled, you can run the main program `maze`. This program generates a maze based on specified dimensions (height and width).

### Run `maze`

To run the main program and display a maze, use the following command:

```bash
./maze <hauteur> <largeur>
```

For example, to generate a 5x5 maze, you can run:

```bash
./maze 5 5
```

This will create a maze with 5 rows and 5 columns and display it in the standard output.


---

## Running Unit Tests

Unit tests are automated in the file `maze_test.c` and can be run via the `tests.sh` script. You can run all tests or a specific test.


### Run All Unit Tests

To run all unit tests automatically, use the following command:

```bash
make test
```

This will compile and execute the test program `maze_test`, which will run all tests defined in the `tests.sh` script and display results for each test.

### Run a Specific Test

If you want to run a specific test, you can use the executable ./tests.sh with the test number as an argument. To run it, use the following command:

```bash
./tests.sh value
```

This will run only the specified test and output its result in the standard


### Testing Specific Features

To test specific maze features, run the following command with the test name:

```bash
./maze_test <nom_du_test>
```

Available tests :

- tree : Verifies the integrity of the tree structure
- queue : Tests queue operations.
- value : Checks that matrix values stay within specified bounds.
- border : Validates outer wall configurations of the maze.

---

## Cleaning Compiled Files

To remove compiled files (such as `maze`, `maze_test`, or `.o` files), use:

```bash
make clean
```

This will delete all files generated during compilation.

---

## Project Structure

The project’s file structure is as follows:

```
├── maze.c                # Main program code
├── maze_test.c           # Unit tests code
├── tests.sh              # Script to run automated tests
├── Makefile              # Compilation file
└── README.md             # Project documentation
```

---

## Note

If you encounter permission errors when running the `tests.sh` script, ensure the file is executable with:

```bash
chmod +x tests.sh
```

This will allow the test script to run correctly.
