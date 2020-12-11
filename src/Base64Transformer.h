#pragma once

#include <stdlib.h>
#include <string>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/remove_whitespace.hpp>

using namespace boost::archive::iterators;

std::string base64EncoderDecoder(bool shouldEncode, std::string iStringToCipher)
{
	typedef transform_width<binary_from_base64<remove_whitespace<std::string::const_iterator> >, 8, 6 > it_binary_t;
  	typedef insert_linebreaks<base64_from_binary<transform_width<std::string::const_iterator,6,8> >, 72 > it_base64_t;

	if (shouldEncode)
	{
		unsigned int writePaddChars = (3-iStringToCipher.length()%3)%3;
		std::string base64(it_base64_t(iStringToCipher.begin()),it_base64_t(iStringToCipher.end()));
		base64.append(writePaddChars,'=');
		return base64;
	}

	unsigned int paddChars = count(iStringToCipher.begin(), iStringToCipher.end(), '=');
	std::replace(iStringToCipher.begin(),iStringToCipher.end(),'=','A');
	std::string result(it_binary_t(iStringToCipher.begin()), it_binary_t(iStringToCipher.end()));
	result.erase(result.end()-paddChars,result.end());
	return result;
}