import json


def get_config():
    config = json.load(open('config.json'))
    while True:
        user_input = input(
            """Press Enter to read latest config, 
            or type NEW for new config
            or type SEE to see the config
            :: """).lower()
        if user_input == "see":
            print(config)
        if user_input == "new":
            for config_item in config:
                config[config_item] = input(f"Enter {config_item}: ")
            json.dump(config, open("config.json", "w"), indent=4)
            break
        if user_input == "":
            break
    return config
