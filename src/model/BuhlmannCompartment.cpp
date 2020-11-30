//
// Created by guyfleeman on 11/28/20.
//

#include "BuhlmannCompartment.hpp"

using namespace buhlmann;
using namespace buhlmann::model;

Compartment::Compartment(BuhlmannModelVersion version, int compartmentNum)
{
    this->version = version;
    this->compartmentNum = compartmentNum;
}

Compartment::Compartment(Compartment &compartment)
{
    this->version = compartment.version;
    this->compartmentNum = compartment.compartmentNum;
    this->pressureN2 = compartment.getPressureN2();
    this->pressureHe = compartment.getPressureHe();
}

Compartment::~Compartment()
= default;

void Compartment::setNum(int n)
{
    this->compartmentNum = n;
}

void Compartment::setVersion(BuhlmannModelVersion version)
{
    this->version = version;
}

float Compartment::getCeiling(float gradientFactorSlope) const
{
    float compositePressure = pressureN2 + pressureHe;
    float aFactor = (getConstants().nitrogenA * pressureN2 + getConstants().heliumA * pressureHe) / compositePressure;
    float bFactor = (getConstants().nitrogenB * pressureN2 + getConstants().heliumB * pressureHe) / compositePressure;

    float gfCorrectionNumerator = compositePressure - aFactor * gradientFactorSlope;
    float gfCorrectionDenominator = gradientFactorSlope / bFactor + 1.0f - gradientFactorSlope;
    float safeAscentCeiling = gfCorrectionNumerator / gfCorrectionDenominator;

    return safeAscentCeiling;
    /*
    float stopN2, stopHe;
    stopN2 = (pressureN2 - getConstants().nitrogenA) * getConstants().nitrogenB;
    stopHe = (pressureHe - getConstants().heliumA) * getConstants().heliumB;

    return std::max(stopN2, stopHe);
     */
}

float Compartment::getNoDecompressionTime() const
{
    return 0.0f;
}

float Compartment::getPressureN2() const
{
    return pressureN2;
}

float Compartment::getPressureHe() const
{
    return pressureHe;
}

void Compartment::setPressureN2(float p)
{
    pressureN2 = p;
}

void Compartment::setPressureHe(float p)
{
    pressureHe = p;
}

const CompartmentConstants_t& Compartment::getConstants() const
{
    return Compartment::getConstants(version, compartmentNum);
}

