#include "CBson.h"

bson_obj_map *CBson::parse(const char *buff,unsigned int size)
{
	bson_iterator *it=bson_iterator_alloc();;
	bson_iterator_from_buffer(it,buff);
	bson_obj_map *dict=CBson::__parse(it);
	bson_iterator_dealloc(it);
	return dict;
}

bson_obj_map *CBson::__parse(bson_iterator *it)
{
	bson_obj_map *dict=new bson_obj_map();
	while(bson_iterator_next(it))
	{
		bson_obj* v= CBson::__parse_value(it);
		const char *key=bson_iterator_key(it);
		(*dict)[new std::string(key)]=v;
	}
	return dict;
}

bson_obj* CBson::__parse_value(bson_iterator *it)
{
	bson_type type=bson_iterator_type(it);
	bson_obj *r=new bson_obj();
	r->type=type;
	switch(type)
	{
	case BSON_DOUBLE:
		{
			double v=bson_iterator_double(it);
			r->value.d_v=v;
			break;
		}
	case BSON_INT:
		{
			int v=bson_iterator_int(it);
			r->value.i_v=v;
			break;
		}
	case BSON_LONG:
		{
			long v=bson_iterator_long(it);
			r->value.i_v=v;
			break;
		}
	case BSON_BOOL:
		{
			bool v =bson_iterator_bool(it);
			r->value.bool_v=v;
			break;
		}
	case BSON_STRING:
		{
			const char *v=bson_iterator_string(it);
			r->value.s_v=strdup(v);
			break;
		}
	case BSON_NULL:
		{
			
			break;
		}
	case BSON_DATE:
		{
			bson_date_t date=bson_iterator_date(it);
			r->value.date=date;
			break;
		}
	case BSON_BINDATA:
		{
			const unsigned char *v=(const unsigned char*)bson_iterator_bin_data(it);
			unsigned int size= bson_iterator_bin_len(it);
			bin_obj *bin=new bin_obj();
			bin->data=(unsigned char*)new char[size];
			memcpy(bin->data,v,size);
			bin->size=size;
			r->value.b_v=bin;
			break;
		}
	case BSON_OBJECT:
		{
			bson_iterator *s_it=bson_iterator_alloc();
			bson_iterator_subiterator(it,s_it);
			bson_obj_map *s= CBson::__parse(s_it);
			r->value.obj=s;
			bson_iterator_dealloc(s_it);
			break;
		}
	
	case BSON_ARRAY:
		{
			bson_iterator *s_it=bson_iterator_alloc();
			bson_iterator_subiterator(it,s_it);
			bson_obj_list *array=new bson_obj_list();
			while(bson_iterator_next(s_it))
			{
				bson_obj* v= CBson::__parse_value(s_it);
				array->push_back(v);
			}
			r->value.list=array;
			bson_iterator_dealloc(s_it);
			break;
		}
	

	}
	return r;
}

const char *CBson::save(bson_obj_map* dict,unsigned int *size)
{
	bson *b=bson_alloc();
	bson_init(b);
	__save(dict,b);
	bson_finish(b);
	*size=(unsigned int)bson_size(b);
	unsigned char *buff=(unsigned char *)malloc(*size);
	memcpy(buff,bson_data(b),*size);
	bson_destroy(b);
	bson_dealloc(b);
	return (const char*)buff;
}

void CBson::__save(bson_obj_map* dict,bson *b)
{
	bson_obj_map::iterator it=dict->begin();
	for(;it!=dict->end();it++)
		{
			std::string *key=it->first;
			bson_obj *o=it->second;
			CBson::__save_value(o,key->c_str(),b);
		}
}

void CBson::__save_value(bson_obj *o,const char *key,bson *b)
{
	switch(o->type)
	{
	case BSON_DOUBLE:
		{
			bson_append_double(b,key,o->value.d_v);
			break;
		}
	case BSON_INT:
		{
			bson_append_int(b,key,o->value.i_v);
			break;
		}
	case BSON_BOOL:
		{
			bson_append_bool(b,key,o->value.bool_v);
			break;
		}
	case BSON_STRING:
		{
			bson_append_string(b,key,o->value.s_v);
			break;
		}
	case BSON_NULL:
		{
			bson_append_null(b,key);
			break;
		}
	case BSON_DATE:
		{
			bson_append_date(b,key,o->value.date);
			break;
		}
	case BSON_BINDATA:
		{
			bson_append_binary(b,key,BSON_BIN_BINARY,(const char*)o->value.b_v->data,o->value.b_v->size);
			break;
		}
	case BSON_OBJECT:
		{
			bson_append_start_object(b,key);
			CBson::__save(o->value.obj,b);
			bson_append_finish_object(b);
			break;
		}

	case BSON_ARRAY:
		{
			bson_append_start_array(b,key);
			bson_obj_list::iterator it=o->value.list->begin();
			for(;it!=o->value.list->end();it++)
			{
				bson_obj *obj=*it;
				__save_value(obj,"",b);
			}
			bson_append_finish_array(b);
			break;
		}
	}
	
}


void CBson::bson_obj_map_destroy(bson_obj_map* dict)
{
	bson_obj_map::iterator it=dict->begin();
	for(;it!=dict->end();it++)
	{
		delete it->first;
		bson_obj *o=it->second;
		__bson_obj_destroy(o);
		delete o;
	}
	dict->clear();
	delete dict;
}

void CBson::__bson_obj_destroy(bson_obj *o)
{
	switch(o->type)
	{
	case BSON_DOUBLE:
		{
			
			break;
		}
	case BSON_INT:
		{
			
			break;
		}
	case BSON_STRING:
		{
			
			free(o->value.s_v);
			break;
		}

	case BSON_BINDATA:
		{
			delete[] o->value.b_v->data;
			delete o->value.b_v;
			break;
		}
	case BSON_OBJECT:
		{
			CBson::bson_obj_map_destroy(o->value.obj);
			break;
		}

	case BSON_ARRAY:
		{

			bson_obj_list::iterator it=o->value.list->begin();
			for(;it!=o->value.list->end();it++)
			{
				bson_obj *obj=*it;
				__bson_obj_destroy(obj);
			}
			o->value.list->clear();
			delete o->value.list;
			break;
		}
	}
}

 bin_obj *CBson::bin_dup(bin_obj *bin)
{
	bin_obj *new_bin=NULL;
	if(bin)
	{
		new_bin=new bin_obj;
		new_bin->data=new unsigned char[bin->size];
		memcpy(new_bin->data,bin->data,bin->size);
		new_bin->size=bin->size;
	}
	return new_bin;
}
void CBson::bin_destroy(bin_obj *bin)
{
	if(bin)
	{
		delete[] bin->data;
		delete bin;
	}
}