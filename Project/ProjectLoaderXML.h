#pragma once
#include "OSE-V2-STD-Modules/EngineDependencies/rapidxml-1.13/rapidxml.hpp"
#include "OSE-Core/Project/ProjectLoader.h"

namespace ose::project
{
	//Loads projects from xml text files
	class ProjectLoaderXML : public ProjectLoader
	{
	public:
		ProjectLoaderXML();
		~ProjectLoaderXML() noexcept;

		std::unique_ptr<Project> LoadProject(const std::string & project_name);
		std::unique_ptr<ProjectInfo> LoadProjectManifest(const std::string & project_path);
		std::unique_ptr<std::map<std::string, std::string>> LoadSceneDeclerations(const std::string & project_path);
		std::unique_ptr<ose::game::Tag> LoadTagDefinitions(const std::string & project_path);
		void LoadProjectSettings(const std::string & project_path);
		void LoadInputSettings(const std::string & project_path);
		std::unique_ptr<ose::game::Scene> LoadScene(const Project & project, const std::string & scene_name);
		std::unique_ptr<ose::entity::Entity> LoadEntityPrefab(const std::string & prefab_path, const Project & project);

	private:
		//Parse the XML file at 'path' and store the contents in 'content'
		//NOTE - contents must remain in memory while accessing doc as it used in-place parsing
		//@returns {std::unique_ptr<rapidxml::xml_document<>>} Pointer to the parsed document
		std::unique_ptr<rapidxml::xml_document<>> LoadXmlFile(const std::string & path, std::string & contents);

		std::unique_ptr<ose::entity::Entity> ParseEntity(rapidxml::xml_node<> * entity_node,
				std::unordered_map<std::string, std::string> & aliases, const Project & project);
		
		void ParseTag(std::vector<ose::game::Tag> & tags, rapidxml::xml_node<> * tag_node);
		void ParseResources(rapidxml::xml_node<> * resources_node, std::unordered_map<std::string, std::string> & aliases, const Project & project);

		const std::string file_extension = ".xml";
	};
}

