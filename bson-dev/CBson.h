#ifndef CBSON_H__
#define CBSON_H__
#include <map>
#include <list>
#include <string>
#include "bson/bson.h"

typedef struct tag_bin_obj
{
	unsigned char *data;
	unsigned int size;
}bin_obj;
struct ptrCmp: public std::binary_function<const std::string*, const std::string*, bool>
{
	bool operator()( const std::string * s1, const std::string * s2 ) const
	{
		return strcmp( s1->c_str(), s2->c_str() ) < 0;
	}
};
typedef struct tag_bson_obj
{
	bson_type type;
	union{
		std::map<std::string*,tag_bson_obj*,ptrCmp> *obj;
		int i_v;
		double d_v;
		char *s_v;
		bin_obj *b_v;
		bool bool_v;
		std::list<tag_bson_obj*> *list;
		bson_date_t date;
	} value;
}bson_obj;
typedef std::map<std::string*,tag_bson_obj*,ptrCmp> bson_obj_map;
typedef std::list<tag_bson_obj*> bson_obj_list;
class CBson{
public:
	static bson_obj_map* parse(const char *buff,unsigned int size);
	static const char *save(bson_obj_map* dict,unsigned int *size);
	static void bson_obj_map_destroy(bson_obj_map* dict);
	static bin_obj *bin_dup(bin_obj *bin);
	static void bin_destroy(bin_obj *bin);
private:
	static bson_obj_map* __parse(bson_iterator *it);
	static bson_obj* __parse_value(bson_iterator *it);
	static void __save(bson_obj_map* dict,bson *b);
	static void __save_value(bson_obj *o,const char *key,bson *b);
	static void __bson_obj_destroy(bson_obj *o);
	static bson_obj_map *create_map();
};
#endif
