from listener import Listener
from sender import Sender
import krpc


class Controller:
    def __init__(self, config, start_disconnected):
        self.listener = Listener(config["address"], int(config["port"]))
        if not start_disconnected:
            self.conn = krpc.connect(
                address=config["krpc_address"],
                rpc_port=int(config["rpc_port"]),
                stream_port=int(config["stream_port"])
            )
        else:
            self.conn = {
                "space_center": {
                    "active_vessel": {
                        "name": "DUMMY VESSEL"
                    }
                }
            }
        self.sender = Sender(self.conn, config["signal_split_char"])
        self.running = False

    async def run(self):
        self.running = True
        while self.running:
            self.sender.send(self.listener.listen_step())
