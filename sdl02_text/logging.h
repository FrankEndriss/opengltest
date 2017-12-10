/*
 * logging.h
 * Interface to logging stuff
 * see http://www.boost.org/doc/libs/1_63_0/libs/log/doc/html/log/tutorial.html#log.tutorial.trivial
 * TODO find/implement pattern to settup class specific loggers, see "Loggers with channel support"
 * http://www.boost.org/doc/libs/1_63_0/libs/log/doc/html/log/detailed/sources.html
 */

#ifndef LOGGING_H_
#define LOGGING_H_

#include <boost/log/trivial.hpp>

#define TRACE	BOOST_LOG_TRIVIAL(trace)
#define DEBUG	BOOST_LOG_TRIVIAL(debug)
#define INFO	BOOST_LOG_TRIVIAL(info)
#define WARN	BOOST_LOG_TRIVIAL(warning)
#define ERROR	BOOST_LOG_TRIVIAL(error)
#define FATAL	BOOST_LOG_TRIVIAL(fatal)

#endif /* LOGGING_H_ */
