
// references libraries that will be used in code
#include <iostream>
#include <string>
#include <filesystem>
#include <list>
#include <typeinfo>

#using <System.xml.dll>

// adding different scopes to program
using namespace System;
using namespace System::Xml;
using namespace System::Collections::Generic;

namespace fs = std::filesystem;

public ref class XML_DAL
{

#pragma region Public:
public:

#pragma region Constructor Calls
    XML_DAL();

#pragma endregion

#pragma region Variables

#pragma endregion

#pragma region Class Methods

    /// <summary>
    /// Opens an XmlDocument at the specified path, assigns it to the class doc
    /// </summary>
    /// <param name="path">std::string to dir containing target file</param>
    /// <param name="name">std::string with filename and extension</param>
    /// <returns></returns>
    XmlDocument^ OpenXmlFile(std::string path) {
        // concats both inputted strings after they are converted to String^
        String^ abs_path = ConvertStdString(path);

        // instantiates XmlDocument object
        XmlDocument^ temp_doc = gcnew XmlDocument();

        // opens XmlDocument object if file exists
        if (FileExist(path)) {
            temp_doc->Load(abs_path);
        }
        else {
            Console::WriteLine("Invalid File/Path");
        }

        return temp_doc;
    }

    /// <summary>
    /// Edits specified attribute of specified XmlElement
    /// </summary>
    /// <param name="node">Target XmlElement</param>
    /// <param name="att">Attribute to edit</param>
    /// <param name="value">Value to assign to the target attribute</param>
    void EditNodeAttribute(XmlElement^ node, String^ att, String^ value) {
        node->SetAttribute(att, value);
    }

    /// <summary>
    /// Adds new Node element to XMLDo
    /// </summary>
    /// <param name="doc">Parent XMLDocument</param>
    /// <param name="name">String^ name of new child XmlElement</param>
    void AddNode(XmlDocument^ doc, String^ name) {
        XmlElement^ node = doc->CreateElement(name);
    }

    /// <summary>
    /// Adds child element within another node
    /// </summary>
    /// <param name="doc">Parent XML Document</param>
    /// <param name="parent">Parent XmlElement contianing child XmlElement</param>
    /// <param name="name">String^ name of new child XmlElement</param>
    /// <param name="value">InnerText of new child XmlElement</param>
    void AddChild(XmlDocument^ doc, XmlElement^ parent, String^ name, String^ value) {
        XmlElement^ child = doc->CreateElement(name);
        child->InnerText = value;
        parent->AppendChild(child);
    }

    /// <summary>
    /// Saves XmlDocument object to specified path. Overwrites any existing file.
    /// </summary>
    /// <param name="doc">Target XmlDocument</param>
    /// <param name="path">String^ of target save location</param>
    void SaveXML(XmlDocument^ doc, String^ path) {
        doc->PreserveWhitespace = true;
        doc->Save(path);
    }

    /// <summary>
    /// Uses XmlDocument->Save to create new empty .xml file
    /// </summary>
    /// <param name="path">String^ target location to create new XmlDocument</param>
    void CreateXML(String^ path) {
        XmlDocument^ doc = gcnew XmlDocument();
        doc->PreserveWhitespace = true;
        doc->Save(path);
    }

    /// <summary>
    /// Debugging tool writes root node and all childrent to console.
    /// </summary>
    /// <param name="doc">Target XmlDocument to be printed</param>
    void PrintXML(XmlDocument^ doc) {
        Console::WriteLine(doc->DocumentElement->OuterXml);
    }

    /// <summary>
    /// Converts std::string to String^
    /// </summary>
    /// <param name="input">std::string to be converted</param>
    /// <returns>String^ with same content as inputted std::string</returns>
    String^ ConvertStdString(std::string input) {
        String^ output = gcnew String(input.c_str());
        return output;
    }

    /// <summary>
    /// Method checks if file at inputted path exists
    /// </summary>
    /// <param name="path">Absolute path to file, excluding file name and end "\"</param>
    /// <param name="name">Name of target file, including file extension</param>
    /// <returns></returns>
    bool FileExist(std::string path, std::string name) {
        std::string fullPath = path + "\\" + name;
        bool exist = fs::exists(fullPath);
        return exist;
    }

    /// <summary>
    /// Overload of FilePath() with 1 arg.
    /// </summary>
    /// <param name="path">std::string full path to file</param>
    /// <returns>bool</returns>
    bool FileExist(std::string path) {
        bool exist = fs::exists(path);
        return exist;
    }

#pragma endregion

#pragma endregion


};

