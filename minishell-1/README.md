# Minishell Project

## Overview
Minishell is a simple shell implementation that supports basic shell functionalities, including input and output redirection. The project is designed to help users understand shell operations and improve their programming skills in C.

## Project Structure
```
minishell
├── src
│   ├── redirs
│   │   └── redirs.c
│   └── ...
├── include
│   └── minishell.h
├── check_norm.sh
├── git_commit.sh
└── README.md
```

## Files Description

- **src/redirs/redirs.c**: Contains functions for handling input and output redirection in a shell environment.
  
- **include/minishell.h**: Header file that includes declarations for functions and data structures used in the minishell project.

- **check_norm.sh**: A shell script that executes Norminette on each subdirectory within the `src` folder. It ensures that the code adheres to the coding standards.

- **git_commit.sh**: A shell script that takes an argument `$1` and executes `git add $1`, `git commit -m "default"`, and `git push`. This script simplifies the process of committing changes to the repository.

## Usage

### Running Norminette
To check the coding standards for the project, run the following command:
```bash
./check_norm.sh
```

### Committing Changes
To commit changes to a specific file or directory, use the following command:
```bash
./git_commit.sh <path_to_file_or_directory>
```

## License
This project is licensed under the MIT License. See the LICENSE file for more details.