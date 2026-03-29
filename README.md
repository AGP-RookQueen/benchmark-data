### Benchmark datasets and experimental results from:  

**“Algorithms for Art Gallery Problems with Rook and Queen Vision”**  
*Duarte Nóbrega, MSc Thesis, University of Porto, 2025*

## Scripts

The core implementation is divided into:

- Scripts for running the algorithms.
- Scripts for benchmarking the Integer Linear Programming (ILP) model creation times.
- Miscellaneous scripts.

In particular:

* `baseline.py`: Python implementation of the baseline method.
* `our.py`: Python implementation of our proposed method.
* `baseline_build.py`: Benchmarks the time required to create the ILP model using the baseline method.
* `our_build.py`: Benchmarks the time required to create the ILP model using our method.

## Experimental Results & Datasets

The experimental data is organised by polyomino class. The repository includes folders for **Convex**, **(Descending) Parallelogram**, and **Generic (Polyominoes without Holes - Permutominoes)**. 

Inside each polyomino class directory, the default structure is organised as follows:

    📁 [Polyomino Class Name]/
    ├── 📁 images/               # Visual renders of each polyomino
    ├── 📁 input/                # Bitmap representations of each polyomino
    ├── 📁 create_inputs/        # Contains scripts to create the input files
    │   └── 📁 input_raw/        # Inputs defined by their vertices in: Clockwise (CW) order for Parallelograms and Generic, Counter-Clockwise (CCW) for Convex
    ├── 📄 results_baseline.txt  # Execution times for the baseline method
    ├── 📄 results_our.txt       # Execution times for our method
    ├── 📄 results_build_baseline_build_final.txt # ILP model build times (baseline)
    └── 📄 results_build_our_build_final.txt      # ILP model build times (our method)

**Special Case for Convex Polyominoes:**
For the **Convex** class specifically, there is an additional folder named `large_inputs` containing large bitmap input files. The execution results for these specific inputs follow the exact same formatting rules as the standard tests, but are located in the `results_baseline_large_input.txt` and `results_our_large_input.txt` files (or their respective folders).

### Understanding the Result Files

* **Execution Times** (`results_baseline.txt`, `results_our.txt`, and the large input equivalents): 
  Each line in these files is organised as follows:
  `Test [NUMBER OF VERTICES]: [OPTIMAL NUMBER OF ROOKS] [EXECUTION TIME] [EXIT CODE]`
  * **Exit Codes:** The exit codes correspond to the Gurobi optimizer status codes (e.g., `2` = Solved/Optimal, `9` = Timeout). For a complete list of numerical codes, please refer to the [Gurobi Status Codes documentation](https://docs.gurobi.com/projects/optimizer/en/current/reference/numericcodes/statuscodes.html) *(last visited on 28/10/2025)*.
  * **Note on Execution Time:** The maximum execution time for all tests is capped at **3600 seconds (1 hour)**. Runs exceeding this limit will reflect a timeout in their exit code.

* **Build Times** (`results_build_baseline_build_final.txt` & `results_build_our_build_final.txt`): 
  Each line in these files is organised as follows:
  `Test [NUMBER OF VERTICES]: [TIME TAKEN TO BUILD MODEL]` (representing the time taken to build the model for the respective method).