/**
 * \file	YAIP++.cpp
 * \author	ThirtySomething
 * \date	2016-09-19
 * \brief	Implementation of Yet Another INI Parser
 */
#include "YAIP++.h"
#include <iostream>
#include <fstream>

/**
 * Namespace of YAIP
 */
namespace YAIP
{
	// ******************************************************************
	// ******************************************************************
	// Regular expressioin for matching a key/value pair
	const std::regex YAIP::RegExKeyValue("([\\s]+)?(([a-zA-Z0-9\\.])+){1}([\\s]+)?(=){1}([\\s]+)?(([a-zA-Z0-9\\.\\+-])+)?([\\s]+)?([#;])?(.)*");
	//                                        1           2                 3       4      5               6                7        8    9
	// 1 - Possible whitespaces
	// 2 - Key => Match everything with upper-/lowercase characters, numbers and a dot
	// 3 - Possible whitespaces
	// 4 - The assignment
	// 5 - Possible whitespaces
	// 6 - Value => Match everything with upper-/lowercase characters, numbers, dot, plus and minus sign
	// 7 - Possible whitespaces
	// 8 - Marker for "comment start"
	// 9 - Comment

	// ******************************************************************
	// ******************************************************************
	// Regular expressioin for matching a section
	const std::regex YAIP::RegExSection("([\\s]+)?(\\[){1}([\\s]+)?(([a-zA-Z0-9])+){1}([\\s]+)?(\\]){1}([\\s]+)?([#;])?(.)*");
	//                                      1       2        3            4              5       6        7       8    9
	// 1 - Possible whitespaces
	// 2 - Opening square bracked
	// 3 - Possible whitespaces
	// 4 - Section => Match everything with upper-/lowercase characters and numbers
	// 5 - Possible whitespaces
	// 6 - Closing square bracked
	// 7 - Possible whitespaces
	// 8 - Marker for "comment start"
	// 9 - Comment

	// ******************************************************************
	// ******************************************************************
	YAIP::YAIP(void)
		:m_IniData()
	{
	}

	// ******************************************************************
	// ******************************************************************
	YAIP::~YAIP(void)
	{
	}

	// ******************************************************************
	// ******************************************************************
	void YAIP::INIFileLoad(std::string Filename)
	{
		std::ifstream IniFile;

		// Always clear internal storage
		m_IniData.clear();

		// Open the INI file for reading
		IniFile.open(Filename, std::ios::in);

		/**
		 * \todo Handle file errors like file does not exist
		 */
		if (true == IniFile.is_open())
		{
			std::string CurrentLine;

			// Temporary storage for all lines of the INI file
			tVectorString FileContent;

			// Read as long as lines exist
			while (std::getline(IniFile, CurrentLine))
			{
				FileContent.push_back(CurrentLine);
			}
			IniFile.close();

			// Parse INI file
			ParseFileContent(FileContent);
		}
	}

	// ******************************************************************
	// ******************************************************************
	void YAIP::INIFileSave(std::string Filename)
	{
		std::ofstream IniFile;

		// Open the INI file for writing
		IniFile.open(Filename, std::ios::trunc);

		/**
		 * \todo Handle file errors like cannot open file
		 */
		if (true == IniFile.is_open())
		{
			// Get a list of all existing sections
			tVectorString SectionList = SectionListGet();
			for (tVectorString::iterator LoopSection = SectionList.begin(); LoopSection != SectionList.end(); ++LoopSection)
			{
				// Save section
				INIFileSaveSection(*LoopSection, IniFile);
			}

			IniFile.close();
		}
	}

	// ******************************************************************
	// ******************************************************************
	void YAIP::INIFileSaveSection(std::string Section, std::ofstream &IniFile)
	{
		// Retrieve list of keys
		tVectorString KeyList = SectionKeyListGet(Section);

		// Write only if keys are available - no keys, no write
		if (0 < KeyList.size())
		{
			// Write section marker
			if (0 != Section.length())
			{
				IniFile << "[" << Section << "]" << std::endl;
			}


			// Loop over all keys, retrieve the values and save them
			for (tVectorString::iterator LoopKey = KeyList.begin(); LoopKey != KeyList.end(); ++LoopKey)
			{
				std::string Default = "";
				std::string Key = *LoopKey;
				std::string Value = SectionKeyValueGet(Section, Key, Default);
				IniFile << Key << "=" << Value << std::endl;
			}
		}
	}

