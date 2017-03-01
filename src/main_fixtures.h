#pragma once

#include "YAIP++.h"

/**
 * Base class for fixtures and tests
 */
class YAIPTestFixtureBase
{
public:
	/**
	 * Instance of INI parser
	 */
	org::derpaul::yaip::YAIP IniParser;

	/**
	 * Name of sample INI file
	 */
	static const std::string INIFilename;

	/**
	 * Name of section
	 */
	static const std::string Section;

	/**
	 * Name of key for bool
	 */
	static const std::string KeyBool;

	/**
	 * Name of key for double
	 */
	static const std::string KeyDouble;

	/**
	 * Name of key for float
	 */
	static const std::string KeyFloat;

	/**
	 * Name of key for integer
	 */
	static const std::string KeyInteger;

	/**
	 * Name of key for long
	 */
	static const std::string KeyLong;

	/**
	 * Name of key for string
	 */
	static const std::string KeyString;
};

/**
 * Class for fixtures and tests for saving data
 */
class YAIPTestFixtureSave : public YAIPTestFixtureBase
{
public:
	/**
	 * Value for bool
	 */
	static const bool ValueBool;

	/**
	 * Value for double
	 */
	static const double ValueDouble;

	/**
	 * Value for float
	 */
	static const float ValueFloat;

	/**
	 * Value for integer
	 */
	static const int ValueInteger;

	/**
	 * Value for long
	 */
	static const long ValueLong;

	/**
	 * Value for string
	 */
	static const std::string ValueString;
};

/**
 * Class for fixtures and tests for loading data
 */
class YAIPTestFixtureLoad : public YAIPTestFixtureSave
{
public:
	/**
	 * Default value for bool
	 */
	static const bool DefaultBool;

	/**
	 * Default value for double
	 */
	static const double DefaultDouble;

	/**
	 * Default value for float
	 */
	static const float DefaultFloat;

	/**
	 * Default value for integer
	 */
	static const int DefaultInteger;

	/**
	 * Default value for long
	 */
	static const long DefaultLong;

	/**
	 * Default value for string
	 */
	static const std::string DefaultString;
};

/**
 * Class for fixtures and tests for reading section list
 */
class YAIPTestFixtureSectionList : public YAIPTestFixtureBase
{
};

/**
 * Class for fixtures and tests for reading section key list
 */
class YAIPTestFixtureSectionKeyList : public YAIPTestFixtureBase
{
};

/**
 * Class for fixtures and tests for removing section key
 */
class YAIPTestFixtureSectionKeyKill : public YAIPTestFixtureBase
{
};

/**
 * Class for fixtures and tests for removing section
 */
class YAIPTestFixtureSectionKill : public YAIPTestFixtureBase
{
};
