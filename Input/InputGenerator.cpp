#include <fstream>
#include <iostream>
#include <random>
#include <string>

int main() {
    std::ofstream file("file.txt");

    if (!file.is_open()) {
        std::cerr << "Failed to create inputfile.txt\n";
        return 1;
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    // Interactive processes
    std::uniform_int_distribution<> io_bound_cpu(1, 3);
    std::uniform_int_distribution<> io_bound_io(20, 35);

    // CPU-bound processes
    std::uniform_int_distribution<> cpu_bound_cpu(7, 10);
    std::uniform_int_distribution<> cpu_bound_io(20, 35);

    //std::uniform_int_distribution<> io_bound_count(5, 10);
    //std::uniform_int_distribution<> cpu_bound_count(2, 4);

    std::uniform_int_distribution<> process_type(0,1);
    std::uniform_int_distribution<> bursts_number(10,30);

    const int total_processes = 300;

    for (int pid = 1; pid <= total_processes; pid++) {

        int process = process_type(gen);
        int cpu_count = bursts_number(gen);
        int total_bursts = 2 * cpu_count - 1; //between 19 - 59

        file << pid << ' ' ; //<< process<< " "<< total_bursts;

        for (int i = 0; i < cpu_count; i++) {

            int cpu = process == 1 ? io_bound_cpu(gen): cpu_bound_cpu(gen);

            file << ' ' << cpu;

            // No I/O burst after the final CPU burst.
            if (i < cpu_count - 1) {
                int io = process == 1 ? io_bound_io(gen) : cpu_bound_io(gen);
                file << ' ' << io;
            }
        }

        file << '\n';
    }

    file.close();

    std::cout << "Generated inputfile.txt successfully.\n";
    return 0;
}