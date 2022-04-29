//
// Created by bogum on 10.04.2022.
//

#ifndef PROJECT2_LOCATION_H
#define PROJECT2_LOCATION_H


struct Location {

private:
    int x;
    int y;
    int z;

public:
    Location(int x, int y, int z);
    int getX();
    int getY();
    int getZ();
    void setX(int x);
    void setY(int y);
    void setZ(int z);
    void setLocation(int x, int y, int z);
    int getLocalId();

};


#endif //PROJECT2_LOCATION_H