	// ******************************************************************
	// ******************************************************************
	bool YAIP::NewKeyValue(const std::string &Line, std::string &Key, std::string &Value)
	{
		// Assume no new key/value pair
		bool Success = false;
		std::smatch RegExpMatch;

		// Check for match
		if (true == std::regex_search(Line, RegExpMatch, YAIP::RegExKeyValue))
		{
			// Change new key/value pair only in case of a match.
			// Unfortunately in C++ there are no named groups possible
			// so we have to use the index of the group.
			Key = RegExpMatch[2].str();
			Value = RegExpMatch[7].str();
			Success = true;
		}

		return Success;
	}

	// ******************************************************************
	// ******************************************************************
	bool YAIP::NewSection(const std::string &Line, std::string &Section)
	{
		// Assume no new section
		bool Success = false;
		std::smatch RegExpMatch;

		// Check for match
		if (true == std::regex_search(Line, RegExpMatch, YAIP::RegExSection))
		{
			// Change section only in case of a match.
			// Unfortunately in C++ there are no named groups possible
			// so we have to use the index of the group.
			Section = RegExpMatch[4].str();
			Success = true;
		}

		return Success;
	}

	// ******************************************************************
	// ******************************************************************
	void YAIP::ParseFileContent(tVectorString &FileContent)
	{
		std::string CurrentSection = "";
		std::string CurrentKey = "";
		std::string CurrentValue = "";
		tMapStringString CurrentSectionData;

		// Insert default section - some key/value entries might exist without a section
		m_IniData.insert(std::make_pair(CurrentSection, CurrentSectionData));

		// Loop over the INI file
		for (size_t Loop = 0; Loop < FileContent.size(); Loop++)
		{
			std::string Line = FileContent.at(Loop);

			// Got a new section?
			if (true == NewSection(Line, CurrentSection))
			{
				// Add new internal structures
				tMapStringString SectionData;
				m_IniData.insert(std::make_pair(CurrentSection, SectionData));
			}

			// Got a new key/value pair?
			if (true == NewKeyValue(Line, CurrentKey, CurrentValue))
			{
				// Add new data
				m_IniData[CurrentSection].insert(std::make_pair(CurrentKey, CurrentValue));
			}
		}
	}

	// ******************************************************************
	// ******************************************************************
	void YAIP::SectionKeyKill(std::string Section, std::string Key)
	{
		// First check for section existence
		tMapMapStringString::iterator SectionDataRaw = m_IniData.find(Section);
		if (m_IniData.end() != SectionDataRaw)
		{
			// Memorize section data
			tMapStringString KeyValueData = SectionDataRaw->second;

			// Remove section
			m_IniData.erase(Section);

			// Figure out if requested key exists
			tMapStringString::iterator KeyValuePosition = KeyValueData.find(Key);
			if (KeyValueData.end() != KeyValuePosition)
			{
				// Key exists, kill key
				KeyValueData.erase(Key);
			}

			// Re-insert section with deleted key
			m_IniData.insert(std::make_pair(Section, KeyValueData));
		}
	}

	// ******************************************************************
	// ******************************************************************
	tVectorString YAIP::SectionKeyListGet(std::string Section)
	{
		tVectorString KeyList;

		// First check for section existence
		tMapMapStringString::iterator SectionDataRaw = m_IniData.find(Section);
		if (m_IniData.end() != SectionDataRaw)
		{
			tMapStringString SectionData = SectionDataRaw->second;
			// Loop over the key/value map retrieved from section
			for (tMapStringString::iterator Loop = SectionData.begin(); Loop != SectionData.end(); ++Loop)
			{
				KeyList.push_back(Loop->first);
			}
		}

		return KeyList;
	}

	// ******************************************************************
	// ******************************************************************
	void YAIP::SectionKill(std::string Section)
	{
		// First check for section existence
		tMapMapStringString::iterator SectionDataRaw = m_IniData.find(Section);
		if (m_IniData.end() != SectionDataRaw)
		{
			// Section exists, kill section
			m_IniData.erase(Section);
		}
	}

	// ******************************************************************
	// ******************************************************************
	tVectorString YAIP::SectionListGet(void)
	{
		tVectorString SectionList;

		// Loop over the major map containing all sections
		for (tMapMapStringString::iterator Loop = m_IniData.begin(); Loop != m_IniData.end(); ++Loop)
		{
			SectionList.push_back(Loop->first);
		}

		return SectionList;
	}
}
