//
// Created by bogum on 05.06.2022.
//

#ifndef PROJECT2_TEMPFACE_H
#define PROJECT2_TEMPFACE_H


#include <vector>
#include "Face.h"
#include "../utils/ObjectStore.h"

class TempFace {
private:
    std::vector<double> v1;

    std::vector<double> t1;

    std::vector<double> v2;

    std::vector<double> t2;

    std::vector<double> v3;

    std::vector<double> t3;
public:
    TempFace* fromFace(Face face, ObjectStore* objectStore);

    [[nodiscard]] const std::vector<double> &getV1() const;

    void setV1(const std::vector<double> &set_v1);

    [[nodiscard]] const std::vector<double> &getT1() const;

    void setT1(const std::vector<double> &set_t1);

    [[nodiscard]] const std::vector<double> &getV2() const;

    void setV2(const std::vector<double> &set_v2);

    [[nodiscard]] const std::vector<double> &getT2() const;

    void setT2(const std::vector<double> &set_t2);

    [[nodiscard]] const std::vector<double> &getV3() const;

    void setV3(const std::vector<double> &set_v3);

    [[nodiscard]] const std::vector<double> &getT3() const;

    void setT3(const std::vector<double> &set_t3);

};


#endif //PROJECT2_TEMPFACE_H
