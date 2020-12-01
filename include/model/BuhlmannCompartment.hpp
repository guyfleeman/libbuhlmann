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

/**
 * represents a tissue compartment in the Buhlmann model
 */
class Compartment {
	//friend class Model;

public:
    /**
     * default constructor
     *
     * creates an empty compartment
     */
    Compartment() = default;

    /**
     * creates a compartment
     * @param version model/algorithm version
     * @param compartmentNum index
     */
	Compartment(BuhlmannModelVersion version, int compartmentNum);

	/**
	 * copy constructor
	 * @param compartment
	 */
	Compartment(Compartment &compartment);

	/**
	 * destructor
	 */
	~Compartment();

	/**
	 * sets the compartment index
	 * @param n
	 */
	void setNum(int n);

	/**
	 * sets the version
	 * @param version
	 */
	void setVersion(BuhlmannModelVersion version);

	/**
	 * gets the gas constants associated with the version and compartment index
	 * @return
	 */
	const CompartmentConstants_t& getConstants() const;

	/**
	 * gets the current compartment ceiling in atm
	 * @param gradientFactorSlope gradient factor correction
	 * @return ceiling
	 */
	float getCeiling(float gradientFactorSlope = 1.0f) const;

	/**
	 * gets the time remaining for the compartment before there's a decompression obligation
	 * @return
	 */
	float getNoDecompressionTime() const;

	/**
	 * gets the Nitrogen component of the compartment pressure
	 * @return
	 */
	float getPressureN2() const;

	/**
	 * gets the Helium component of the compartment pressure
	 * @return
	 */
	float getPressureHe() const;

	/**
	 * sets the Nitrogen component of the compartment pressure
	 * @param p
	 */
	void setPressureN2(float p);

	/**
	 * sets the Helium component of the compartment pressure
	 * @param p
	 */
	void setPressureHe(float p);

	/**
	 * prints the compartment
	 * @param os
	 * @param cp
	 * @return
	 */
    friend std::ostream & operator<<(std::ostream & os, const Compartment & cp) {
        os << "Compartment " << cp.compartmentNum << ": ";
        os << "ceil: " << cp.getCeiling() << "ft. ";
        os << "NDL: " << cp.getNoDecompressionTime() << "s ";
        os << "(PPs: " << cp.getPressureN2() << " " << cp.getPressureHe() << ")";

        return os;
    }

    /**
     * gets constants
     * @param version
     * @param compartmentNum
     * @return
     */
	static const CompartmentConstants_t& getConstants(BuhlmannModelVersion version, int compartmentNum) {
	    if (version == BuhlmannModelVersion::ZHL_16C) {
	        return ZHL_16C_CONSTANTS[compartmentNum];
	    }

	    throw "BuhlmannCompartment.hpp: ERROR - version error";
	}
private:
    /**
     * compartment index in the model
     */
    int compartmentNum = 16;

    /**
     * model version
     */
	BuhlmannModelVersion version = BuhlmannModelVersion::ZHL_16C;

	/**
	 * nitrogen component pressure
	 */
	float pressureN2{};

	/**
	 * helium component pressure
	 */
	float pressureHe{};
};
};
};
#endif //LIBBUHLMAN_BUHLMANNCOMPARTMENT_HPP
