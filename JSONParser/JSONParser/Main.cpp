
#include <iostream>

#include "JsonParser.h"


std::ostream &operator<<(std::ostream &os , JSONNode &node)
{
	if(node.Is<jsonString>())
	{
		os << node.As<jsonString>();
	}
	else if(node.Is<jsonNumber>())
	{
		os << node.As<jsonNumber>();
	}
	else if(node.Is<jsonArray>())
	{
		os << "[ ";
		for(std::shared_ptr<JSONNode> &ChildNode : node.As<jsonArray>())
		{
			os << *ChildNode << ", ";
		}
		os << " ]";
	}
	else if(node.Is<jsonObject>())
	{
		os << "{ ";
		for(auto &pair : node.As<jsonObject>())
		{
			os << pair.first << " : " << *pair.second << ", ";
		}
		os << " }";
	}
	else
	{
		os << "AAAA";
	}
	return os;
}

std::ostream &operator<<(std::ostream &os , const JSONTokenType &type)
{
	switch(type)
	{
		case JSONTokenType::CurlyBracketOpen:
			os << "CurlyBracketOpen";
			break;
		case JSONTokenType::CurlyBracketClosed:
			os << "CurlyBracketClosed";
			break;
		case JSONTokenType::Colon:
			os << "Colon";
			break;
		case JSONTokenType::String:
			os << "String";
			break;
		case JSONTokenType::Number:
			os << "Number";
			break;
		case JSONTokenType::SquareBracketOpen:
			os << "SquareBracketOpen";
			break;
		case JSONTokenType::SquareBracketClosed:
			os << "SquareBracketClosed";
			break;
		case JSONTokenType::Comma:
			os << "Comma";
			break;
		case JSONTokenType::Boolean:
			os << "Boolean";
			break;
		case JSONTokenType::Null:
			os << "Null";
			break;
		default:
			break;
	}
	return os;
}


int main(int argc , char *argv)
{
	std::string json = "{ \"isJson\": 12} ";

	std::string complexInlineJSON = "{\"integer\": 123, \"double\": 12.3, \"string\": \"Hello World\", \"lista\": [\"item1\", \"item2\", 12, 78.8], \"object\": {\"key1\": \"value2\", \"key3\": [1,2,3,4,5,6,7,8, \"end\"] }}";

	jsonVariant variant = 15.0;
	std::cout << std::holds_alternative<jsonNumber>(variant) << std::endl;

	std::cout << "Value is " << std::get<jsonNumber>(variant) << std::endl;

	std::vector<JSONToken> tokens;
	std::shared_ptr<JSONNode> currentNode = nullptr;
	std::shared_ptr<JSONNode> rootNode = nullptr;

	bool Colon = false;
	std::string currentKey;
	if(JSONTokenizer(complexInlineJSON , tokens))
	{
		for(const JSONToken &token : tokens)
		{

			if(token.Type == JSONTokenType::CurlyBracketOpen)
			{
				currentNode = std::make_shared<JSONNode>(currentNode);
				currentNode->Value = jsonObject();
				if(rootNode == nullptr)
				{
					rootNode = currentNode;
				}
				if(currentNode->Parent && currentNode->Parent->Is<jsonObject>())
				{
					currentNode->Parent->As<jsonObject>()[currentKey] = currentNode;
					Colon = false;
				}
			}
			else if(token.Type == JSONTokenType::CurlyBracketClosed)
			{
				currentNode = currentNode->Parent;
			}
			else if(token.Type == JSONTokenType::SquareBracketOpen)
			{
				currentNode = std::make_shared<JSONNode>(currentNode);
				currentNode->Value = jsonArray();
				if(rootNode == nullptr)
				{
					rootNode = currentNode;
				}
				if(currentNode->Parent && currentNode->Parent->Is<jsonObject>())
				{
					currentNode->Parent->As<jsonObject>()[currentKey] = currentNode;
					Colon = false;
				}
			}
			else if(token.Type == JSONTokenType::SquareBracketClosed)
			{
				currentNode = currentNode->Parent;
			}
			else if(token.Type == JSONTokenType::String)
			{

				if(currentNode->Is<jsonArray>())
				{
					std::shared_ptr<JSONNode> newNode = std::make_shared<JSONNode>(currentNode , token.Value);
					std::cout << currentNode->As<jsonArray>().size() << std::endl;
					currentNode->As<jsonArray>().push_back(newNode);

				}
				else if(currentNode->Is<jsonObject>())
				{

					if(Colon == false)
					{
						currentKey = token.Value;
					}
					else
					{
						Colon = false;
						currentNode->As<jsonObject>()[currentKey] = std::make_shared<JSONNode>(currentNode , token.Value);
					}
				}

			}
			else if(token.Type == JSONTokenType::Number)
			{
				if(currentNode->Is<jsonArray>())
				{
					std::shared_ptr<JSONNode> newNode = std::make_shared<JSONNode>(currentNode , std::stod(token.Value));
					std::cout << currentNode->As<jsonArray>().size() << std::endl;
					currentNode->As<jsonArray>().push_back(newNode);
				}
				else if(currentNode->Is<jsonObject>())
				{
					Colon = false;
					currentNode->As<jsonObject>()[currentKey] = std::make_shared<JSONNode>(currentNode , std::stod(token.Value));
				}
			}
			else if(token.Type == JSONTokenType::Colon)
			{

				Colon = true;
			}

			std::cout << "Token Type is " << token.Type << " and value is " << token.Value << std::endl;
		}
		if(rootNode != nullptr)
		{
			if(rootNode->Is<jsonArray>())
			{
				std::cout << "RootNode is an array \n";
			}
			if(rootNode->Is<jsonObject>())
			{

				std::cout << "RootNode is a JsonObject \n";

			}

			std::cout << *rootNode << "\n";
		}
	}

	std::getchar();
}