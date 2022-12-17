#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <math.h>
#include <set>

struct position
{
    int x;
    int y;
};

struct sensor
{
    position self;
    position beacon;
    int MHdistance;
};

int manhattenDistance(sensor &sensor)
{
    sensor.MHdistance = fabs(sensor.self.x - sensor.beacon.x) + fabs(sensor.self.y - sensor.beacon.y);
    return sensor.MHdistance;
}

int main(int argc, char **argv)
{
    std::cout << "called: " << argv[0] << " with " << (argc - 1) << " arguments" << std::endl;
    if (argc != 3)
        return -1;

    int lineNr;
    sscanf(argv[2], "%d", &lineNr);

    std::vector<sensor> sensors;
    std::set<int> coverdPoints;
    std::set<int> coverdByThing;
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
        currentSensor.self = {sensorX, sensorY};
        currentSensor.beacon = {beaconX, beaconY};
        manhattenDistance(currentSensor);
        sensors.push_back(currentSensor);
    }
    input.close();

    for (auto &&sensor : sensors)
    {
        int distance = fabs(sensor.self.y - lineNr);
        if (distance <= sensor.MHdistance)
        {
            int lineElems = fabs(sensor.MHdistance - distance);

            int from = (sensor.self.x - (lineElems));
            int to = (sensor.self.x + (lineElems));

            std::cout << "found sensor that is near enough - it should cover " << lineElems * 2 - 1 << " positions of row from " << from << " to " << to << std::endl;
            for (int idx = from; idx <= to; idx++)
                coverdPoints.insert(idx);

            if (sensor.self.y == lineNr)
                coverdByThing.insert(sensor.self.x);

            if (sensor.beacon.y == lineNr)
                coverdByThing.insert(sensor.beacon.x);
        }
    }

    std::cout << "the result is: " << coverdPoints.size() - coverdByThing.size() << std::endl;
    return 0;
}