# Large-sort
Sort1GB file with 100 MB of available RAM

C++ implementation of a sorting algorithm which sorts 1GB file of double values(generated with LargeSort::generate_1GB_random_doubles_file(); method) with available 100 MB of RAM. The program runs on my intel i9 9900k approximately 550 seconds. 
Typical use is:

largeSort.generate_1GB_random_doubles_file();
largeSort.sort();
largeSort.check_output_file();
  
