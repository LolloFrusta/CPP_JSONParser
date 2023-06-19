#pragma once

#include <string>
#include <variant>
#include <optional>
#include <vector>
#include <memory>
#include <unordered_map>



	struct JSONNode;

	using jsonNumber = double;
	using jsonString = std::string;
	using jsonBool = bool;
	using jsonArray = std::vector<std::shared_ptr<JSONNode>>;
	using jsonObject = std::unordered_map<std::string , std::shared_ptr<JSONNode>>;

	using jsonVariant = std::variant<jsonNumber , jsonString , jsonBool , jsonArray , jsonObject>;

	struct JSONNode
	{
		std::optional<jsonVariant> Value;
		std::shared_ptr<JSONNode> Parent;
		JSONNode();
		JSONNode(std::shared_ptr<JSONNode> InParent);
		JSONNode(std::shared_ptr<JSONNode> InParent , std::string StringValue);
		JSONNode(std::shared_ptr<JSONNode> InParent , double InDouble);


		template <typename T>
		bool Is() const
		{
			return Value.has_value() ? std::holds_alternative<T>(Value.value()) : false;
		}


		template <typename T>
		T &As()
		{
			return std::get<T>(Value.value());
		}


	};

	enum class JSONTokenType
	{
		CurlyBracketOpen ,
		CurlyBracketClosed ,
		Colon ,
		String ,
		Number ,
		SquareBracketOpen ,
		SquareBracketClosed ,
		Comma ,
		Boolean ,
		Null ,
	};

	struct JSONToken
	{
		JSONTokenType Type;
		std::string Value;

		JSONToken(JSONTokenType InType);

	};

	bool JSONTokenizer(std::string json , std::vector<JSONToken> &tokens);