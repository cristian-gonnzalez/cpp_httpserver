#pragma once

#include <map>
#include <memory>
#include <string>
#include <unordered_map>

#include "command.h"
#include "cmd_add.h"

enum class CommandId : int
{
    ADD,
};

class CommandHandler
{
    public:
        CommandHandler()
        {
            _cmd_lookup["add"] = CommandId::ADD;
            _commands[CommandId::ADD] = std::make_unique<CommandAdd>();
        }

        ~CommandHandler() = default;

        CommandId str_to_cmd_id(const std::string& str)
        {
            if (_cmd_lookup.contains(str))
                return _cmd_lookup[ str ];

            throw std::runtime_error("Unknown command: " + str);
        }

        std::string handle(const std::string& cmd_name, const std::string& input)
        {
            CommandId id = str_to_cmd_id(cmd_name);
            if ( _commands.contains(id) )
                return _commands[ id ]->execute(input);
                
            throw std::runtime_error("Command not registered");
        }

    private:
        std::unordered_map< std::string, 
                            CommandId>         _cmd_lookup{};
        std::map< CommandId, 
                  std::unique_ptr< Command > > _commands{};
};
