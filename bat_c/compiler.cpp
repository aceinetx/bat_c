#include "token.h"
#include "instruction_structs.h"
#include <string>
#include <format>
#include <stack>
#include "var_map.h"
#include "error.h"

std::string compile(tokenarray_t tokens, std::string& assembly) {

	INSTDefine current_define;
	std::string function_def = "";

	char wi_ident_unknown_stage = 0;
	char wi_inc_dec_stage = 0;
	bool in_expression = false;
	tokenarray_t expression;
	std::string current_function = "";
	std::string expression_variable = "";
	tokenarray_t compare_expression;
	std::stack<std::string> if_stack;

	TokenType last_cmpexpr_operator = IDENTIFIER;

	WorkingInstruction current_working_instruction = WI_NOTHING;

	std::string inc_dec_inst = "";

	for (int i = 0; i < tokens.size(); i++) {
		Token& token = tokens.at(i);
		
		if (current_working_instruction == WI_NOTHING) {
			if (!in_expression) {
				if (token.type == DEFINE) {
					current_working_instruction = WI_DEFINE;
				}
				else if (token.type == IDENTIFIER) {
					wi_ident_unknown_stage = -1;
					current_working_instruction = WI_IDENT_UNKNOWN;
				}
				else if (token.type == INCREMENT || token.type == DECREMENT) {
					wi_inc_dec_stage = -1;
					current_working_instruction = WI_INCREMENT_DECREMENT;
				}
				else if (token.type == RSBRACKET && !function_def.empty()) {
					assembly.append("ret\n");
					function_def.clear();
				}
				else if (token.type == COMPARE_EXPR) {
					current_working_instruction = WI_COMPARE_EXPR;
				}
				else if (token.type == IF) {
					std::string label = ".if_" + std::to_string(i);
					if_stack.push(label);
					if (last_cmpexpr_operator == EQUALTO) {
						assembly.append("brh ne " + label + "\n");
					}
					if (last_cmpexpr_operator == NOTEQUALTO) {
						assembly.append("brh eq " + label + "\n");
					}
					if (last_cmpexpr_operator == GREATER) {
						assembly.append("brh lt " + label + "\n");
					}
					if (last_cmpexpr_operator == LESS) {
						assembly.append("brh ge " + label + "\n");
					}
					if (last_cmpexpr_operator == GREATEREQUAL) {
						assembly.append("brh lt " + label + "\n");
					}
					if (last_cmpexpr_operator == LESSEQUAL) {
						assembly.append("brh ge " + label + "\n");
					}
				}
				else if (token.type == ENDIF) {
					std::string label = if_stack.top();
					if_stack.pop();

					assembly.append(label + "\n");
				}
			}
			else {
				if (token.type == ENDL) {
					in_expression = false;
					Token& left_part = expression.at(0);

					if (expression.size() == 1) {
						if (left_part.type == IDENTIFIER) {
							assembly.append("ldi r4 " + std::to_string(VarMapGet(left_part.value, current_function)) + "\n");
							assembly.append("lod r4 r13 r0\n");
						}
						else if (left_part.type == NUMBER) {
							assembly.append("ldi r13 " + std::to_string(left_part.value_long) + "\n");
						}
					}
					else {

						Token& op = expression.at(1);
						Token& right_part = expression.at(2);
						if (left_part.type == IDENTIFIER) {
							assembly.append("ldi r4 " + std::to_string(VarMapGet(left_part.value, current_function)) + "\n");
							assembly.append("lod r4 r11 r0\n");
						}
						else if (left_part.type == NUMBER) {
							assembly.append("ldi r11 " + std::to_string(left_part.value_long) + "\n");
						}

						if (right_part.type == IDENTIFIER) {
							assembly.append("ldi r4 " + std::to_string(VarMapGet(right_part.value, current_function)) + "\n");
							assembly.append("lod r4 r12 r0\n");
						}
						else if (right_part.type == NUMBER) {
							assembly.append("ldi r12 " + std::to_string(right_part.value_long) + "\n");
						}

						if (op.type == PLUS) assembly.append("add r11 r12 r13\n");
						else if (op.type == MINUS) assembly.append("sub r11 r12 r13\n");
						else if (op.type == MUL) {
							std::string label = ".mul_" + std::to_string(i);
							assembly.append("ldi r13 0\n");
							assembly.append(label + "\n");
							assembly.append("add r13 r12 r13\n");
							assembly.append("dec r11\n");
							assembly.append("cmp r11 r0\n");
							assembly.append("brh ne " + label + "\n");

						}
						else if (op.type == DIV) {
							throw_error("Division is not supported", token);
						}
						else {
							throw_error("Invalid operator", token);
						}
					}
					assembly.append("ldi r4 " + std::to_string(VarMapGet(expression_variable, current_function)) + "\n");
					assembly.append("str r4 r13 r0\n");
					expression.clear();
				}
				else {
					expression.push_back(token);
				}
			}
		}
		else if (current_working_instruction == WI_DEFINE) {
			if (current_define.name == (char*)-1) {
				if (token.type != IDENTIFIER) throw_error("token type isn't identifier in DEFINE", token);

				current_define.name = token.value;
			}
			else {
				if (token.type != NUMBER) throw_error("token type isn't number in DEFINE", token);
				current_define.value = token.value_long;

				std::string define_name_cppstr; define_name_cppstr.append(current_define.name);

				std::string asm_line = "define " + define_name_cppstr + " " + std::to_string(current_define.value) + "\n";
				assembly.insert(0, asm_line);
				
				current_define.name = (char*)-1;
				current_define.value = 0;
				current_working_instruction = WI_NOTHING;
			}
		}
		else if (current_working_instruction == WI_IDENT_UNKNOWN) {
			if (wi_ident_unknown_stage == -1) {
				wi_ident_unknown_stage = 0;
				i -= 2;
			} 
			else if (wi_ident_unknown_stage == 0) {
				function_def.clear();
				function_def.append(token.value);
				wi_ident_unknown_stage = 1;
			}
			else if (wi_ident_unknown_stage == 1) {
				if (token.type == LSBRACKET) {
					assembly.append("." + function_def + "\n");
					current_function = function_def;
				}
				else if (token.type == ENDL) {
					VarMapAdd(function_def, current_function);
					/*std::string offset = std::to_string(var_map.back().offset);
					assembly.append("ldi r4 " + offset + "\n");
					assembly.append("str r4 r0 r0\n");*/
				}
				else if (token.type == LPAREN) {
					i++;
					assembly.append("cal ." + function_def + "\n");
				}
				else if (token.type == EQUAL) {
					expression_variable = function_def;
					in_expression = true;
				}
				wi_ident_unknown_stage = -1;
				current_working_instruction = WI_NOTHING;
				
			}
		}
		else if (current_working_instruction == WI_INCREMENT_DECREMENT) {
			if (wi_inc_dec_stage == -1) {
				wi_inc_dec_stage = 0;
				i -= 2;
			}
			else if (wi_inc_dec_stage == 0) {
				if (token.type == INCREMENT) inc_dec_inst = "inc";
				if (token.type == DECREMENT) inc_dec_inst = "dec";
				wi_inc_dec_stage = 1;
			}
			else if (wi_inc_dec_stage == 1) {
				std::string offset = std::to_string(VarMapGet(token.value, current_function));
				assembly.append("ldi r4 " + offset + "\n");
				assembly.append("lod r4 r13 r0\n");
				assembly.append(inc_dec_inst + " r13\n");
				assembly.append("str r4 r13 r0\n");
				wi_inc_dec_stage = -1;
				current_working_instruction = WI_NOTHING;
			}
		}
		else if (current_working_instruction == WI_COMPARE_EXPR) {
			if (token.type != ENDL) {
				compare_expression.push_back(token);
			}
			else {
				current_working_instruction = WI_NOTHING;

				Token& left_part = compare_expression.at(0);
				Token& op = compare_expression.at(1);
				Token& right_part = compare_expression.at(2);


				if (left_part.type == IDENTIFIER) {
					assembly.append("ldi r4 " + std::to_string(VarMapGet(left_part.value, current_function)) + "\n");
					assembly.append("lod r4 r12 r0\n");
				}
				else if (left_part.type == NUMBER) {
					assembly.append("ldi r12 " + std::to_string(left_part.value_long) + "\n");
				}

				if (right_part.type == IDENTIFIER) {
					assembly.append("ldi r4 " + std::to_string(VarMapGet(right_part.value, current_function)) + "\n");
					assembly.append("lod r4 r13 r0\n");
				}
				else if (right_part.type == NUMBER) {
					assembly.append("ldi r13 " + std::to_string(right_part.value_long) + "\n");
				}

				if (op.type == GREATER) assembly.append("dec r12\n");
				if (op.type == LESSEQUAL) assembly.append("dec r12\n");

				assembly.append("cmp r12 r13\n");
				last_cmpexpr_operator = op.type;

				compare_expression.clear();
			}
		}
	}

	return assembly;
}