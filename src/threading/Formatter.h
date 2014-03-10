// See the file "COPYING" in the main distribution directory for copyright.

#ifndef THREADING_FORMATTER_H
#define THREADING_FORMATTER_H

#include "../Desc.h"
#include "MsgThread.h"

namespace threading { namespace formatter {

/**
  * A thread-safe class for converting values into some textual format. 
  * This is a base class that implements the interface forcommon 
  * rendering/parsing code needed by a number of input/output threads.
  */
class Formatter {
public:
	/**
	 * Constructor.
	 *
	 * @param t The thread that uses this class instance. The class uses
	 * some of the thread's methods, e.g., for error reporting and
	 * internal formatting.
	 *
	 */
	Formatter(threading::MsgThread* t);

	/**
	 * Destructor.
	 */
	virtual ~Formatter();

	/**
	 * Convert a list of threading values into an implementation specific representation.
	 *
	 * @param desc The ODesc object to write to.
	 *
	 * @param num_fields The number of fields in the logging record.
	 *
	 * @param fields Information about the fields for each of the given log values.
	 *
	 * @param vals The field values.
	 *
	 * @return Returns true on success, false on error. Errors are also
	 * flagged via the reporter.
	 */
	virtual bool Describe(ODesc* desc, int num_fields, const threading::Field* const * fields,
	                      threading::Value** vals) const = 0;

	/**
	 * Convert a threading value into an implementation specific representation.
	 *
	 * @param desc The ODesc object to write to.
	 *
	 * @param val the Value to render to the ODesc object.
	 *
	 * @return Returns true on success, false on error. Errors are also
	 * flagged via the reporter.
	 */
	virtual bool Describe(ODesc* desc, threading::Value* val) const = 0;

	/**
	 * Convert a threading value into an implementation specific representation.
	 *
	 * @param desc The ODesc object to write to.
	 *
	 * @param val the Value to render to the ODesc object.
	 *
	 * @param The name of a field associated with the value.
	 *
	 * @return Returns true on success, false on error. Errors are also
	 * flagged via the reporter.
	 */
	virtual bool Describe(ODesc* desc, threading::Value* val, const string& name) const = 0;

	/**
	 * Convert a representation of a field into a value.
	 *
	 * @param s The string to parse.
	 *
	 * @param The name of a field associated with the value. Used only
	 * for error reporting.
	 *
	 * @return The new value, or null on error. Errors are also flagged
	 * via the reporter.
	 */
	virtual threading::Value* ParseValue(string s, string name, TypeTag type, TypeTag subtype = TYPE_ERROR) const = 0;

	/**
	 * Convert an IP address into a string.
	 *
	 * @param addr The address.
	 *
	 * @return An ASCII representation of the address.
	 */
	string Render(const threading::Value::addr_t& addr) const;

	/**
	 * Convert an subnet value into a string.
	 *
	 * @param addr The address.
	 *
	 * @return An ASCII representation of the subnet.
	 */
	string Render(const threading::Value::subnet_t& subnet) const;

	/**
	 * Convert a double into a string. This renders the double with Bro's
	 * standard precision.
	 *
	 * @param d The double.
	 *
	 * @return An ASCII representation of the double.
	 */
	string Render(double d) const;

	/**
	 * Convert a string into a TransportProto. The string must be one of
	 * \c tcp, \c udp, \c icmp, or \c unknown.
	 *
	 * @param proto The transport protocol
	 *
	 * @return The transport protocol, which will be \c TRANSPORT_UNKNOWN
	 * on error. Errors are also flagged via the reporter.
	 */
	TransportProto ParseProto(const string &proto) const;

	/**
	 * Convert a string into a Value::addr_t.
	 *
	 * @param addr String containing an IPv4 or IPv6 address.
	 *
	 * @return The address, which will be all-zero on error. Errors are
	 * also flagged via the reporter.
	 */
	threading::Value::addr_t ParseAddr(const string &addr) const;

protected:
	bool CheckNumberError(const string& s, const char * end) const;

	threading::MsgThread* thread;
};

}}

#endif /* THREADING_FORMATTER_H */