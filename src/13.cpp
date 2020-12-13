#include <iostream>
#include <fstream>
#include <cassert>
#include <iterator>
#include <vector>
#include <algorithm>
#include <numeric>

int main(int argc, char *argv[])
{
    assert(("expect input", argc == 2));

    std::ifstream ifs{argv[1]};

    int arrival;
    ifs >> arrival;

    std::vector<int> buses_id;
    std::vector<int> buses_id_offset;

    std::string value;
    int max_value = 0;
    int offset = 0;
    while(std::getline(ifs, value, ','))
    {
        if(value != "x") {
            buses_id.push_back(std::stoi(value));
            max_value = std::max(std::stoi(value), max_value);

            buses_id_offset.push_back(offset);
        }
        ++offset;
    }

    int earliest_bus = max_value;
    for(auto bus_id : buses_id) {
        earliest_bus = std::min(arrival - (arrival % bus_id) + bus_id, earliest_bus);
    }

    std::cout << "Part A\n";
    std::cout << earliest_bus * (earliest_bus - arrival % earliest_bus) << '\n';


    std::size_t departure = 0;

    while(true)
    {
        bool condition = true;
        for(int i = 1; i < buses_id.size(); ++i) {
            condition &= (departure + buses_id_offset.at(i)) % buses_id.at(i) == 0;
            if(!condition)
                break;
        }

        if(condition)
            break;

        departure += buses_id.at(0);
    }

    std::cout << "Part B\n";
    std::cout << departure << '\n';

    return 0;
}
