#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <time.h>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <assert.h>
#include <stdio.h>

using namespace std;


class LargeSort {
	std::string generated_path;
	std::string sorted_path;
	std::vector<std::string> temp_file_names;

	const std::string tmp_files_path = "C:\\Users\\david\\Desktop\\";

	const std::size_t file_size = 1024 * 1024 *1024 / sizeof(double); // 1GB
	const std::size_t chunk_size = file_size  / 11;

	struct QueueEntry {
		int i; //temp file index
		double v; //value
	};

	struct Comparison {
		bool operator()(const QueueEntry entry1, const QueueEntry entry2) const
		{
			return entry1.v > entry2.v;
		}
	};

	bool file_exists(const std::string& name) const{
		ifstream f(name);
		return f.good();
	}

	template<typename T> void print_queue(T& q) const {
		while (!q.empty()) {
			std::cout << q.top().v << " " << q.top().i << endl;
			q.pop();
		}
		std::cout << '\n';
	}

	std::pair<std::vector<double>, int> read_chunk(ifstream& input_stream) const{
		std::vector<double> chunk(chunk_size);
		double num = 0.;
		int i = 0;

		while (i != chunk.size() && input_stream.read((char*)& num, sizeof(double))) {
			chunk[i++] = num;
		}

		return std::pair<std::vector<double>, int>(chunk, i);
	}
	
	void write_sorted_buffer_to_temp_file(const std::pair<std::vector<double>, int>& buff, int i) {
		std::string name = tmp_files_path + "temp_file" +  std::to_string(i);
		std::ofstream temp_file(name, std::ios_base::binary);

		for (int j = 0; j < buff.second; j++) {
			temp_file.write((char*)& buff.first[j], sizeof(double));
		}
		temp_file.close();
		temp_file_names.push_back(name);
	}

	void merge_sorted_temp_files() const {
		double num;
		vector<ifstream> temp_files_streams;
		std::priority_queue<QueueEntry, std::vector<QueueEntry>, Comparison> q;

		std::ofstream sorted_out(sorted_path, std::ios_base::binary);

		for (const std::string& tmp_file_name : temp_file_names) {
			temp_files_streams.push_back(std::ifstream(tmp_file_name, std::ios_base::binary));
		}

		for (int i = 0; i < temp_files_streams.size(); ++i) {
			temp_files_streams[i].read((char*)& num, sizeof(double));
			q.push({ i, num });
		}
		std::cout << "Priority queue successfully created!" << endl;
		std::cout << "Merging sorted temporary files ..." << endl;


		while (!q.empty())
		{
			// extract minimum
			QueueEntry min = q.top();
			q.pop();
		
			sorted_out.write((char*)& min.v, sizeof(double));

			if (temp_files_streams[min.i].read((char*)& num, sizeof(double))) {
				q.push({ min.i, num });
			}
		}

		sorted_out.close();

		for (ifstream& tmp_file_stream : temp_files_streams) {
			tmp_file_stream.close();
		}
		std::cout << "Merge successfully finished" << endl;
	}

public:

	LargeSort(const std::string& unsorted_file_path, const std::string& sorted_file_path) : generated_path(unsorted_file_path), sorted_path(sorted_file_path) {
	}

	~LargeSort() {
		std::cout << "Removing temporary files ..." << std::endl;

		for (const std::string& tmp_file_name : temp_file_names) {
			if (remove(tmp_file_name.c_str()) != 0) {
				std::cerr << "Error deleting file " + tmp_file_name << std::endl;
			}
			std::cout << tmp_file_name + " temporary file removed successfully!" << std::endl;
		}
	}

	void check_output_file() const {
		double num1, num2;

		std::ifstream sorted_out(sorted_path, std::ios_base::binary);

		std::cout << "Checking sorted files ..." << sorted_path << std::endl;

		while (sorted_out.read((char*)& num1, sizeof(double)) && sorted_out.read((char*)& num2, sizeof(double))) {		
			assert(num1 <= num2 && "File is not sorted!");
		}

		sorted_out.close();
		std::cout << "File is sorted" << std::endl;
	}

	void generate_1GB_random_doubles_file() const {
		std::ofstream out(generated_path, std::ios_base::binary);
		srand((unsigned int)time(NULL));

		if (out.good())
		{
			std::cout << "Generating random doubles! " << std::endl;
			
			double scale_factor = 10.0; // generate doubles from interval [0., 10.]
			for (int i = 0; i < file_size; i++) {
				double random_double = ((double)rand() / (double)(RAND_MAX)) * scale_factor;
				out.write((char*)& random_double, sizeof(double));
			}
			out.close();

			std::cout << "Random doubles successfully generated! " << std::endl;
		}
		else {
			std::cerr << "Read/Write error! Quitting! ";
		}
	}

	void sort() {
		
		if (!file_exists(generated_path)) {
			std::cerr << "Random file doesn't exist! Quitting!" << std::endl;
			return;
		}
		
		int i = 0;
		vector<double> buffer;

		ifstream in = std::ifstream(generated_path, std::ios_base::binary);
		
		while (true) {
			std::pair<std::vector<double>, int> buffer = read_chunk(in);
			if (buffer.second == 0) {
				break;
			}
			std::sort(buffer.first.begin(), buffer.first.begin() + buffer.second);

			write_sorted_buffer_to_temp_file(buffer, i++);
			cout << std::to_string(i-1) <<"-th temporary file successfully sorted!" << std::endl;

		}

		in.close();
		merge_sorted_temp_files();

	}

};


int main() {
	int t_begin = time(NULL);
	
	LargeSort largeSort("C:\\Users\\david\\Desktop\\random_doubles.bin", "C:\\Users\\david\\Desktop\\output_sorted_doubles.bin");
	largeSort.generate_1GB_random_doubles_file();
	largeSort.sort();
	largeSort.check_output_file();

	int t_end = time(NULL);

	std::cout << "Elapsed time: " << t_end - t_begin << " seconds!" << std::endl;
	std::cout << "Successfully Finished!" << std::endl;
}