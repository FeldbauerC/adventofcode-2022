#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <math.h>
#include <set>

/*
inspired by: https://www.reddit.com/r/adventofcode/comments/zmfwg1/2022_day_15_part_2_seekin_for_the_beacon/
*/

typedef std::pair<int, int> position;

constexpr int tuningFactor = 4000000;

struct sensor
{
    position self;
    position beacon;
    int MHdistance;
};

void manhattenDistance(sensor &sensor)
{
    sensor.MHdistance = fabs(sensor.self.first - sensor.beacon.first) + fabs(sensor.self.second - sensor.beacon.second);
}

int manhattenDistance(position pos1, position pos2)
{
    return fabs(pos1.first - pos2.first) + fabs(pos1.second - pos2.second);
}

void insertOuter(std::set<position> &borders, sensor currentSensor)
{
    for (int i = 0; i <= (currentSensor.MHdistance + 1); i++)
    {
        borders.insert({(currentSensor.self.first + i), (currentSensor.self.second + ((currentSensor.MHdistance + 1) - i))});
        borders.insert({(currentSensor.self.first - i), (currentSensor.self.second - ((currentSensor.MHdistance + 1) - i))});
        borders.insert({(currentSensor.self.first + i), (currentSensor.self.second - ((currentSensor.MHdistance + 1) - i))});
        borders.insert({(currentSensor.self.first - i), (currentSensor.self.second + ((currentSensor.MHdistance + 1) - i))});
    }
}

int main(int argc, char **argv)
{
    std::cout << "called: " << argv[0] << " with " << (argc - 1) << " arguments" << std::endl;
    if (argc != 3)
        return -1;

    int maxPos;
    sscanf(argv[2], "%d", &maxPos);

    std::vector<sensor> sensors;
    std::set<position> signalborders;
    std::ifstream input;
    input.open(argv[1]);

    while (!input.eof())
    {
        char line_buf[80];
        input.getline(&line_buf[0], sizeof(line_buf));

        if (line_buf[0] == '\0')
        {
            std::cerr << "parsed empty line" << std::endl; // ok if last line
            continue;
        }
        int sensorX, sensorY, beaconX, beaconY;
        sscanf(line_buf, "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d", &sensorX, &sensorY, &beaconX, &beaconY);
        sensor currentSensor;
        currentSensor.self = std::make_pair(sensorX, sensorY);
        currentSensor.beacon = std::make_pair(beaconX, beaconY);
        manhattenDistance(currentSensor);
        sensors.push_back(currentSensor);
    }
    input.close();

    std::cout << "parsed input" << std::endl;

    /* process sensor borders and removing when overlap or out of range */
    for (auto &&sensor : sensors)
    {
        for (auto it = signalborders.begin(); it != signalborders.end();)
        {
            if (manhattenDistance(sensor.self, *it) <= sensor.MHdistance ||
                it->first >= maxPos ||
                it->second >= maxPos ||
                it->first < 0 ||
                it->second < 0)
            {
                auto currentIT = it;
                it++;
                signalborders.erase(currentIT);
            }
            else
                it++;
        }

        insertOuter(signalborders, sensor);
    }

    std::cout << "processed all sensor borders" << std::endl;
    /* perform removing out of range borders */
    for (auto it = signalborders.begin(); it != signalborders.end();)
    {
        if (it->first >= maxPos || it->second >= maxPos || it->first < 0 || it->second < 0)
        {
            auto currentIT = it;
            it++;
            signalborders.erase(currentIT);
        }
        else
            it++;
    }

    std::cout << "removed all out of range borders" << std::endl;

    /* perform removing covered borders */
    for (auto &&sensor : sensors)
    {
        for (auto it = signalborders.begin(); it != signalborders.end();)
        {
            if (manhattenDistance(sensor.self, *it) <= sensor.MHdistance)
            {
                auto currentIT = it;
                it++;
                signalborders.erase(currentIT);
            }
            else
                it++;
        }
    }

    std::cout << "performed removing covered borders " << std::endl;

    if (signalborders.size() == 1)
    {
        std::cout << "the result coordinate is at: " << signalborders.begin()->first << ":" << signalborders.begin()->second << std::endl;
        uint64_t tuningFrequency = signalborders.begin()->first * tuningFactor + signalborders.begin()->second;

        std::cout << "the result is: " << tuningFrequency << std::endl;
    }
    else
    {
        std::cout << "failed - more then one signal border is left" << std::endl;
    }

    return 0;
}