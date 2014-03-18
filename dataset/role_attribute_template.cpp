#include "role_attribute_template.h"

#include "../assert.h"

#include "csv_reader.h"

namespace Dataset {
bool RoleAttributeTemplate::init() {
	CSVReader reader;
	if (!reader.loadFile("./data/hero.csv")) {
		ASSERT(false);
	}
	int level, val;
	std::string str;
    do {
		RoleAttribute::ptr attr(new RoleAttribute);
		ASSERT(reader.Attribute("level", level));
		ASSERT(reader.Attribute("hp", attr->hp));
		ASSERT(reader.Attribute("leve_exp", attr->levelExp));
		ASSERT(reader.Attribute("mp", attr->mp)); 
		ASSERT(reader.Attribute("min_attack", attr->minPhysicalAttack));
		ASSERT(reader.Attribute("max_attack", attr->maxPhysicalAttack)); 
		ASSERT(reader.Attribute("min_magic_attack", attr->minMagicAttack)); 
		ASSERT(reader.Attribute("max_magic_attack", attr->maxMagicAttack)); 
		ASSERT(reader.Attribute("defense", attr->defence)); 
		ASSERT(reader.Attribute("Magic_defense", attr->magicDefence));
		ASSERT(reader.Attribute("hit", attr->hit));
		ASSERT(reader.Attribute("dodge", attr->dodge));
		ASSERT(reader.Attribute("toughness", attr->toughness));
		ASSERT(reader.Attribute("move", attr->moveSpeed));
		//ASSERT(reader.Attribute("attack_speed", attr->attackSpeed));
		ASSERT(reader.Attribute("aggressivity", attr->aggressivity));
		ASSERT(reader.Attribute("skill_id_1", val));
		attr->skills.push_back(val);

		ASSERT(reader.Attribute("skill_id_2", val));
        attr->skills.push_back(val);

		ASSERT(reader.Attribute("skill_id_3", val));
        attr->skills.push_back(val);

		m_attributes.insert(std::make_pair(level, attr)); 
    }  while (reader.moveNext());

	return true;
}

RoleAttribute::ptr RoleAttributeTemplate::get(short level) {
	std::map<int, RoleAttribute::ptr>::iterator it = m_attributes.find(level);
	return it == m_attributes.end() ? RoleAttribute::ptr() : it->second;
}
}
