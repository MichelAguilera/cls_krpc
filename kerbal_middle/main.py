import asyncio
from config_tool import get_config
from controller import Controller


def main(config):
    cont = Controller(config, start_disconnected=False)
    asyncio.run(cont.run())


if __name__ == '__main__':
    main(get_config())
