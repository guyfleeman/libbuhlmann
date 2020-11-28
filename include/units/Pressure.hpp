//
// Created by guyfleeman on 10/17/2020.
//

#ifndef LIBBUHLMAN_PRESSURE_HPP
#define LIBBUHLMAN_PRESSURE_HPP

enum PressureUnit {
	ATM,
	BAR
};

class Pressure {
public:
	Pressure(float val, const PressureUnit unit);
	virtual ~Pressure();

	void set(Pressure val);
	void set(float val, PressureUnit unit);

	Pressure get();
	float get(PressureUnit unit);

private:
	float val;
	PressureUnit unit;
};


#endif //LIBBUHLMAN_PRESSURE_HPP