#pragma region Constructor Methods

/// <summary>
/// Empty Comstructor
/// </summary>
XML_DAL::XML_DAL() {

}
#pragma endregion


#pragma region Methods

/// <summary>
/// method gets user input and returns it to calling function
/// </summary>
/// <returns>std::string of user input</returns>
std::string GetInput() {
    // instantiating a string for path
    std::string input;

    // getting an entire line of input
    std::getline(std::cin, input);


    // returning string variable path
    return input;
}

/// <summary>
/// Returns full path to file to read
/// </summary>
/// <param name="dal">XML_DAL functonal class</param>
/// <returns>std::string absolute path to file</returns>
std::string InputMenu(XML_DAL^ dal) {
    // variables
    std::string tempPath, tempName;
    bool exist = false;

    // loops if invalid path is entered
    while (exist == false) {
        // collects file path
        std::cout << "Enter File Path: \n";
        tempPath = GetInput();

        std::cout << "Enter File Name: \n";
        tempName = GetInput();

        // verifies file path
        exist = dal->FileExist(tempPath, tempName);

        // causes loop if path is invalid
        if (exist == false) {            
            std::cout << "Invalid path. Try again.";
        }
    }

    return tempPath + "\\" + tempName;
}

/// <summary>
/// Collects input for multiple save locations
/// </summary>
/// <returns>std::list<std::string> of path files</returns>
std::list<std::string> SaveMenu() {
    // variables
    std::list<std::string> temp_list;
    std::string tempPath, tempName, cont;

    // loops until user breaks loop
    while (true){
        // collects input
        std::cout << "Enter File Path: \n";
        tempPath = GetInput();

        std::cout << "Enter File Name: \n";
        tempName = GetInput();

        // asks user for continue
        std::cout << "Enter another save location? (y/n)";
        cont = GetInput();

        // adds path to list
        temp_list.push_back(tempPath + "\\" + tempName);

        // checks continue condition
        if (cont == "n") {
            break;
        }
    }

    // returns list of save paths
    return temp_list;
}

/// <summary>
/// Loops through path list and saves a copy of the xml doc to each location
/// </summary>
/// <param name="path_list">std::list<std::string> of paths</param>
/// <param name="doc">XmlDocument^</param>
/// <param name="dal">XML_DAL functional class</param>
void SaveToPath(std::list<std::string> path_list, XmlDocument^ doc, XML_DAL^ dal) {
    // foreach loop executing on each value in path_list
    for (std::string path : path_list) {
        // converting std::string to String^ with dal class method ConvertStdString
        String^ temp_path = dal->ConvertStdString(path);

        // saving XML doc to path with dal class method SaveXML
        dal->SaveXML(doc, temp_path);
    }
}

#pragma endregion


int main()
{
    // variables
    XML_DAL^ dal = gcnew XML_DAL;
    std::list<std::string> savePath_list;
    std::string readPath;

    // calling InputMenu
    std::cout << "Enter path of file to read\n";
    readPath = InputMenu(dal);

    // Calling SaveMenu
    std::cout << "Enter save path for each entered file\n";
    savePath_list = SaveMenu();

    // Reading .xml to variable
    XmlDocument^ doc = dal->OpenXmlFile(readPath);

    // printing copy of .xml to console
    dal->PrintXML(doc);

    // writing a copy of xml.doc to each path in savePath_List
    SaveToPath(savePath_list, doc, dal);    
}