#include "JsonParser.h"


JSONNode::JSONNode()
{
	Value = std::nullopt;
	Parent = nullptr;
}
JSONNode::JSONNode(std::shared_ptr<JSONNode> InParent) : Parent(InParent)
{

}

JSONNode::JSONNode(std::shared_ptr<JSONNode> InParent , std::string StringValue) : Parent(InParent) , Value(StringValue)
{

}

JSONNode::JSONNode(std::shared_ptr<JSONNode> InParent , double InDouble) : Parent(InParent) , Value(InDouble)
{

}

JSONToken::JSONToken(JSONTokenType InType) : Type(InType)
{
}

bool JSONTokenizer(std::string json , std::vector<JSONToken> &tokens)
{
	size_t offset = 0;
	while(offset < json.size())
	{
		char c = json[offset++];
		if(std::isspace(c))
		{
			continue;
		}
		if(c == '"')
		{
			JSONToken token(JSONTokenType::String);
			bool isValid = false;
			while(offset < json.size())
			{
				c = json[offset++];
				if(c == '"')
				{
					isValid = true;
					break;
				}
				token.Value += c;
			}
			if(!isValid)
			{
				return false;
			}
			tokens.push_back(token);
		}
		else if(c == '{')
		{
			tokens.push_back(JSONToken(JSONTokenType::CurlyBracketOpen));
		}
		else if(c == '}')
		{
			tokens.push_back(JSONToken(JSONTokenType::CurlyBracketClosed));
		}
		else if(c == '[')
		{
			tokens.push_back(JSONToken(JSONTokenType::SquareBracketOpen));
		}
		else if(c == ']')
		{
			tokens.push_back(JSONToken(JSONTokenType::SquareBracketClosed));
		}
		else if(c == ':')
		{
			tokens.push_back(JSONToken(JSONTokenType::Colon));
		}
		else if(c == ',')
		{
			tokens.push_back(JSONToken(JSONTokenType::Comma));
		}
		else if(c == '+' || c == '-' || (c >= '0' && c <= '9'))
		{
			JSONToken token(JSONTokenType::Number);
			token.Value += c;
			bool isValid = false;
			bool isDot = false;
			while(offset < json.size())
			{
				c = json[offset++];
				if(c == '.')
				{
					if(isDot)
					{
						return false;
					}
					isDot = true;
					token.Value += c;
				}
				else if(c >= '0' && c <= '9')
				{
					token.Value += c;
				}
				else
				{
					isValid = true;
					break;
				}
			}
			if(!isValid)
			{
				return false;
			}
			tokens.push_back(token);
			offset--;
		}
		else
		{
			return false;
		}
	}
	return true;
}