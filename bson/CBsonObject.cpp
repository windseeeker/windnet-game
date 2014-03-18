#include "CBsonObject.h"


CBsonObject::CBsonObject():m_Obj(NULL)
{


}
void CBsonObject::createNew()
{
	if(m_Obj)
	{
		CBson::bson_obj_map_destroy(m_Obj);
	}
	m_Obj=new bson_obj_map;
}
 CBsonObject CBsonObject::parse(bin_obj &bin)
 {
	 CBsonObject obj;
	 obj.m_Obj=CBson::parse((const char*)bin.data,bin.size);
	 return obj;
 }

 bin_obj CBsonObject::save()
 {
	 bin_obj bin;
	 bin.data=(unsigned char*)CBson::save(m_Obj,&bin.size);
	 return bin;
 }

 int CBsonObject::getIntValue(const std::string &string)
 {
	 CBsonElement element=getElementValue(string);
	 if(element.m_Element)
	 {
		  return element.m_Element->value.i_v;
	 }
	 return 0;
	
 }

 bool CBsonObject::getBoolValue(const std::string &string)
 {
	  CBsonElement element=getElementValue(string);
	   return element.m_Element->value.bool_v;
 }
 double CBsonObject::getDoubleValue(const std::string &string)
 {
	 CBsonElement element=getElementValue(string);
	 return element.m_Element->value.d_v;
 }
 std::string CBsonObject::getStringValue(const std::string &string)
 {
	  CBsonElement element=getElementValue(string);
	  const char *value="";
	  if(element.m_Element)
	  {
		  value=element.m_Element->value.s_v;
	  }
	  return std::string(value);
 }

 bin_obj *CBsonObject::getBinaryValue(const std::string &string)
 {
	  CBsonElement element=getElementValue(string);
	  return element.m_Element->value.b_v;
 }
 CBsonObject CBsonObject::getObjectValue(const std::string &string)
 {
	 CBsonElement element=getElementValue(string);
	 CBsonObject obj;
	 obj.m_Obj=NULL;
	 if (element.m_Element == NULL) { /////////by william(shc), to protect temporarily
		 return obj;
	 }
	 if(element.m_Element->type==BSON_OBJECT)
	 {
		 obj.m_Obj=element.m_Element->value.obj;
	 }
	 return obj;
 }
 CBsonArray CBsonObject::getArrayValue(const std::string &string)
 {
	  CBsonElement element=getElementValue(string);
	  CBsonArray array;
	  array.m_List=NULL;
	  if(element.m_Element->type==BSON_ARRAY)
	  {
		array.m_List=element.m_Element->value.list;
	  }
	  return array;
 }
 CBsonElement CBsonObject::getElementValue(const std::string &string)
 {
	 std::string key=string;
	 bson_obj *obj=(*m_Obj)[&key];
	 CBsonElement element;
	 element.m_Element=obj;
	 return element;
 }
 void  CBsonObject::setIntValue(const std::string &string,int value)
 {
	 CBsonElement element=CBsonElement::createIntElement(value);
	 setElementValue(string,element);
 }
 void  CBsonObject::setBoolValue(const std::string &string,bool value)
 {
	 CBsonElement element=CBsonElement::createBoolElement(value);
	 setElementValue(string,element);
 }
 void  CBsonObject::setDoubleValue(const std::string &string,double value)
 {
	 CBsonElement element=CBsonElement::createDoubleElement(value);
	 setElementValue(string,element);
 }
 void  CBsonObject::setStringValue(const std::string &string,const std::string &value)
 {
	 CBsonElement element=CBsonElement::createStringElement(value);
	 setElementValue(string,element);
 }
 void  CBsonObject::setObjectValue(const std::string &string,CBsonObject &value)
 {
	 CBsonElement element=CBsonElement::createOBjectElement(value);
	 setElementValue(string,element);
 }
 void  CBsonObject::setElementValue(const std::string &string,CBsonElement &value)
 {
	 bson_obj *obj=value.m_Element;
	 (*m_Obj)[new std::string(string)]=obj;

 }
 void  CBsonObject::setArrayValue(const std::string &string,CBsonArray &value)
 {
	 CBsonElement element=CBsonElement::createArrayElement(value);
	 setElementValue(string,element);
 }
 void  CBsonObject::setBinaryValue(const std::string &string,bin_obj &bin)
 {
	 CBsonElement element= CBsonElement::createBinaryElement(bin);
	 setElementValue(string,element);
 }

 bool CBsonObject::isExist(const std::string &str)
 {
	 std::string key=str;
	 bson_obj_map::iterator it=m_Obj->find(&key);
	 return it!=m_Obj->end();
 }

 //================CBsonElement=============


 int CBsonElement::getIntValue()
 {
	 return m_Element->value.i_v;
 }
 bool CBsonElement::getBoolValue()
 {
	 return m_Element->value.bool_v;
 }
 double CBsonElement::getDoubleValue()
 {
	  return m_Element->value.d_v;
 }
 std::string CBsonElement::getStringValue()
 {
	 return m_Element->value.s_v;
 }
 CBsonObject CBsonElement::getObjectValue()
 {
	 CBsonObject obj;
	 obj.m_Obj=m_Element->value.obj;
	 return obj;
 }
 CBsonArray CBsonElement::getArray()
 {
	 CBsonArray array;
	 array.m_List=m_Element->value.list;
	 return array;
 }
 CBsonElement CBsonElement::createIntElement(int value)
 {
	 bson_obj *obj=new bson_obj();
	 obj->type=BSON_INT;
	 obj->value.i_v=value;
	 CBsonElement element;
	 element.m_Element=obj;
	 return element;
 }
 CBsonElement CBsonElement::createBoolElement(bool value)
 {
	 bson_obj *obj=new bson_obj();
	 obj->type=BSON_BOOL;
	 obj->value.bool_v=value;
	 CBsonElement element;
	 element.m_Element=obj;
	 return element;
 }
  CBsonElement CBsonElement::createDoubleElement(double value)
  {
	  bson_obj *obj=new bson_obj();
	  obj->type=BSON_DOUBLE;
	  obj->value.d_v=value;
	  CBsonElement element;
	  element.m_Element=obj;
	  return element;
  }
  CBsonElement CBsonElement::createStringElement(const std::string &value)
  {
	  bson_obj *obj=new bson_obj();
	  obj->type=BSON_STRING;
	  obj->value.s_v=strdup(value.c_str());
	  CBsonElement element;
	  element.m_Element=obj;
	  return element;
  }
  CBsonElement CBsonElement::createOBjectElement(CBsonObject &value)
  {
	  bson_obj *obj=new bson_obj();
	  obj->type=BSON_OBJECT;
	  obj->value.obj=value.m_Obj;
	  CBsonElement element;
	  element.m_Element=obj;
	  return element;
  }
  CBsonElement CBsonElement::createArrayElement(CBsonArray &value)
  {
	  bson_obj *obj=new bson_obj();
	  obj->type=BSON_ARRAY;
	  obj->value.list=value.m_List;
	  CBsonElement element;
	  element.m_Element=obj;
	  return element;
  }
  CBsonElement CBsonElement::createBinaryElement(bin_obj &bin)
  {
	   bson_obj *obj=new bson_obj();
	   bin_obj *bo=new bin_obj;
	   unsigned char *data=new unsigned char[bin.size];
	   memcpy(data,bin.data,bin.size);
	   bo->data=data;
	   bo->size=bin.size;
	   obj->type=BSON_BINDATA;
	   obj->value.b_v=bo;
	   CBsonElement element;
	   element.m_Element=obj;
	   return element;
  }

  void CBsonArray::createNew()
  {
	  m_List=new bson_obj_list;
  }
  int CBsonArray::count()
  {
	  int size=0;
	  if(m_List)
	  {
		  size=m_List->size();
	  }
	  return size;
  }
  bson_obj_list *CBsonArray::getList()
  {
	  return m_List;
  }
  void CBsonArray::addElement(CBsonElement &element)
  {
	  if(m_List)
	  {
		  m_List->push_back(element.m_Element);
	  }
  }
