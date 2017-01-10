To compile the code and generate executable, 
please run the run.sh using the command ./run.sh

If it requests execution rights,
enter this in the commandline within the folder.

chmod +x run.sh
./run.sh

To run the code,

cd build

./kdtree 
This demonstrates few tasks from building, querying, serializing, deserializing, checking and so on.
Details provided in the documentation.
(or)

./kdtree -b /data/sample_data.csv
This builds the tree from the specified .csv file.
(or)

./kdtree -d /data/tree.kd
This deserializes a tree which was already built

Find the generated output files at src/data/output.txt
This file contains the data as instructed in the assignment question paper 
in the format of 

index_of_nearest_neighbor_in_input_file, euclidean_distance_between_the_nn_and_query_point

For detailed documentation, please find the file src/html/annotated.html

