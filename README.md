# Projects

To run the program:
./allocate -f <filename> -m (infinite | first-fit | paged | virtual) -q (1 | 2 | 3) 

    All options are required for program to run:
    -f： <filename> specifies a valid relative or absolute path to the input file describing the processes
    -m:  memory-strategy is one of (infinite | first-fit | paged | virtual)
    -q:  quantum where quantum is one of (1 | 2 | 3)
