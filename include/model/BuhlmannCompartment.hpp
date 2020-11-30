//
// Created by guyfleeman on 11/28/20.
//

#ifndef LIBBUHLMAN_BUHLMANNCOMPARTMENT_HPP
#define LIBBUHLMAN_BUHLMANNCOMPARTMENT_HPP

#include <iostream>

#include "BuhlmannModelVersion.hpp"
#include "BuhlmannConstatnts.hpp"

namespace buhlmann {
namespace model {

class Compartment {
	//friend class Model;

public:
    Compartment() = default;

	Compartment(BuhlmannModelVersion version, int compartmentNum);

	Compartment(Compartment &compartment);

	~Compartment();

	void setNum(int n);

	void setVersion(BuhlmannModelVersion version);

	const CompartmentConstants_t& getConstants() const;

	float getCeiling(float gradientFactorSlope = 1.0f) const;

	float getNoDecompressionTime() const;

	float getPressureN2() const;

	float getPressureHe() const;

	void setPressureN2(float p);

	void setPressureHe(float p);

    friend std::ostream & operator<<(std::ostream & os, const Compartment & cp) {
        os << "Compartment " << cp.compartmentNum << ": ";
        os << "ceil: " << cp.getCeiling() << "ft. ";
        os << "NDL: " << cp.getNoDecompressionTime() << "s ";
        os << "(PPs: " << cp.getPressureN2() << " " << cp.getPressureHe() << ")";

        return os;
    }

	static const CompartmentConstants_t& getConstants(BuhlmannModelVersion version, int compartmentNum) {
	    if (version == BuhlmannModelVersion::ZHL_16C) {
	        return ZHL_16C_CONSTANTS[compartmentNum];
	    }

	    throw "BuhlmannCompartment.hpp: ERROR - version error";
	}
private:
    int compartmentNum = 16;
	BuhlmannModelVersion version = BuhlmannModelVersion::ZHL_16C;

	float pressureN2{};
	float pressureHe{};
};
};
};
#endif //LIBBUHLMAN_BUHLMANNCOMPARTMENT_HPP
