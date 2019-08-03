# Large-sort
Sort 1GB file of double values with 100 MB of available RAM.

C++ implementation of a sorting algorithm which sorts 1GB file of double values(generated with LargeSort::generate_1GB_random_doubles_file(); method) with 100 MB of available RAM.                                                                                                          The program runs on my intel i9 9900k approximately 637 seconds.                              
Typical use is:

LargeSort largeSort("[...]\\\random_doubles.bin", "[...]\\\output_sorted_doubles.bin");                                             
largeSort.generate_1GB_random_doubles_file();                                                                                
largeSort.sort();                                                                                                            
largeSort.check_output_file();                                                                                                
