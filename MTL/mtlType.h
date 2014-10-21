#ifndef MTL_TYPE_H_INCLUDED__
#define MTL_TYPE_H_INCLUDED__

// http://stackoverflow.com/questions/18731075/c-fast-dynamic-type-subtype-check

typedef unsigned long long TypeID;

class mtlBase
{
protected:
	static bool IsType(TypeID id) { return (GetClassType() == id); }

public:
	static TypeID GetClassType( void ) { return 0; }
	virtual TypeID GetInstanceType( void ) const { return GetClassType(); }

	virtual bool IsInstanceType(TypeID id) const { return IsType(id); }
	bool IsInstanceType(const mtlBase &base) const { return IsInstanceType(base.GetInstanceType()); }
	template < typename other_t >
	bool IsInstanceType( void ) const { return IsInstanceType(other_t::GetClassType()); }

	static bool IsClassType(TypeID id) { return IsType(id); }
	static bool IsClassType(const mtlBase &base) { return IsType(base.GetInstanceType()); }
	template < typename other_t >
	static bool IsClassType( void ) { return IsType(other_t::GetClassType()); }
};

//
// Use the second template parameter to be able to make a
// distinction between two different objects that share
// the same base class. Declaration should look like the
// following:
//
// class Object : public mtlInherit<BaseObject, Object>
// ...
//

template < typename base_t, typename type_t = int >
class mtlInherit : public base_t
{
private:
	// This type has to be of type_t. Using a small value
	// such as 'char' for all mtlInherit<base, ...> types
	// causes MSVC to optimize memory layout and make sure
	// that all mtlInherit<base, ...> share the same static
	// member despite being of different types, for instance
	// &mtlInherit<someClass, child1>::m_typeAddress == &mtlInherit<someClass, child2>::m_typeAddress
	static type_t *m_typeAddress;

protected:
	static bool IsType(TypeID id) { return (GetClassType() != id) ? base_t::IsType(id) : true; }

public:
	static TypeID GetClassType( void ) { return (TypeID)(&m_typeAddress); }
	virtual TypeID GetInstanceType( void ) const { return GetClassType(); }

	virtual bool IsInstanceType(TypeID id) const { return IsType(id); }
	bool IsInstanceType(const mtlBase &base) const { return IsInstanceType(base.GetInstanceType()); }
	template < typename other_t >
	bool IsInstanceType( void ) const { return IsInstanceType(other_t::GetClassType()); }

	static bool IsClassType(TypeID id) { return IsType(id); }
	static bool IsClassType(const mtlBase &base) { return IsType(base.GetInstanceType()); }
	template < typename other_t >
	static bool IsClassType( void ) { return IsType(other_t::GetClassType()); }
};

template < typename base_t, typename type_t > type_t *mtlInherit<base_t, type_t>::m_typeAddress = NULL; // we don't care about initialization order since we are not interested in it's value

#endif
