#ifndef TYPEINFO_H
#define TYPEINFO_H

#ifndef CMAKE_BUILD

#ifdef TYPEINFO_SHARED
#define TYPEINFOAPI __declspec(dllexport)
#else
#define TYPEINFOAPI
#endif

namespace std
{
}


#ifdef __cplusplus
extern "C"
{
#endif



struct std_type_info_data
{
	const char *volatile UndecoratedName = nullptr;
	const char DecoratedName[1];

	std_type_info_data() = delete;
	std_type_info_data(const std_type_info_data &) = delete;
	std_type_info_data(std_type_info_data &&) = delete;
	std_type_info_data &operator=(const std_type_info_data &) = delete;
	std_type_info_data &operator=(std_type_info_data &&) = delete;
};



#ifdef __cplusplus
}
#endif



extern "C++"
{

class type_info
{
	public:
	type_info(const type_info &) = delete;
	type_info &operator=(const type_info &) = delete;
	TYPEINFOAPI unsigned long long hash_code() const noexcept;
	TYPEINFOAPI bool operator==(const type_info &other) const noexcept;
	TYPEINFOAPI bool operator!=(const type_info &other) const noexcept;
	TYPEINFOAPI bool before(const type_info &other) const noexcept;
	TYPEINFOAPI const char *name() const noexcept;
	TYPEINFOAPI const char *raw_name() const noexcept;
	virtual ~type_info() noexcept;

	private:
	mutable std_type_info_data data;
};

}

#endif

#endif //TYPEINFO_H
