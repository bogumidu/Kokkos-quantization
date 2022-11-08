//
// Created by bogum on 08.11.2022.
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

    Location();

    [[nodiscard]] int getX() const;
    [[nodiscard]] int getY() const;
    [[nodiscard]] int getZ() const;
    void setX(int set_x);
    void setY(int set_y);
    void setZ(int set_z);
    void setLocation(int set_x, int set_y, int set_z);
    [[nodiscard]] int getLocalId() const;

    bool operator==(const Location &rhs) const;

    bool operator!=(const Location &rhs) const;

};


#endif //PROJECT2_LOCATION_H
