//
// Created by guyfleeman on 10/17/2020.
//

#ifndef LIBBUHLMAN_INVALIDVALUEEXCEPTION_HPP
#define LIBBUHLMAN_INVALIDVALUEEXCEPTION_HPP

#include <exception>

namespace libbuhlman {
	class InvalidValueException : public std::exception {
		const char *what() const throw() {
			return "Invalid Value Exception";
		}
	};
}


#endif //LIBBUHLMAN_INVALIDVALUEEXCEPTION_HPP
