#ifndef CBSON_OBJECT_H__
#define CBSON_OBJECT_H__
#include "CBson.h"
#include <string>
	class CBsonArray;
	class CBsonElement;
	class CBsonObject
	{
	public:
		CBsonObject();
		bson_obj_map *m_Obj;
	public:
		int getIntValue(const std::string &string);
		void createNew();
		double getDoubleValue(const std::string &string);
		bool getBoolValue(const std::string &string);
		std::string getStringValue(const std::string &string);
		bin_obj *getBinaryValue(const std::string &string);
		CBsonObject getObjectValue(const std::string &string);
		CBsonArray getArrayValue(const std::string &string);
		CBsonElement getElementValue(const std::string &string);
		void  setIntValue(const std::string &string,int value);
		void  setBoolValue(const std::string &string,bool value);
		void  setDoubleValue(const std::string &string,double value);
		void  setStringValue(const std::string &string,const std::string &value);
		void  setObjectValue(const std::string &string,CBsonObject &value);
		void  setElementValue(const std::string &string,CBsonElement &value);
		void  setArrayValue(const std::string &string,CBsonArray &value);
		void  setBinaryValue(const std::string &string,bin_obj &bin);
		static  CBsonObject parse(bin_obj &bin);
		bin_obj save();
		bool isExist(const std::string &string);
	};

	class CBsonElement
	{
	public:
		bson_obj *m_Element;
	public:
		int getIntValue();
		bool getBoolValue();
		double getDoubleValue();
		std::string getStringValue();
		CBsonObject getObjectValue();
		CBsonArray getArray();
		static CBsonElement createIntElement(int value);
		static CBsonElement createBoolElement(bool value);
		static CBsonElement createDoubleElement(double value);
		static CBsonElement createStringElement(const std::string &value);
		static CBsonElement createOBjectElement(CBsonObject &value);
		static CBsonElement createArrayElement(CBsonArray &value);
		static CBsonElement createBinaryElement(bin_obj &bin);
	};
#define FOR_EACH_CBSON_ARRAY(ELEMENT,ARRAY) \
		bson_obj_list *macro_array=ARRAY.getList(); \
		bson_obj_list::iterator macro_it=macro_array->begin(); \
		for(;macro_it!=macro_array->end();macro_it++) \
	{ \
		bson_obj *macro_obj=*macro_it; \
		CBsonElement ELEMENT; \
		ELEMENT.m_Element=macro_obj; 

#define FOR_EACH_CBSON_ARRAY_END() }


#define FOR_EACH_CBSON_ARRAY_TAIL(ELEMENT,ARRAY) \
	bson_obj_list *macro_array=ARRAY.getList(); \
	bson_obj_list::iterator macro_it=macro_array->end(); \
	for(;macro_it!=macro_array->begin();macro_it--) \
	{ \
	bson_obj *macro_obj=*macro_it; \
	CBsonElement ELEMENT; \
	ELEMENT.m_Element=macro_obj; 

#define FOR_EACH_CBSON_ARRAY_TAIL_END() }
	class CBsonArray
	{
	public:
		bson_obj_list *m_List;

	public:
		void createNew();
		int count();
		bson_obj_list *getList();
		void addElement(CBsonElement &element);
	};
#endif
