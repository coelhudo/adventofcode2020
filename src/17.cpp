#include <iostream>
#include <fstream>
#include <cassert>
#include <map>
#include <set>
#include <algorithm>
#include <tuple>
#include <vector>

typedef std::tuple<int,int,int> Coordinate;

std::ostream & operator<<(std::ostream &os, Coordinate const& coord){
    os << std::get<0>(coord) << ' ' << std::get<1>(coord) << ' ' << std::get<2>(coord);
    return os;
}

typedef std::set<Coordinate> Cubes3DSpace;


int main(int argc, char *argv[])
{
    assert(("expect input", argc == 2));

    std::ifstream ifs{argv[1]};

    Cubes3DSpace cubes_space;

    std::string line;
    int current_x = 0;
    while(ifs >> line) {
        for(int current_y = 0; current_y < line.size(); ++current_y) {
            if(line[current_y] == '#') {
                auto cube = Coordinate(0, current_x, current_y);
                cubes_space.emplace(cube);
            }
        }
        ++current_x;
    }

    std::set<std::tuple<int, int, int>> neighbours_delta;
    std::set<std::tuple<int, int, int>> initial_deltas;
    for(auto x :{0,1})
        for(auto y :{0,1})
            for(auto z: {0,1})
                initial_deltas.emplace(x,y,z);

    for(auto [x,y,z]: initial_deltas) {
        neighbours_delta.emplace(x,y,z);
        neighbours_delta.emplace(x*-1, y, z);
        neighbours_delta.emplace(x, y*-1, z);
        neighbours_delta.emplace(x, y, z*-1);
        neighbours_delta.emplace(x*-1, y*-1, z);
        neighbours_delta.emplace(x, y*-1, z*-1);
        neighbours_delta.emplace(x*-1, y, z*-1);
        neighbours_delta.emplace(x*-1, y*-1, z*-1);
    }

    int cycles = 6;
    while(cycles--)
    {
        Cubes3DSpace inactive_cubes_with_neighbours;
        Cubes3DSpace new_cubes_space;

        //Apply first rule on active cubes
        for(auto cube: cubes_space)
        {
            auto[z,x,y] = cube;
            int count{};
            for(auto [delta_z, delta_x, delta_y] : neighbours_delta) {
                auto neighbour = Coordinate(z+delta_z, x+delta_x, y+delta_y);
                if(neighbour != cube && cubes_space.find(neighbour) != std::end(cubes_space)) {
                    ++count;
                 } else
                     // if was tested as neighbour of one of the active cubes it means
                     // that this is an inactive cube that needs to be checked against
                     // its neighbours. We save the inactive cubes to apply second rule later
                    inactive_cubes_with_neighbours.emplace(neighbour);
            }
            if(count == 2 || count == 3) {
                new_cubes_space.emplace(cube);
            }
        }

        //Apply second rule on inactive cubes
        for(auto cube: inactive_cubes_with_neighbours)
        {
            auto[z,x,y] = cube;
            int count{};
            for(auto [delta_z, delta_x, delta_y] : neighbours_delta) {
                auto neighbour = Coordinate(z+delta_z, x+delta_x, y+delta_y);
                if(cube != neighbour && cubes_space.find(neighbour) != std::end(cubes_space))
                    ++count;
            }
            if(count == 3) {
                new_cubes_space.emplace(cube);
            }
        }

        cubes_space = new_cubes_space;
    }

    std::cout << "Part A\n";
    std::cout << cubes_space.size() << '\n';

    return 0;
}